#ifndef __UI_MENU_H_
#define __UI_MENU_H_

#include "platform.h"
#include "ui.h"

class MenuItem {
public:
	typedef void (*item_handler)();
	MenuItem(char const * label, item_handler handler) :
		handler(handler), label(label)
	{}
	void exec() const {
		if(handler)
			handler();
	}
	char const * get_label() const {return label;}
protected:
	item_handler handler;
	char const * label;
};

class Menu {
public:
	Menu(MenuItem const * item_list, uint32_t num_items) :
		item_list(item_list), num_items(num_items), item_index(0)
	{}
	
	bool handle(eventmask_t evt);
	
	void draw(bool update=true);
	
protected:	
	MenuItem const * const item_list;
	uint32_t const num_items;
	
	uint32_t item_index;
	
	
};

#endif // __UI_MENU_H_
