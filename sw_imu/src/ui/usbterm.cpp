#include "usbterm.h"

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
	{"listcmds", "[s]()", (ShellCommand::shell_callback_t)&USBTerm::cmd_listcmds},
	{"help", "s()", (ShellCommand::shell_callback_t)&USBTerm::cmd_help},
	{"settime", "(datetime:date)", (ShellCommand::shell_callback_t)&USBTerm::cmd_settime},
	{"ping", "s(s:ping)", (ShellCommand::shell_callback_t)&USBTerm::cmd_ping},
	{"listbuffers", "[s:name,u:length,s:format]()", (ShellCommand::shell_callback_t)&USBTerm::cmd_listbuffers},
	{"fetchbuffer", "u,logbuffer(s:buffer_name)", (ShellCommand::shell_callback_t)&USBTerm::cmd_fetchbuffer}
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
	parse_string(cmd);
	chprintf(usbserial1.stream(), parse_string(cmd));
	usbserial1.write_byte(0);
	return 0;
}

int32_t USBTerm::cmd_listcmds(const char* cmd){
	uint_fast8_t i;
	write_value< uint32_t >(num_commands);
	for(i = 0; i < num_commands; i++){
		chprintf(usbserial1.stream(), "%s %s", commands[i].get_root(), commands[i].get_args());
		usbserial1.write_byte(0);
	}
	return 0;
}

int32_t USBTerm::cmd_listbuffers(const char* cmd){
	return 0;
}

int32_t USBTerm::cmd_fetchbuffer(const char* cmd){
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



