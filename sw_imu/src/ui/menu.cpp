#include "ui/menu.h"

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