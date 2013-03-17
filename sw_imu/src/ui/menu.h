#ifndef __UI_MENU_H_
#define __UI_MENU_H_

#include "platform.h"
#include "ui.h"

//! @addtogroup UI
//! @{

/*!
 @brief A single menu entry
 
 It contains a pointer to a label and an event handler function to be called
 upon selection of this item. This function will be called from the main UI
 thread context.
 */
class MenuItem {
public:
	//! A callback function for a given item
	typedef void (*item_handler)();
	
	MenuItem(){}
	
	MenuItem(char const * label, item_handler handler) :
		handler(handler), label(label)
	{}
	
	char const * get_label() const {return label;}
protected:
	item_handler handler;
	char const * label;
	
	void exec() const {
		if(handler)
			handler();
	}
	
	friend class Menu;
};

/*!
 @brief A basic menu driver for a list of items in contiguous memory
 */
class Menu {
public:
	Menu(MenuItem const * item_list, uint32_t num_items) :
		item_list(item_list), num_items(num_items), item_index(0)
	{}
	
	/*!
	 @brief Handle an event mask from the UI
	 @param evt The recently arrived event
	 @return Whether or not anything was done
	 
	 If there is something to be done by the menu, the appropriate action will
	 be performed
	 */
	bool handle(eventmask_t evt);
	
	/*!
	 @brief Update the framebuffer with the current menu state
	 @param update Whether or not the panel should be written
	 */
	void draw(bool update=true);
	
	/*!
	 @brief Run the menu in a very simple loop for "normal" behavior
	 */
	void run();
	
protected:	
	//! A list of menu items
	MenuItem const * const item_list;
	//! The number of items in the list
	uint32_t const num_items;
	
	//! The current menu selection
	uint32_t item_index;
};

//! @}

#endif // __UI_MENU_H_
