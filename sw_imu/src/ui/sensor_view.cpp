#include "ui/sensor_view.h"

using namespace Platform;

void SensorView::exec(){
	eventmask_t evt;
	
	UI::inhibit_suspend();
	
	oled.fb.clear_area(1);
	
	thread = chThdCreateStatic(SVThread, stack_size, LOWPRIO,(tfunc_t) start_thread, (void *)this);
	
	chEvtGetAndClearEvents(ALL_EVENTS);

	while(!chThdShouldTerminate()){
		evt = chEvtWaitOneTimeout(ALL_EVENTS, MS2ST(50));
		if(!evt) continue;
		
		evt = UI::handle_evt(evt);
		
		switch(evt){
		case 0:
			continue;
		case UI::MASK_BACK:
		case UI::MASK_ABORT:
			terminate();
			return;
		case UI::MASK_LEFT:
			switch(mode){
			case MODE_ACC: mode = MODE_PRS; break;
			case MODE_GYRO: mode = MODE_ACC; break;
			case MODE_MAG: mode = MODE_GYRO; break;
			case MODE_PRS: mode = MODE_MAG; break;
			}
			break;
		case UI::MASK_RIGHT:
			switch(mode){
			case MODE_ACC: mode = MODE_GYRO; break;
			case MODE_GYRO: mode = MODE_MAG; break;
			case MODE_MAG: mode = MODE_PRS; break;
			case MODE_PRS: mode = MODE_ACC; break;
			}
			break;
		case UI::MASK_SELECT:
			terminate();
			return;
		case UI::MASK_SUSPEND:
			evt = chEvtWaitOne(UI::MASK_RESUME);
			chEvtGetAndClearEvents(ALL_EVENTS);
			UI::handle_evt(UI::MASK_RESUME);
			break;
		default:
			break;
		}
	}
	
	terminate();
	
	UI::enable_suspend();
}
	
msg_t SensorView::terminate(){
	chThdTerminate(thread);
	return chThdWait(thread);
}

	
void SensorView::thread_action(){
	char some_string[32];
	
	auto old_mode = MODE_ACC;
	
	acc_listener.init(acc_buffer, buffer_len);
	acc_source.register_queue(acc_listener);
	Acquisition::require_sources(Acquisition::SRC_ACC1);
	
	while(!chThdShouldTerminate()){
		if(mode != old_mode){
			// Clear the old area
			oled.fb.clear_area(1, 3, 80);
			switch(old_mode){
			case MODE_ACC:
				acc_source.unregister_queue(acc_listener);
				Acquisition::norequire_sources(Acquisition::SRC_ACC1);
				break;
			case MODE_MAG:
				mag_source.unregister_queue(mag_listener);
				Acquisition::norequire_sources(Acquisition::SRC_MAG1);
				break;
			case MODE_GYRO:
				gyro_source.unregister_queue(gyro_listener);
				Acquisition::norequire_sources(Acquisition::SRC_GYRO1);
				break;
			case MODE_PRS:
				prs_source.unregister_queue(prs_listener);
				Acquisition::norequire_sources(Acquisition::SRC_PRS1);
				break;
			}
			switch(mode){
			case MODE_ACC:
				Acquisition::require_sources(Acquisition::SRC_ACC1);
				acc_listener.init(acc_buffer, buffer_len);
				acc_source.register_queue(acc_listener);
				break;
			case MODE_MAG:
				Acquisition::require_sources(Acquisition::SRC_MAG1);
				mag_listener.init(mag_buffer, buffer_len);
				mag_source.register_queue(mag_listener);
				break;
			case MODE_GYRO:
				Acquisition::require_sources(Acquisition::SRC_GYRO1);
				gyro_listener.init(gyro_buffer, buffer_len);
				gyro_source.register_queue(gyro_listener);
				break;
			case MODE_PRS:
				Acquisition::require_sources(Acquisition::SRC_PRS1);
				prs_listener.init(prs_buffer, buffer_len);
				prs_source.register_queue(prs_listener);
				break;
			}
			old_mode = mode;
		}

		// Wait for data first before altering the framebuffer
		switch(old_mode){
		case MODE_ACC:
			acc_listener.receive_to(acc_measurement);
			break;
		case MODE_GYRO:
			gyro_listener.receive_to(gyro_measurement);
			break;
		case MODE_MAG:
			if(!mag_listener.receive_to(mag_measurement, MS2ST(100))) continue;
			break;
		case MODE_PRS:
			if(!prs_listener.receive_to(prs_measurement, MS2ST(100))) continue;
			break;
		}

		oled.fb.clear_area(1, 3, 0, 90);

		switch(old_mode){
		case MODE_ACC:
			oled.fb.write_text<Courier3>("ACC:", 1, 0, 90);
			if(display_mode == DISP_PROGRESS) acc_measurement *= 0.4;
			break;
		case MODE_GYRO:
			oled.fb.write_text<Courier3>("GYR:", 1, 0, 90);
			if(display_mode == DISP_PROGRESS) gyro_measurement *= 0.01;
			break;
		case MODE_MAG:
			oled.fb.write_text<Courier3>("MAG:", 1, 0, 90);
			if(display_mode == DISP_PROGRESS) mag_measurement *= 0.01;
			break;
		case MODE_PRS:
			// Pressure is only on one line, unlike others, so just print it all
			oled.fb.write_text<Courier3>("PRS:", 1, 0, 90);
			if(display_mode == DISP_PROGRESS) prs_measurement *= 0.08;
			switch(display_mode){
			case DISP_NUMBER:
				imu_sprint(some_string, prs_measurement);
				oled.fb.write_text<SmallFont>(some_string, 1, 90);
				break;
			case DISP_PROGRESS:
				oled.fb.draw_pos_neg_bar(prs_measurement * 0.01, 1, 80, 127);
				break;
			}
			oled.update();
			continue;
		}
		// We should only get to this point for similar euclidean types
		switch(display_mode){
		case DISP_NUMBER:
			imu_sprint(some_string, acc_measurement.x);
			oled.fb.write_text<SmallFont>(some_string, 1, 90);
			imu_sprint(some_string, acc_measurement.y);
			oled.fb.write_text<SmallFont>(some_string, 2, 90);
			imu_sprint(some_string, acc_measurement.z);
			oled.fb.write_text<SmallFont>(some_string, 3, 90);
			break;
		case DISP_PROGRESS:
			oled.fb.draw_pos_neg_bar(acc_measurement.x, 1, 80, 127);
			oled.fb.draw_pos_neg_bar(acc_measurement.y, 2, 80, 127);
			oled.fb.draw_pos_neg_bar(acc_measurement.z, 3, 80, 127);
			break;
		}
		oled.update();
	}
	
	switch(old_mode){
	case MODE_ACC:
		Acquisition::norequire_sources(Acquisition::SRC_ACC1);
		acc_source.unregister_queue(acc_listener);
		break;
	case MODE_MAG:
		Acquisition::norequire_sources(Acquisition::SRC_MAG1);
		mag_source.unregister_queue(mag_listener);
		break;
	case MODE_GYRO:
		Acquisition::norequire_sources(Acquisition::SRC_GYRO1);
		gyro_source.unregister_queue(gyro_listener);
		break;
	case MODE_PRS:
		Acquisition::norequire_sources(Acquisition::SRC_PRS1);
		prs_source.unregister_queue(prs_listener);
		break;
	}
}
