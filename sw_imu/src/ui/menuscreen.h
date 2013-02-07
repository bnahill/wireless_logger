#ifndef __MENUSCREEN_H_
#define __MENUSCREEN_H_

class MenuScreen;

class MenuItem {
public:
	typedef void (*item_handler)(uint32_t *);
	MenuItem(char const * label, item_handler handler) :
		handler(handler), label(label), prev(nullptr), next(nullptr)
	{}
	void exec(){
		handler(nullptr);
	}
protected:
	item_handler handler;
	char const * label;
	MenuItem *prev, *next;
	friend class MenuScreen;
	friend class MenuItemList;
};

class MenuItemList {
public:
	MenuItemList(MenuItem * array, uint32_t num) :
		item(array)
	{
		uint_fast8_t i;
		array[0].prev = nullptr;
		for(i = 1; i < num; i++){
			array[i].prev = &array[i - 1];
			array[i - 1].next = &array[i - 1];
		}
		array[num - 1].next = nullptr;
	}
	
	MenuItem * item;
};

class MenuScreen : public Screen {
public:
	MenuScreen(Screen * parent) :
		Screen(parent)
	{
		chSemInit(&item_lock, 1);
	}
	
	void show(){
		button[0].clear_callbacks();
		button[1].clear_callbacks();
		button[2].clear_callbacks();
		

		button[0].set_press_handler((Button::button_handler)go_right_callback,
								(uint32_t *)this);
		button[1].set_press_handler((Button::button_handler)select_callback,
								(uint32_t *)this);
		button[2].set_press_handler((Button::button_handler)go_left_callback,
								(uint32_t *)this);

		oled.fb.clear();
		
		oled.update();
	}
	
private:
	MenuItemList items;

	Semaphore item_lock;
	void lock(){chSemWait(&item_lock);}
	void unlock(){chSemSignal(&item_lock);}
	
	void do_select_callback(){
		lock();
		if(items.item){
			items.item->exec();
		}
		unlock();
	}
	
	void do_go_right_callback(){
		lock();
		if(items.item && items.item->next){
			items.item = items.item->next;
		}
		unlock();
	}
	
	void do_go_left_callback(){
		lock();
		if(items.item && items.item->prev){
			items.item = items.item->prev;
		}
		unlock();
	}
	
	static void select_callback(MenuScreen *ptr){
		ptr->do_select_callback();
	}
	
	static void go_right_callback(MenuScreen *ptr){
		ptr->do_go_right_callback();
	}
	
	static void go_left_callback(MenuScreen *ptr){
		ptr->do_go_left_callback();
	}
};

#endif // __MENUSCREEN_H_
