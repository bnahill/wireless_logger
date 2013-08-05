#include "ui/menu.h"

using namespace Platform;

void Menu::draw(bool update){
	uint32_t i;
	char row_buffer[32];
	char const * const str = item_list[item_index].get_label();
	char * line_end,  * line_start;
	uint32_t line_count = 1;
	// Erase all but top row
	oled.fb.clear_area(1);
	
	// Copy the string
	for(i = 0; str[i] != 0; i++){
		row_buffer[i] = str[i];
		if(str[i] == '\n')
			line_count++;
	}
	row_buffer[i] = 0;
	
	line_end = row_buffer;
	for(i = 1; i < line_count + 1; i++){
		line_start = line_end;
		for(; (*line_end != '\n') && (*line_end != 0); line_end++);
		*line_end = 0;
		oled.fb.write_text_centered<SmallFont>(
			line_start, i,
			oled.fb.num_columns / 2,
			oled.fb.num_columns - 30
		);
		line_end++;
	}
	if(item_index != 0){
		oled.fb.write_text<MediumFont>("<",1,0);
	}
	if(item_index < num_items - 1){
		oled.fb.write_text<MediumFont>(">",1,oled.fb.num_columns - MediumFont::char_width);
	}
	if(update)
		oled.update();
}


bool Menu::handle(eventmask_t evt){
	switch(evt){
	case UI::MASK_LEFT:
		item_index = max((int32_t)0, item_index - 1);
		draw();
		return true;
	case UI::MASK_RIGHT:
		item_index = min(num_items - 1, item_index + 1);
		draw();
		return true;
	case UI::MASK_SELECT:
		if(item_index < num_items){
			item_list[item_index].exec();
			// Redraw on return
			draw();
		}
		return true;
	default:
		return false;
	}
}

void Menu::run(bool enable_exit){
	bool get_out = false;
	eventmask_t evt;
	draw();
	
	chEvtGetAndClearEvents(UI::MASK_BUTTONS);
	
	while(!get_out){
		//evt = chEvtWaitOneTimeout(ALL_EVENTS, MS2ST(50));
		evt = chEvtWaitOne(ALL_EVENTS);
		if(!evt) continue;
		
		evt = UI::handle_evt(evt);
		switch(evt){
		case UI::MASK_ABORT:
			get_out = enable_exit;
			break;
		case UI::MASK_SUSPEND:
			
			// BLOCK HERE
			evt = chEvtWaitOne(UI::MASK_RESUME);
			chEvtGetAndClearEvents(ALL_EVENTS);
			UI::handle_evt(UI::MASK_RESUME);
			break;
		case UI::MASK_RESUME:
			break;
		default:
			handle(evt);
		}
	}
}
