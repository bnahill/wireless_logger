#include "ui/filemenu.h"
#include "coffee/cfs-coffee.h"
#include "string.h"

MenuItem const FileMenu::items[] = {
	//{"File List", static_cast<MenuItem::item_handler>(launch_file_list)},
	//{"Flash Format", static_cast<MenuItem::item_handler>(launch_flash_format)},
	{"Exit", static_cast<MenuItem::item_handler>(UI::exit_current)},
};

uint32_t const FileMenu::item_count = sizeof(items)/sizeof(*items);

using namespace Platform;

void FileMenu::exec(){
	menu.run();
}

void FileMenu::launch_flash_format(){
	
}

void FileMenu::launch_file_list(){
// 	constexpr uint32_t max_items = 32;
// 	struct cfs_dir d;
// 	struct cfs_dirent dirent;
// 	MenuItem items[max_items];
// 	char menu_strings[16][max_items];
// 	uint32_t item_count;
// 	if(cfs_opendir(&d, "") == -1){
// 		// Error!
// 		oled.fb.write_text<SmallFont>("Error opening directory", 1, 0);
// 		oled.update();
// 		UI::wait_for_button(UI::MASK_SELECT);
// 		return;
// 	}
// 	for(item_count = 0; item_count < max_items - 1; item_count++){
// 		if(cfs_readdir(&d, &dirent) == -1)
// 			break;
// 		strncpy(&menu_strings[item_count][0], dirent.name, 16);
// 		menu_strings[item_count][15] = '\0';
// 		items[item_count] = MenuItem(&menu_strings[item_count][0], nullptr);
// 	}
// 	
// 	items[item_count] = MenuItem("Exit", UI::exit_current);
// 	
// 	Menu m(items, item_count + 1);
// 	
// 	cfs_closedir(&d);
// 	
// 	m.run();
}


