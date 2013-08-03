#include "ui/rngscreen.h"
#include "platform.h"

using namespace Platform;

RNGScreen::RNGScreen() :
	num_choices(1)
{
	
}

void RNGScreen::print_number(uint32_t num, char * where){
	uint32_t pos = 10000000;
	uint32_t div;
	if(0 == num){
		*where = '0';
		*(where+1) = 0;
		return;
	}
	while(pos > num){
		pos /= 10;
	}
	while(pos){
		div = num / pos;
		*(where++) = div + '0';
		num -= div * pos;
		pos /= 10;
	}
	*where = 0;
}

void RNGScreen::exec(){
	eventmask_t evt;
	char line1[32] = "# choices: 1";
	char line2[32] = "";
	uint32_t exit_count = 0;
	uint32_t value;
	oled.fb.clear_area(1);
	
	oled.fb.write_text_centered<SmallFont>(line1, 1,
	                                       oled.fb.num_columns/2,
	                                       oled.fb.num_columns);
	oled.update();

	// Launch the USB terminal
	Random::init();
	
	chEvtGetAndClearEvents(ALL_EVENTS);

	while(!chThdShouldTerminate()){
		evt = chEvtWaitOneTimeout(ALL_EVENTS, MS2ST(5));
		evt = UI::ui.handle_evt(evt);
		
		switch(evt){
		case 0:
			if(Platform::button[1].is_pressed()){
				if(++exit_count > 100){
					terminate();
					return;
				}
			} else {
				exit_count = 0;
			}
			continue;
		case UI::MASK_ABORT:
			terminate();
			return;
		case UI::MASK_SELECT:
			oled.fb.clear_area(2);
			oled.update();
			chThdSleep(MS2ST(500));
			if(Platform::button[1].is_pressed()){
				oled.update();
				break;
			}
			value = (Random::get_value() % num_choices) + 1;
			print_number(value, line2);
			
			oled.fb.write_text_centered<MediumFont>(line2, 2, oled.fb.num_columns/2, oled.fb.num_columns);
			oled.update();
			break;
		case UI::MASK_LEFT:
			num_choices = max(num_choices - 1, 1);
			print_number(num_choices, line1 + 11);
			oled.fb.write_text_centered<SmallFont>(line1, 1,
	                                       oled.fb.num_columns/2,
	                                       oled.fb.num_columns);
			oled.fb.clear_area(2);
			oled.update();
			break;
		case UI::MASK_RIGHT:
			num_choices += 1;
			print_number(num_choices, line1 + 11);
			oled.fb.write_text_centered<SmallFont>(line1, 1,
	                                       oled.fb.num_columns/2,
	                                       oled.fb.num_columns);
			oled.fb.clear_area(2);
			oled.update();
			break;
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

msg_t RNGScreen::terminate(){
	Random::close();
	return 0;
}
