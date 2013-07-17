#include "ui/testmenu.h"
#include "ui/tests.h"
//#include "ui/footstepcount.h"

MenuItem const TestMenu::items[] = {
	{"FLog Format", (MenuItem::item_handler)Tests::fs_format},
	{"FLog Mount", (MenuItem::item_handler)Tests::fs_mount},
	{"Flash Erase", (MenuItem::item_handler)Tests::flash_erase},
	//{"FS RW Test", (MenuItem::item_handler)Tests::flash_create_test_file},
	{"Flash Bad Blocks", (MenuItem::item_handler)Tests::flash_bad_block_check},
	{"Flash RW Test\n(will corrupt mem)", (MenuItem::item_handler)Tests::flash_test},
	{"Flash ECC Test\n(will corrupt mem)", (MenuItem::item_handler)Tests::flash_ecc_flexibility_test},
//	{"Count Steps", (MenuItem::item_handler)UI::launch_screen<FootstepCount>},
	{"Exit", (MenuItem::item_handler)TestMenu::exit},
};

uint32_t const TestMenu::item_count = sizeof(items)/sizeof(*items);

void TestMenu::exec(){
	eventmask_t evt;
	bool get_out = false;
	
	chEvtGetAndClearEvents(ALL_EVENTS);
	
	menu.draw();
	
	while(!get_out){
		evt = chEvtWaitOneTimeout(ALL_EVENTS, MS2ST(5));
		if(!evt) continue;
		
		evt = UI::ui.handle_evt(evt);
		switch(evt){
		case UI::MASK_ABORT:
			get_out = true;
			break;
		case UI::MASK_SUSPEND:
			
			// BLOCK HERE
			evt = chEvtWaitOne(UI::MASK_RESUME);
			chEvtGetAndClearEvents(ALL_EVENTS);
			UI::ui.handle_evt(UI::MASK_RESUME);
			break;
		case UI::MASK_RESUME:
			break;
		default:
			menu.handle(evt);
		}
	}
}

void TestMenu::exit(){
	chEvtSignal(UI::ui.thread, UI::MASK_ABORT);
}

