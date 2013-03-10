#include "ui/sensor_view.h"

using namespace Platform;

void SensorView::exec(){
	eventmask_t evt;
	
	oled.fb.clear_area(1);
	
	thread = chThdCreateStatic(SVThread, stack_size, LOWPRIO,(tfunc_t) start_thread, (void *)this);
	
	chEvtGetAndClearEvents(ALL_EVENTS);

	while(!chThdShouldTerminate()){
		evt = chEvtWaitOneTimeout(ALL_EVENTS, MS2ST(5));
		if(!evt) continue;
		
		evt = UI::ui.handle_evt(evt);
		
		switch(evt){
		case 0:
			continue;
		case UI::MASK_ABORT:
			terminate();
			return;
		case UI::MASK_LEFT:
			switch(mode){
			case MODE_ACC: mode = MODE_MAG; break;
			case MODE_GYRO: mode = MODE_ACC; break;
			case MODE_MAG: mode = MODE_GYRO; break;
			}
			break;
		case UI::MASK_RIGHT:
			switch(mode){
			case MODE_ACC: mode = MODE_GYRO; break;
			case MODE_GYRO: mode = MODE_MAG; break;
			case MODE_MAG: mode = MODE_ACC; break;
			}
			break;
		case UI::MASK_SELECT:
			terminate();
			return;
		case UI::MASK_SUSPEND:
			evt = chEvtWaitOne(UI::MASK_RESUME);
			chEvtGetAndClearEvents(ALL_EVENTS);
			UI::ui.handle_evt(UI::MASK_RESUME);
			break;
		default:
			break;
		}
	}
	
	terminate();
}
	
msg_t SensorView::terminate(){
	chThdTerminate(thread);
	return chThdWait(thread);
}

	
void SensorView::thread_action(){
	Euclidean3_f32 measurement;
	EventListener listener;
	char some_string[32];
	
	chEvtRegisterMask(&Acquisition::tick_source, &listener, 1);
	
	while(!chThdShouldTerminate()){
		chEvtWaitOne(1);
		
		oled.fb.clear_area(1, 3, 0, 90);
		switch(mode){
		case MODE_ACC:
			oled.fb.write_text<Courier3>("ACC:", 1, 0, 90);
			acc1.get_reading(measurement);
			if(display_mode == DISP_PROGRESS) measurement *= 0.2;
			break;
		case MODE_GYRO:
			oled.fb.write_text<Courier3>("GYR:", 1, 0, 90);
			gyro1.get_reading(measurement);
			if(display_mode == DISP_PROGRESS) measurement *= 0.01;
			break;
		case MODE_MAG:
			oled.fb.write_text<Courier3>("MAG:", 1, 0, 90);
			mag1.get_reading(measurement);
			if(display_mode == DISP_PROGRESS) measurement *= 0.01;
			break;
		}
		switch(display_mode){
		case DISP_NUMBER:
			float_to_string(measurement.x, some_string);
			oled.fb.write_text<SmallFont>(some_string, 1, 90);
			float_to_string(measurement.y, some_string);
			oled.fb.write_text<SmallFont>(some_string, 2, 90);
			float_to_string(measurement.z, some_string);
			oled.fb.write_text<SmallFont>(some_string, 3, 90);
			break;
		case DISP_PROGRESS:
			oled.fb.draw_pos_neg_bar(measurement.x, 1, 80, 127);
			oled.fb.draw_pos_neg_bar(measurement.y, 2, 80, 127);
			oled.fb.draw_pos_neg_bar(measurement.z, 3, 80, 127);
			break;
		}
		oled.update();
	}
	
	chEvtUnregister(&Acquisition::tick_source, &listener);
}