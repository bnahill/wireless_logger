#include "ui/eventlogmenu.h"
#include "platform.h"

using namespace Platform;

typedef enum {
	POS_ITEMS = 0,
	POS_CLEAR_ITEMS,
	POS_EXIT
} position_t;

static void display_position(EventItem * item, position_t position){
	char const * severity = "";
	oled.fb.clear_area(1);
	switch(position){
	case POS_ITEMS:
		if(!item){
			oled.fb.write_text_centered<Courier3>("None", 1, oled.fb.num_columns/2, oled.fb.num_columns);
		} else {
			switch(item->severity){
			case EventItem::SEVERITY_NOTE:
				severity = "Note";
				break;
			case EventItem::SEVERITY_WARNING:
				severity = "Warning";
				break;
			case EventItem::SEVERITY_ERROR:
				severity = "Error";
				break;
			}
			oled.fb.write_text_centered<SmallFont>(severity, 1, oled.fb.num_columns/2, oled.fb.num_columns);
			oled.fb.write_text_centered<SmallFont>(item->message, 2, oled.fb.num_columns/2, oled.fb.num_columns);
		}
		break;
	case POS_CLEAR_ITEMS:
		oled.fb.write_text_centered<Courier3>("Clear", 1, oled.fb.num_columns/2,oled.fb.num_columns);
		break;
	case POS_EXIT:
		oled.fb.write_text_centered<Courier3>("Exit", 1, oled.fb.num_columns/2,oled.fb.num_columns);
		break;
	}
	oled.update();
}

void EventMenu::exec() {
	EventItem * item;
	EventItem * tmp_item;
	position_t position;
	eventmask_t evt;
	uint32_t exit_count;
	
	chEvtGetAndClearEvents(ALL_EVENTS);

	item = evt_log.items_head;
	position = POS_ITEMS;
	
	display_position(item, position);
	
	exit_count = 0;
	while(!chThdShouldTerminate()){
		evt = chEvtWaitOneTimeout(ALL_EVENTS, MS2ST(20));
		evt = UI::handle_evt(evt);
		
		switch(evt){
		case 0:
			if(button[1].is_pressed()){
				if(++exit_count > 50){
					UI::exit_current();
					return;
				}
			} else {
				exit_count = 0;
			}
			continue;
		case UI::MASK_ABORT:
			return;
		case UI::MASK_SELECT:
			switch(position){
			case POS_ITEMS:
				break;
			case POS_CLEAR_ITEMS:
				// Check in case new items were added
				evt_log.clear();
				position = POS_ITEMS;
				item = nullptr;
				display_position(nullptr, POS_ITEMS);
				break;
			case POS_EXIT:
				UI::exit_current();
				break;
			}
			break;
		case UI::MASK_LEFT:
			switch(position){
			case POS_ITEMS:
				tmp_item = evt_log.previous(item);
				if((!tmp_item) && item){
					// Beginning of list
				} else {
					item = tmp_item;
				}
				break;
			case POS_CLEAR_ITEMS:
				// Check in case new items were added
				if(item){
					for(;evt_log.next(item); item = evt_log.next(item));
				}
				position = POS_ITEMS;
				break;
			case POS_EXIT:
				if(item)
					position = POS_CLEAR_ITEMS;
				else
					position = POS_ITEMS;
				break;
			}
			display_position(item, position);
			break;
		case UI::MASK_RIGHT:
			switch(position){
			case POS_ITEMS:
				if(!item){
					// If empty, skip "clear"
					position = POS_EXIT;
					break;
				}
				tmp_item = evt_log.next(item);
				if(!tmp_item && item){
					// Just hit end
					position = POS_CLEAR_ITEMS;		
				} else {
					item = tmp_item;
				}
				break;
			case POS_CLEAR_ITEMS:
				position = POS_EXIT;
				break;
			case POS_EXIT:
				break;
			}
			display_position(item, position);
			break;
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


void EventMenu::exit() {

}
