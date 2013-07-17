#include "usbterm.h"
#include "flogfs.h"
#include <string.h>

using namespace Platform;

USBFile * USBTerm::usbfile = nullptr;

void USBTerm::reg_file(USBFile* file){
	chMtxLock(&file_lock);
	USBFile * iter;
	if(usbfile){
		iter = usbfile;
		while(iter->next) iter = iter->next;
		iter->next = file;
	} else {
		usbfile = file;
	}
	file->next = nullptr;
	chMtxUnlock();
}



ShellCommand const USBTerm::commands[] = {
	{"listcmds",
	 "[s]()",
	 (ShellCommand::shell_callback_t)&USBTerm::cmd_listcmds},
	{"help",
	 "s()",
	 (ShellCommand::shell_callback_t)&USBTerm::cmd_help},
	{"settime",
	 "(datetime:date)",
	 (ShellCommand::shell_callback_t)&USBTerm::cmd_settime},
	{"ping",
	 "s(s:ping)",
	 (ShellCommand::shell_callback_t)&USBTerm::cmd_ping},
	{"listbuffers",
	 "[s:name,u:length,s:format,s:storage]()",
	 (ShellCommand::shell_callback_t)&USBTerm::cmd_listbuffers},
	{"fetchbuffer",
	 "u,logbuffer(s:buffer_name)",
	 (ShellCommand::shell_callback_t)&USBTerm::cmd_fetchbuffer},
	{"liststreams",
	 "[s:name,s:format]()",
	 (ShellCommand::shell_callback_t)&USBTerm::cmd_liststreams},
	{"startstream",
	 "stream(s:stream_name)",
	 (ShellCommand::shell_callback_t)&USBTerm::cmd_startstream},
	{"fs_format",
	 "()",
	 (ShellCommand::shell_callback_t)&USBTerm::cmd_fs_format},
	{"fs_mount",
	 "()",
	 (ShellCommand::shell_callback_t)&USBTerm::cmd_fs_mount},
	{"fs_ls",
	 "[s]()",
	 (ShellCommand::shell_callback_t)&USBTerm::cmd_fs_ls},
	{"fs_append",
	 "(s:name,buffer:data:max=1024)",
	 (ShellCommand::shell_callback_t)&USBTerm::cmd_fs_append},
	{"fs_read",
	 "[buffer](s:name,u:len)", // Returns an array of buffers
	 (ShellCommand::shell_callback_t)&USBTerm::cmd_fs_read},
	{"flash_read_sector",
	 "buffer:data,buffer:spare(u:block,u:sector)",
	 (ShellCommand::shell_callback_t)&USBTerm::cmd_flash_read_sector},
	{"flash_write_sector",
	 "(u:block,u:sector,buffer:data:max=512,buffer:spare:max=4)",
	 (ShellCommand::shell_callback_t)&USBTerm::cmd_flash_write_sector},
	{"flash_erase_block",
	 "(u:block)",
	 (ShellCommand::shell_callback_t)&USBTerm::cmd_flash_erase_block},
};

uint32_t const USBTerm::num_commands = sizeof(commands)/sizeof(*commands);

static inline bool is_ascii_num(char n){return (n >= '0') && (n <= '9');}

msg_t USBTerm::terminate(){
	chThdTerminate(thread);
	return chThdWait(thread);
}

void USBTerm::thread_action(){
	//Euclidean3_f32 m;
	uint8_t command[256];
	uint8_t *iter;
	uint32_t i;
	
	usbserial1.init();
	
	iter = command;
	
	enum {
		ST_IDLE,
		ST_READING,
	} state = ST_IDLE;
	
	uint_fast8_t length = 0;
	uint8_t silent_count = 0;
	
	while(!chThdShouldTerminate()){
		// Try to read a byte
		if(!usbserial1.read(iter, 1, MS2ST(50))){
			if(++silent_count > 10){
				state = ST_IDLE;
				iter = command;
				length = 0;
			}
			continue;
		}
		// Reset silence counter
		silent_count = 0;
		
		switch(state){
		case ST_IDLE:
			length = *iter;
			// Ignore 0-length commands
			if(length){
				state = ST_READING;
			}
			break;
		case ST_READING:
			if(--length == 0){
				// Command complete
				
				for(i = 0; i < num_commands; i++){
					if(commands[i].match(command, iter + 1 - command)){
						usbserial1.write_byte(
							commands[i].call(command, *this)
						);
						break;
					}
				}
				if(i == num_commands){
					// No matching command
					usbserial1.write_byte(0);
				}
				// Reset pointer
				iter = command;
				state = ST_IDLE;
			} else {
				// Increment pointer
				iter += 1;
			}
			break;
		}
	}
	usbserial1.stop();
}

int32_t USBTerm::cmd_help(const char* cmd){
	chprintf(usbserial1.stream(), "You did something wrong");
	usbserial1.write_byte(0);
	return 0;
}


int32_t USBTerm::cmd_settime(const char* cmd){
	rtc1::rtc_time_t time;
	
	parse_string(cmd);
	
	if(!is_ascii_num(cmd[0])) goto error;
	if(!is_ascii_num(cmd[1])) goto error;
	if(cmd[2] != ':') return 1;
	if(!is_ascii_num(cmd[3])) goto error;
	if(!is_ascii_num(cmd[4])) goto error;
	if(cmd[5] != ':') return 1;
	if(!is_ascii_num(cmd[6])) goto error;
	if(!is_ascii_num(cmd[7])) goto error;
	
	
	time.years_ten = cmd[0] - '0';
	time.years_u = cmd[1] - '0';
	time.months_ten = cmd[3] - '0';
	time.months_u = cmd[4] - '0';
	time.days_ten = cmd[6] - '0';
	time.days_u = cmd[7] - '0';
	
	cmd = &cmd[8];
	while(*cmd == ' ') cmd++;
	
	if(!is_ascii_num(cmd[0])) goto error;
	if(!is_ascii_num(cmd[1])) goto error;
	if(cmd[2] != ':') return 1;
	if(!is_ascii_num(cmd[3])) goto error;
	if(!is_ascii_num(cmd[4])) goto error;
	if(cmd[2] != ':') return 1;
	if(!is_ascii_num(cmd[6])) goto error;
	if(!is_ascii_num(cmd[7])) goto error;
	
	time.hours_ten = cmd[0] - '0';
	time.hours_u = cmd[1] - '0';
	time.minutes_ten = cmd[3] - '0';
	time.minutes_u = cmd[4] - '0';
	time.seconds_ten = cmd[6] - '0';
	time.seconds_u = cmd[7] - '0';
	
	rtc1::set_time(time);
	
	return 0;
	
error:
	return 1;
}

int32_t USBTerm::cmd_ping(const char* cmd){
	// Advance beyond "ping"
	parse_string(cmd);
	// Send back that string
	chprintf(usbserial1.stream(), parse_string(cmd));
	// Seal it
	usbserial1.write_byte(0);
	return 0;
}

int32_t USBTerm::cmd_listcmds(const char* cmd){
	uint_fast8_t i;
	usbserial1.write_byte(num_commands);
	for(i = 0; i < num_commands; i++){
		chprintf(usbserial1.stream(), "%s %s", commands[i].get_root(), commands[i].get_args());
		usbserial1.write_byte(0);
	}
	usbserial1.write_byte(0);
	return 0;
}

int32_t USBTerm::cmd_listbuffers(const char* cmd){
	flog_read_file_t r;
	flog_write_file_t w;
	uint8_t test[50];

// 	flogfs_init();
//
// 	flogfs_format();
//
// 	flogfs_mount();
// 	flogfs_open_write(&w, "test");
// 	flogfs_write(&w, test, 50);
//
// 	flogfs_open_read(&r, "test");
//
// 	flogfs_read(&r, test, 50);
// 	flogfs_close_read(&r);
// 	flogfs_close_write(&w);
//
// 	flogfs_rm("test");

	return 0;
}

int32_t USBTerm::cmd_fetchbuffer(const char* cmd){
	return 0;
}

int32_t USBTerm::cmd_liststreams ( const char* cmd ) {
	return 0;
}

static uint8_t usb_flash_buffer[512];

int32_t USBTerm::cmd_flash_read_sector ( const char* cmd ) {
	uint32_t block, sector;
	uint8_t spare_buffer[4];
	parse_string(cmd);
	block = parse_uint(cmd);
	sector = parse_uint(cmd);

	flash.lock();

	bool success = flash.page_open(block, sector / 4);
	flash.page_read_continued(usb_flash_buffer, 512 * (sector % 4), 512);
	flash.page_read_continued(spare_buffer, 0x804 + 16 * (sector % 4), 4);

	flash.unlock();

	write_value<uint32_t>(512);
	Platform::usbserial1.write_buffer((uint8_t const *)usb_flash_buffer, 512);

	write_value<uint32_t>(4);
	Platform::usbserial1.write_buffer((uint8_t const *)spare_buffer, 4);

	return (success) ? 0 : 1;
}


int32_t USBTerm::cmd_flash_write_sector ( const char* cmd ) {
	uint32_t block, sector;
	uint32_t len;
	uint8_t const * buffer;
	parse_string(cmd);
	block = parse_uint(cmd);
	sector = parse_uint(cmd);
	flash.lock();
	flash.page_open(block, sector / 4);


	// Main data
	buffer = parse_buffer(cmd, len);
	flash.page_write_continued((uint8_t const *)buffer, 512 * (sector % 4), len);

	// Spare data
	buffer = parse_buffer(cmd, len);
	flash.page_write_continued((uint8_t const *)buffer,
	                           0x804 + 16 * (sector % 4), len);

	flash.page_commit();
	flash.unlock();

	return 0;
}


int32_t USBTerm::cmd_flash_erase_block ( const char* cmd ) {
	uint32_t block;
	bool success;
	parse_string(cmd);
	block = parse_uint(cmd);
	flash.lock();

	success = flash.erase_block(block);

	flash.unlock();

	return (success) ? 0 : 1;
}

int32_t USBTerm::cmd_fs_format ( const char* cmd ) {
	if(flogfs_format() == FLOG_SUCCESS)
		return 0;
	return 1;
}

int32_t USBTerm::cmd_fs_mount ( const char* cmd ) {
	if(flogfs_mount() == FLOG_SUCCESS)
		return 0;
	return 1;
}


int32_t USBTerm::cmd_fs_ls ( const char* cmd ) {
	char fname[FLOG_MAX_FNAME_LEN];
	flog_ls_iterator_t iter;
	flogfs_start_ls(&iter);
	while(flogfs_ls_iterate(&iter, fname)){
		usbserial1.write_byte(1);
		chprintf(usbserial1.stream(), fname);
	}
	flogfs_stop_ls(&iter);
	// And close the array
	usbserial1.write_byte(0);
	return 0;
}

int32_t USBTerm::cmd_fs_append ( const char* cmd ) {
	flog_write_file_t f;
	char const * fname;
	uint8_t const * data;
	uint32_t len;
	parse_string(cmd);
	fname = parse_string(cmd);
	data = parse_buffer(cmd, len);

	if(flogfs_open_write(&f, fname) != FLOG_SUCCESS){
		return 1;
	}

	if(flogfs_write(&f, data, len) != len){
		flogfs_close_write(&f);
		return 1;
	}

	if(flogfs_close_write(&f) != FLOG_SUCCESS){
		return 1;
	}
	return 0;
}

int32_t USBTerm::cmd_fs_read ( const char* cmd ) {
	uint8_t buffer[128];
	flog_read_file_t f;
	char const * fname;
	uint32_t to_read, num_read;

	uint32_t len;
	parse_string(cmd);
	fname = parse_string(cmd);
	len = parse_uint(cmd);

	if(flogfs_open_read(&f, fname) != FLOG_SUCCESS){
		return 1;
	}
	while(len){
		to_read = min(len, 128);
		num_read = flogfs_read(&f, buffer, to_read);
		if(num_read){
			// Sending one buffer element
			usbserial1.write_byte(1);
			write_value<uint32_t>(num_read);
			usbserial1.write_buffer(buffer, num_read);
		}
		if(num_read != to_read){
			// EOF
			break;
		}
	}

	// Close the array
	usbserial1.write_byte(0);

	if(flogfs_close_read(&f) != FLOG_SUCCESS){
		return 1;
	}
	return 0;
}


int32_t USBTerm::cmd_startstream ( const char* cmd ) {
	return 0;
}


int32_t USBTerm::parse_int(const char*& str){
	int32_t i = *((int32_t const *)str);
	str += sizeof(int32_t);
	return i;
}

char const * USBTerm::parse_string(const char*& str){
	char const * const ret = str;
	// This will advance str one beyond the 0-terminator
	while(*(str++) != 0);
	return ret;
}

uint32_t USBTerm::parse_uint(const char*& str){
	uint32_t i = *((uint32_t const *)str);
	str += sizeof(uint32_t);
	return i;
}

uint8_t const * USBTerm::parse_buffer ( const char*& str, uint32_t& len ) {
	len = parse_uint(str);
	uint8_t const * const ret = reinterpret_cast<uint8_t const *>(str);
	str += len;
	return ret;
}




