#include "eventlog.h"

void * EventItem::operator new (size_t size, EventLog &l){
	return (void *)l.allocate();
}
