#include "ui/mainmenu.h"
#include "ui/sensor_view.h"
#include "ui/usbscreen.h"

MenuItem const MainMenu::items[item_count] = {
	{"Sensor output\n(numeric)", (MenuItem::item_handler)launch_sensor_view_num},
	{"Sensor output\n(bar)", (MenuItem::item_handler)launch_sensor_view_bar},
	{"USB Transfer", (MenuItem::item_handler)launch_usb_mode},
};

void MainMenu::exec(){
	eventmask_t evt;
	
	chEvtGetAndClearEvents(ALL_EVENTS);
	
	menu.draw();
	
	while(!chThdShouldTerminate()){
		evt = chEvtWaitOneTimeout(ALL_EVENTS, MS2ST(5));
		switch(evt){
		case 0:
			continue;
		case UI::MASK_ABORT:
			return;
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
