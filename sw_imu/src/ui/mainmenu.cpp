#include "ui/mainmenu.h"
#include "ui/sensor_view.h"
#include "ui/usbscreen.h"
#include "ui/rngscreen.h"
#include "ui/testmenu.h"
#include "ui/filemenu.h"

MenuItem const MainMenu::items[] = {
	{"Sensor output\n(numeric)", (MenuItem::item_handler)launch_sensor_view_num},
	{"Sensor output\n(bar)", (MenuItem::item_handler)launch_sensor_view_bar},
	{"USB Transfer", (MenuItem::item_handler)launch_usb_mode},
	{"Files", (MenuItem::item_handler)launch_files},
	{"Tests", (MenuItem::item_handler)launch_tests},
	{"Random numbers", (MenuItem::item_handler)launch_rng},
};

uint32_t const MainMenu::item_count = sizeof(items)/sizeof(*items);

void MainMenu::exec(){
	eventmask_t evt;
	
	chEvtGetAndClearEvents(ALL_EVENTS);
	
	menu.draw();
	
	while(!chThdShouldTerminate()){
		evt = chEvtWaitOneTimeout(ALL_EVENTS, MS2ST(5));
		if(!evt) continue;
		
		evt = UI::ui.handle_evt(evt);
		switch(evt){
		case UI::MASK_ABORT:
			break;
		case UI::MASK_SUSPEND:
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

void MainMenu::launch_sensor_view_bar(){
	SensorView sv(SensorView::DISP_PROGRESS);
	sv.exec();
}

void MainMenu::launch_sensor_view_num(){
	SensorView sv(SensorView::DISP_NUMBER);
	sv.exec();
}

void MainMenu::launch_usb_mode(){
	USBScreen us;
	us.exec();
}

void MainMenu::launch_tests(){
	TestMenu tm;
	tm.exec();
}

void MainMenu::launch_files(){
	FileMenu fm;
	fm.exec();
}

void MainMenu::launch_rng(){
	RNGScreen rs;
	rs.exec();
}
