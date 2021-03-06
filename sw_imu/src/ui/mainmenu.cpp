#include "ui/mainmenu.h"
#include "ui/sensor_view.h"
#include "ui/usbscreen.h"
#include "ui/rngscreen.h"
#include "ui/testmenu.h"
#include "ui/filemenu.h"
#include "ui/eventlogmenu.h"
#include "ui/sensorlog.h"

MenuItem const MainMenu::items[] = {
	{"Sensor output\n(numeric)", (MenuItem::item_handler)launch_sensor_view_num},
	{"Sensor output\n(bar)", (MenuItem::item_handler)launch_sensor_view_bar},
	{"USB Mode", (MenuItem::item_handler)UI::launch_screen<USBScreen>},
	{"Tests", (MenuItem::item_handler)UI::launch_screen<TestMenu>},
	{"Logger", (MenuItem::item_handler)UI::launch_screen<SensorLogUI>},
	{"Event Viewer", (MenuItem::item_handler)UI::launch_screen<EventMenu>},
	{"Files", (MenuItem::item_handler)UI::launch_screen<FileMenu>},
	{"Random numbers", (MenuItem::item_handler)UI::launch_screen<RNGScreen>},
};

uint32_t const MainMenu::item_count = sizeof(items)/sizeof(*items);

void MainMenu::exec(){
	menu.run(false);
}



void MainMenu::launch_sensor_view_num(){
	SensorView sv(SensorView::DISP_NUMBER);
	sv.exec();
}

void MainMenu::launch_sensor_view_bar(){
	SensorView sv(SensorView::DISP_PROGRESS);
	sv.exec();
}
