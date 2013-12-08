#include "ui/sensorlog.h"
#include <acquisition.h>
#include <flogfs.h>
#include <string.h>

using namespace Platform;

template <uint32_t i_bits, uint32_t f_bits>
static void mkformatstr(char * dst, uFractional<i_bits, f_bits> &val){
	imu_sprint(dst, "u", i_bits, ".", f_bits);
}


void SensorEditView::draw(uint32_t margins){
	oled.fb.clear_area(1, 3, margins, oled.fb.num_columns - margins - 1);
	oled.fb.write_text_centered<SmallFont>(name, 1);
	oled.fb.draw_horizontal_mask(3, 0x3F, 58, 1);
	oled.fb.draw_horizontal_mask(3, 0x3F, 69, 1);
	oled.fb.draw_horizontal_mask(2, 0xFC, 58, 1);
	oled.fb.draw_horizontal_mask(2, 0xFC, 69, 1);
	if(enabled){
		oled.fb.draw_horizontal_mask(3, 0b00100000, 59, 1);
		oled.fb.draw_horizontal_mask(3, 0b00101111, 60, 8);
		oled.fb.draw_horizontal_mask(3, 0b00100000, 68, 1);
		oled.fb.draw_horizontal_mask(2, 0b00000100, 59, 1);
		oled.fb.draw_horizontal_mask(2, 0b11110100, 60, 8);
		oled.fb.draw_horizontal_mask(2, 0b00000100, 68, 1);
	} else {
		oled.fb.draw_horizontal_mask(3, 0b00100000, 59, 10);
		oled.fb.draw_horizontal_mask(2, 0b00000100, 59, 10);
	}

	oled.update();
}



void SensorLogUI::draw(){
	oled.fb.clear_area(1);
	if((current_index != 0) && (log_state == LOG_STOPPED)){
		// No arrows while logging, since the buttons don't even respond then
		oled.fb.write_text<MediumFont>("<",1,0);
	}
	if(current_index < num_sensors){
		oled.fb.write_text<MediumFont>(">",1,oled.fb.num_columns - MediumFont::char_width);
	}

	if(current_index == num_sensors){
		switch(log_state){
		case LOG_RUNNING:
			oled.fb.write_text_centered<MediumFont>("RUNNING!", 1);
			break;
		case LOG_STOPPED:
			oled.fb.write_text_centered<MediumFont>("START", 1);
			break;
		}
		oled.update();
	} else {
		sensors[current_index].draw();
	}
}

void SensorLogUI::exec(){
	eventmask_t evt;

	*err_msg = 0;
	oled.fb.clear_area(1);

	draw();

	chEvtGetAndClearEvents(ALL_EVENTS);

	thread = chThdCreateStatic(SLThread, stack_size, NORMALPRIO + 1, (tfunc_t)start_thread, (void *)this);

	while(!chThdShouldTerminate()){
		evt = chEvtWaitOneTimeout(ALL_EVENTS, MS2ST(10));
		// Let higher-priority entities handle it as they please
		evt = UI::handle_evt(evt);
		if(current_index < num_sensors){
			evt = sensors[current_index].handle_event(evt);
		} else if(*err_msg){
			// On "run" screen and error has occurred
			chEvtSignal(thread, ACTION_STOP);
			log_state = LOG_STOPPED;
			draw();
			oled.fb.write_text_centered<SmallFont>(err_msg, 3);
			oled.update();
			*err_msg = 0;
		}
		switch(evt){
		case UI::MASK_BACK:
		case UI::MASK_ABORT:
			terminate();
			return;
		case UI::MASK_RIGHT:
			if((current_index < num_sensors) && (log_state == LOG_STOPPED)){
				current_index += 1;
				draw();
			}
			break;
		case UI::MASK_LEFT:
			if((current_index > 0) && (log_state == LOG_STOPPED)){
				current_index -= 1;
				draw();
			}
			break;
		case UI::MASK_SELECT:
			if(current_index == num_sensors){
				switch(log_state){
					case LOG_STOPPED:
						chEvtSignal(thread, ACTION_START);
						log_state = LOG_RUNNING; // Set here so UI doesn't have to wait
						draw();
						break;
					case LOG_RUNNING:
						chEvtSignal(thread, ACTION_STOP);
						log_state = LOG_STOPPED;
						draw();
						break;
				}
			}
		case UI::MASK_SUSPEND:
			evt = chEvtWaitOne(UI::MASK_RESUME);
			chEvtGetAndClearEvents(ALL_EVENTS);
			UI::handle_evt(UI::MASK_RESUME);
			break;
		default:
			break;
		}
	}
}

msg_t SensorLogUI::terminate(){
	chThdTerminate(thread);
	return chThdWait(thread);
}

sFractional<2,5> a = 1.2;

void SensorLogUI::write_header(){
	char buffer[32];
	char * iter;
	uint8_t rate8;
	// First, get the number of sensors
	uint8_t active_sensors = 0;
	for(uint8_t i = 0; i < num_sensors; i++){
		if(sensors[i].get_enabled())
			active_sensors += 1;
	}
	auto const rate = Acquisition::rate;
	// Write any 8-byte string -- this might have some meaning someday
	flogfs_write(&write_file, (uint8_t*)"POOSTER++", 8);
	// Now print the base sampling rate
	static_assert(sizeof(rate) == 4, "wrong size");
	flogfs_write(&write_file, (uint8_t *)&rate, 4);
	// Now a 1-byte count of sensors that will be described
	flogfs_write(&write_file, &active_sensors, 1);
	// Now to list the sensors
	for(uint8_t i = 0; i < num_sensors; i++){
		if(!sensors[i].get_enabled())
			continue;
		switch(i){
		case 0:
			// Name
			flogfs_write(&write_file, (uint8_t*)"ACC", 4);
			// Rate divider
			rate8 = sensors[i].get_rate();
			flogfs_write(&write_file, &rate8, 1);
			// Fields
			iter = MMA8451Q::format_str(buffer) + 1;
			flogfs_write(&write_file, (uint8_t *)buffer, iter - buffer);
			break;
		case 1:
			// Name
			flogfs_write(&write_file, (uint8_t*)"GYRO", 5);
			// Rate divider
			rate8 = sensors[i].get_rate();
			flogfs_write(&write_file, &rate8, 1);
			// Fields
			iter = decltype(gyro1)::format_str(buffer) + 1;
			flogfs_write(&write_file, (uint8_t *)buffer, iter - buffer);
			break;
		case 2:
			// Name
			flogfs_write(&write_file, (uint8_t*)"MAG", 4);
			// Rate divider
			rate8 = sensors[i].get_rate();
			flogfs_write(&write_file, &rate8, 1);
			// Fields
			iter = MAG3110::format_str(buffer) + 1;
			flogfs_write(&write_file, (uint8_t *)buffer, iter - buffer);
			break;
		case 3:
			// Name
			flogfs_write(&write_file, (uint8_t*)"PRS", 4);
			// Rate divider
			rate8 = sensors[i].get_rate();
			flogfs_write(&write_file, &rate8, 1);
			// Fields
			iter = LPS331::format_str(buffer) + 1;
			flogfs_write(&write_file, (uint8_t *)buffer, iter - buffer);
			break;
		}
	}
}

msg_t SensorLogUI::thread_action(){
	eventmask_t evt;
	char filename[16];
	Euclidean3_f32 sample_eu32;
	float sample_f32;

	decltype(mag_source)::listener_t mag_listener;
	decltype(mag_source)::data_t mag_buffer[2];
	decltype(acc_source)::listener_t acc_listener;
	decltype(acc_source)::data_t acc_buffer[4];
	decltype(gyro_source)::listener_t gyro_listener;
	decltype(gyro_source)::data_t gyro_buffer[4];

	DataListener<float> temp_listener;
	float temp_buffer[2];
	decltype(prs_source)::listener_t prs_listener;
	decltype(prs_source)::data_t prs_buffer[2];

	chRegSetThreadName("LogThread");

	if(flogfs_mount() != FLOG_SUCCESS){
		while(!chThdShouldTerminate()){
			imu_sprint(err_msg, "Can't mount");
			chEvtWaitAnyTimeout(ACTION_START | ACTION_STOP, MS2ST(50));
		}
		return 0;
	}

	while(!chThdShouldTerminate()){
		evt = chEvtWaitAnyTimeout(ACTION_START | ACTION_STOP, MS2ST(50));
		if(evt == ACTION_STOP) continue;
		if(!evt) continue;
		// Otherwise start logging!

		// First, find a filename
		for(uint32_t i = 0; i < 1000; i++){
			imu_sprint(filename, "sensorlog", i);
			if(flogfs_check_exists(filename) == FLOG_FAILURE)
				break; // Found an unused filename
		}
		if(flogfs_open_write(&write_file, filename) == FLOG_FAILURE){
			imu_sprint(err_msg, "Can't open");
			continue;
		}

		write_header();

		uint32_t sensor_enable_mask = 0;

		if(sensors[0].get_enabled()){
			acc_listener.init(acc_buffer, 4);
			acc_source.register_queue(acc_listener);
			sensor_enable_mask |= Acquisition::SRC_ACC1;
		}
		if(sensors[1].get_enabled()){
			gyro_listener.init(gyro_buffer, 4);
			gyro_source.register_queue(gyro_listener);
			sensor_enable_mask |= Acquisition::SRC_GYRO1;
		}
		if(sensors[2].get_enabled()){
			mag_listener.init(mag_buffer, 2);
			mag_source.register_queue(mag_listener);
			sensor_enable_mask |= Acquisition::SRC_MAG1;
		}
		if(sensors[3].get_enabled()){
			prs_listener.init(prs_buffer, 2);
			prs_source.register_queue(prs_listener);
			sensor_enable_mask |= Acquisition::SRC_PRS1;
		}
		if(sensors[4].get_enabled()){
		//	temp_listener.init(temp_buffer, 2);
		//	temp_source.register_queue(temp_listener);
		//	sensor_enable_mask |= Acquisition::SRC_PRS1;
		}

		Acquisition::require_sources(sensor_enable_mask);

		// Write a header? HELL NO!

		uint32_t t = 0;
		while(!chThdShouldTerminate()){
			if(chEvtGetAndClearEvents(ALL_EVENTS) & ACTION_STOP){
				break;
			}
			if(sensors[0].get_enabled() &&
			   (t & (sensors[0].get_rate() - 1)) == 0){
				acc_listener.receive_to(sample_eu32, MS2ST(30));
				flogfs_write(&write_file, (uint8_t *)&sample_eu32, sizeof(sample_eu32));
			}
			if(sensors[1].get_enabled() &&
			   (t & (sensors[1].get_rate() - 1)) == 0){
				gyro_listener.receive_to(sample_eu32, MS2ST(30));
				flogfs_write(&write_file, (uint8_t *)&sample_eu32, sizeof(sample_eu32));
			}
			if(sensors[2].get_enabled() &&
			   (t & (sensors[2].get_rate() - 1)) == 0){
				mag_listener.receive_to(sample_eu32, MS2ST(30));
				flogfs_write(&write_file, (uint8_t *)&sample_eu32, sizeof(sample_eu32));
			}
			if(sensors[3].get_enabled() &&
			   (t & (sensors[3].get_rate() - 1)) == 0){
				prs_listener.receive_to(sample_f32, MS2ST(30));
				flogfs_write(&write_file, (uint8_t *)&sample_f32, sizeof(sample_f32));
			}
// 			if(sensors[4].get_enabled() &&
//			   (t & (sensors[4].get_rate() - 1)) == 0){
// 				temp_listener.receive_to(sample_f32, MS2ST(20));
// 				flogfs_write(&write_file, (uint8_t *)&sample_f32, sizeof(sample_f32));
// 			}
			t += 1;
		}
		flogfs_close_write(&write_file);
		Acquisition::norequire_sources(sensor_enable_mask);

		if(sensors[0].get_enabled()){
			acc_source.unregister_queue(acc_listener);
		}
		if(sensors[1].get_enabled()){
			gyro_source.unregister_queue(gyro_listener);
		}
		if(sensors[2].get_enabled()){
			mag_source.unregister_queue(mag_listener);
		}
		if(sensors[3].get_enabled()){
			prs_source.unregister_queue(prs_listener);
		}
		if(sensors[4].get_enabled()){
		//	temp_source.unregister_queue(temp_listener);
		}
	}
	return 0;
}
