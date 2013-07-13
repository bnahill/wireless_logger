#ifndef __EVENTLOG_H_
#define __EVENTLOG_H_

#include "imu.h"

class EventLog;

class EventItem {
public:
	typedef enum {
		SEVERITY_NOTE,
		SEVERITY_WARNING,
		SEVERITY_ERROR,
	} severity_t;
	EventItem(){};
	EventItem(char const * str, severity_t severity) :
		message(str),
		severity(severity)
	{}
	
	char const * message;
	severity_t severity;
	
	static void * operator new (size_t size, EventLog &l);
protected:
	//! Next element in the linked list
	EventItem * next;
	
	friend class EventLog;
};

class EventLog {	
public:
	EventLog() :
		items_head(nullptr)
	{
		
	}
	
	static constexpr uint32_t max_string_length = 32;
	static constexpr uint32_t max_strings = 64;
	static constexpr uint32_t max_items = 128;

	void init(){
		chPoolInit(&string_pool, max_string_length, nullptr);
		chPoolInit(&item_pool, sizeof(EventItem), nullptr);
		chPoolLoadArray(&string_pool, (void *)string_memory, max_strings);
		chPoolLoadArray(&item_pool, (void *)item_memory, max_items);
	}
	
	void clear(){
		while(items_head){
			remove_item(items_head);
		}
	}
	
	EventItem * next(EventItem * item) const {
		if(item)
			return item->next;
		return items_head;
	}
	
	EventItem * previous(EventItem * item) const {
		EventItem * tmp;
		if(item == items_head){
			// Don't go there
			return nullptr;
		}
		if(item && items_head){
			for(tmp = items_head; tmp->next != item; tmp = tmp->next);
			return tmp;
		}
		return items_head;
	}
	
	EventItem * add(char const * str, EventItem::severity_t severity){
		EventItem * item = new(*this) EventItem(str, severity);
		add_item(item);
		return item;
	}
	
	EventItem * allocate(){
		return reinterpret_cast<EventItem *>(chPoolAlloc(&item_pool));
	}
	
	char * allocate_string(){
		return reinterpret_cast<char *>(chPoolAlloc(&string_pool));
	}
	
	void remove_item(EventItem * item){
		EventItem * tmp;
		chSysLock();
		
		if(items_head == item)
			items_head = item->next;
		else {
			// Iterate until the last item
			for(tmp = items_head; tmp->next && (tmp->next != item); tmp = tmp->next);
			tmp->next = item->next;
		}
		
		chSysUnlock();
		// Check for allocated string. If so, free it.
		if((item->message >= (char *)string_memory) &&
		   (item->message < &((char *)string_memory)[sizeof(string_memory)])){
			// Allocated memory
			chPoolFree(&string_pool, (void *)item->message);
		}
		
		chPoolFree(&item_pool, (void *)item);

	}
	
	EventItem * items_head;

protected:
	MemoryPool string_pool;
	char string_memory[max_strings][max_string_length];
	
	MemoryPool item_pool;
	EventItem item_memory[max_items];
	
	void add_item(EventItem * item){
		EventItem * tmp;
		if(!item)
			return;
		chSysLock();
		
		if(items_head == nullptr)
			items_head = item;
		else {
			// Iterate until the last item
			for(tmp = items_head; tmp->next; tmp = tmp->next);
			tmp->next = item;
		}
		
		chSysUnlock();
	}
};

#endif // __EVENTLOG_H_
