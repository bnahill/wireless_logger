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
	 "(s:name,buffer:data:maxpkt=512)",
	 (ShellCommand::shell_callback_t)&USBTerm::cmd_fs_append},
	{"fs_read",
	 "buffer(s:name,u:len)", // Returns an array of buffers
	 (ShellCommand::shell_callback_t)&USBTerm::cmd_fs_read},
	 {"fs_rm",
	 "(s:name)",
	 (ShellCommand::shell_callback_t)&USBTerm::cmd_fs_rm},
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
	uint8_t command[64];
	uint8_t *iter;
	uint32_t i;
	
	usbserial1.init();
	
	iter = command;
	
	enum {
		ST_IDLE,
		ST_READING,
	} state = ST_IDLE;
	
	uint_fast8_t len_bytes_recvd = 0;
	union {
		uint8_t len_buffer[4];
		uint32_t length;
	};
	length = 0;

	uint8_t silent_count = 0;
	
	while(!chThdShouldTerminate()){
		// Try to read a byte
		if(!usbserial1.read(iter, 1, MS2ST(50))){
			if(++silent_count > 10){
				state = ST_IDLE;
				iter = command;
				length = 0;
				len_bytes_recvd = 0;
			}
		} else {
			// Reset silence counter
			silent_count = 0;
			if(*iter == 0){
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
					usbserial1.write_byte(255);
				}
				iter = command;
			} else {
				iter += 1;
			}
		}

		continue;
		
// 		switch(state){
// 		case ST_IDLE:
// 			len_buffer[len_bytes_recvd] = *iter;
// 			// Ignore 0-length commands
// 			if(++len_bytes_recvd == 4){
// 				state = ST_READING;
// 				len_bytes_recvd = 0;
// 			}
// 			break;
// 		case ST_READING:
// 			if(--length == 0){
// 				// Command complete
//
// 				for(i = 0; i < num_commands; i++){
// 					if(commands[i].match(command, iter + 1 - command)){
// 						usbserial1.write_byte(
// 							commands[i].call(command, *this)
// 						);
// 						break;
// 					}
// 				}
// 				if(i == num_commands){
// 					// No matching command
// 					usbserial1.write_byte(0);
// 				}
// 				// Reset pointer
// 				iter = command;
// 				state = ST_IDLE;
// 			} else {
// 				// Increment pointer
// 				iter += 1;
// 			}
// 			break;
// 		}
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
	uint8_t buffer[8];

	usbserial1.read(buffer, 8, MS2ST(parse_byte_timeout));
	
	if(!is_ascii_num(buffer[0])) goto error;
	if(!is_ascii_num(buffer[1])) goto error;
	if(buffer[2] != ':') return 1;
	if(!is_ascii_num(buffer[3])) goto error;
	if(!is_ascii_num(buffer[4])) goto error;
	if(buffer[5] != ':') return 1;
	if(!is_ascii_num(buffer[6])) goto error;
	if(!is_ascii_num(buffer[7])) goto error;
	
	
	time.years_ten = buffer[0] - '0';
	time.years_u = buffer[1] - '0';
	time.months_ten = buffer[3] - '0';
	time.months_u = buffer[4] - '0';
	time.days_ten = buffer[6] - '0';
	time.days_u = buffer[7] - '0';
	
	do {
		usbserial1.read(buffer, 1, MS2ST(parse_byte_timeout));
	} while(*buffer == ' ');
	usbserial1.read(buffer + 1, 7, MS2ST(parse_byte_timeout));
	
	if(!is_ascii_num(buffer[0])) goto error;
	if(!is_ascii_num(buffer[1])) goto error;
	if(buffer[2] != ':') return 1;
	if(!is_ascii_num(buffer[3])) goto error;
	if(!is_ascii_num(buffer[4])) goto error;
	if(buffer[2] != ':') return 1;
	if(!is_ascii_num(buffer[6])) goto error;
	if(!is_ascii_num(buffer[7])) goto error;
	
	time.hours_ten = buffer[0] - '0';
	time.hours_u = buffer[1] - '0';
	time.minutes_ten = buffer[3] - '0';
	time.minutes_u = buffer[4] - '0';
	time.seconds_ten = buffer[6] - '0';
	time.seconds_u = buffer[7] - '0';
	
	Platform::rtc1::set_time(time);
	
	return 0;
	
error:
	return 1;
}

int32_t USBTerm::cmd_ping(const char* cmd){
	bool err;
	char pong[64];
	uint32_t len;
	parse_string(pong, sizeof(pong), len, err);
	if(err)
		return 1;
	usbserial1.write_buffer(reinterpret_cast<uint8_t const *>(pong), len);
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
// 	flog_read_file_t r;
// 	flog_write_file_t w;
// 	uint8_t test[50];

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

static union {
	flog_write_file_t write_file;
	flog_read_file_t read_file;
};

int32_t USBTerm::cmd_flash_read_sector ( const char* cmd ) {
	uint32_t block, sector;
	uint8_t spare_buffer[4];
	uint8_t sector_buffer[512];
	bool err;

	block = parse_data<uint32_t>(err);
	sector = parse_data<uint32_t>(err);
	if(err)
		return 1;

	flash.lock();

	bool success = flash.page_open(block, sector / 4);
	flash.page_read_continued(sector_buffer, 512 * (sector % 4), 512);
	flash.page_read_continued(spare_buffer, 0x804 + 16 * (sector % 4), 4);

	flash.unlock();

	write_value<uint32_t>(512);
	Platform::usbserial1.write_buffer(sector_buffer, 512);
	write_value<uint32_t>(0);

	write_value<uint32_t>(4);
	Platform::usbserial1.write_buffer(spare_buffer, 4);
	write_value<uint32_t>(0);

	return (success) ? 0 : 1;
}


int32_t USBTerm::cmd_flash_write_sector ( const char* cmd ) {
	static constexpr uint32_t bufferlen = 512;
	uint32_t block, sector;
	uint32_t len;
	uint8_t buffer[bufferlen];

	bool err;

	block = parse_data<uint32_t>(err);
	sector = parse_data<uint32_t>(err);

	if(err) return 1;

	flash.lock();
	flash.page_open(block, sector / 4);


	// Main data
	parse_buffer(buffer, bufferlen, len, err);
	flash.page_write_continued((uint8_t const *)buffer, 512 * (sector % 4), len);

	// Spare data
	parse_buffer(buffer, bufferlen, len, err);
	flash.page_write_continued((uint8_t const *)buffer,
	                           0x804 + 16 * (sector % 4), len);

	flash.page_commit();
	flash.unlock();

	return 0;
}


int32_t USBTerm::cmd_flash_erase_block ( const char* cmd ) {
	uint32_t block;
	bool err;
	block = parse_data<uint32_t>(err);
	if(err) return 1;
	flash.lock();

	err = !flash.erase_block(block);

	flash.unlock();

	return (err) ? 1 : 0;
}

int32_t USBTerm::cmd_fs_format ( const char* cmd ) {
	if(flogfs_format() == FLOG_SUCCESS)
		return 0;
	return 1;
}

int32_t USBTerm::cmd_fs_mount ( const char* cmd ) {
	flogfs_init();
	if(flogfs_mount() == FLOG_SUCCESS)
		return 0;
	return 1;
}


int32_t USBTerm::cmd_fs_ls ( const char* cmd ) {
	char fname[FLOG_MAX_FNAME_LEN];
	flogfs_ls_iterator_t iter;
	flogfs_start_ls(&iter);
	while(flogfs_ls_iterate(&iter, fname)){
		usbserial1.write_byte(1);
		chprintf(usbserial1.stream(), fname);
		usbserial1.write_byte(0);
	}
	flogfs_stop_ls(&iter);
	// And close the array
	usbserial1.write_byte(0);
	return 0;
}

int32_t USBTerm::cmd_fs_append ( const char* cmd ) {
	char fname[FLOG_MAX_FNAME_LEN + 1];
	uint8_t buffer[512];
	uint32_t len;
	bool err;
	parse_string(fname, FLOG_MAX_FNAME_LEN, len, err);

	if(flogfs_open_write(&write_file, fname) != FLOG_SUCCESS){
		return 1;
	}
	while(parse_buffer_pkt(buffer, sizeof(buffer), len, err)){
		if(err || (flogfs_write(&write_file, buffer, len) != len)){
			flogfs_close_write(&write_file);
			return 1;
		}
	}

	if(flogfs_close_write(&write_file) != FLOG_SUCCESS){
		return 1;
	}
	return 0;
}

int32_t USBTerm::cmd_fs_read ( const char* cmd ) {
	uint32_t constexpr buffer_size = 256;
	uint8_t buffer[buffer_size];
	char fname[FLOG_MAX_FNAME_LEN + 1];
	uint32_t to_read, num_read;
	bool err;

	uint32_t len, fname_len;
	parse_string(fname, FLOG_MAX_FNAME_LEN, fname_len, err);
	if(err)
		return 1;
	len = parse_data<uint32_t>(err);

	if(err)
		return 1;

	if(flogfs_open_read(&read_file, fname) != FLOG_SUCCESS){
		return 1;
	}
	if(len == 0) len = -1;
	while(len){
		to_read = min(len, buffer_size);
		num_read = flogfs_read(&read_file, buffer, to_read);
		if(num_read){
			// Sending one buffer element
			write_value<uint32_t>(num_read);
			usbserial1.write_buffer(buffer, num_read);
			len -= num_read;
		}
		if(num_read != to_read){
			// EOF
			break;
		}
	}

	// Close the buffer
	write_value<uint32_t>(0);

	if(flogfs_close_read(&read_file) != FLOG_SUCCESS){
		return 1;
	}
	return 0;
}

int32_t USBTerm::cmd_fs_rm(const char* cmd){
	char fname[FLOG_MAX_FNAME_LEN + 1];
	uint32_t len;
	bool err;
	parse_string(fname, FLOG_MAX_FNAME_LEN, len, err);
	if(flogfs_rm(fname) == FLOG_SUCCESS)
		return 0;
	return 1;
}


int32_t USBTerm::cmd_startstream ( const char* cmd ) {
	return 0;
}

/*
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
}*/


template <typename T>
T USBTerm::parse_data(bool &err) {
	T ret;
	err = (usbserial1.read(reinterpret_cast<uint8_t *>(&ret),
                           sizeof(ret), parse_byte_timeout) != sizeof(T));
	return ret;
}


void USBTerm::parse_string(char * str, uint32_t max_len, uint32_t &len, bool &err) {
	len = 0;
	while(max_len--){
		len += 1;
		err = (usbserial1.read((uint8_t *)str, 1, parse_byte_timeout) != 1);
		if(err) return;
		if(*(str++) == 0)
			return;
	}
}

bool USBTerm::parse_buffer_pkt(uint8_t * dst, uint32_t max_len, uint32_t &len, bool &err) {
	len = parse_data<uint32_t>(err);
	if(len == 0)
		return false;
	if(err) return false;
	len = min(max_len, len);

	err = (usbserial1.read(dst, len, parse_byte_timeout) != len);
	if(err) return false;
	return true;
}

void USBTerm::parse_buffer(uint8_t * dst, uint32_t max_len, uint32_t &len, bool &err) {
	len = 0;
	uint32_t chunk_len;
	while(parse_buffer_pkt(dst, max_len, chunk_len, err)){
		if(err) return;
		max_len -= chunk_len;
		dst += chunk_len;
		len += chunk_len;
	}
}


