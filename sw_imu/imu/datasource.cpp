#include <imu/datasource.h>




template < class datatype >
void DataListener< datatype >::init(datatype * listener_buf, uint32_t max_len){
	len = max_len;
	chSemInit(&sem, 0);
	buf_head = listener_buf;
	rd_head = buf_head;
	wr_head = buf_head;
}

template < class datatype >
datatype * DataListener< datatype >::receive(uint32_t timeout_ms){
	datatype * dst;
	chSysLock();
	if(RDY_OK == chSemWaitTimeoutS(&sem, timeout_ms)){
		dst = rd_head;
		if(rd_head == buf_head + len - 1){
			rd_head = buf_head;
		} else {
			rd_head += 1;
		}
		chSysUnlock();
		return dst;
	}

	chSysUnlock();
	return nullptr;
}

template < class datatype >
datatype * DataListener< datatype >::peekI(uint32_t timeout_ms){
	datatype * dst;
	if(RDY_OK == chSemWaitTimeoutS(&sem, timeout_ms)){
		chSemSignalI(&sem);
		dst = rd_head;
		return dst;
	}
	return nullptr;
}

template < class datatype >
bool DataListener< datatype >::receive_to(datatype & dst, uint32_t timeout_ms){
	chSysLock();
	if(RDY_OK == chSemWaitTimeoutS(&sem, timeout_ms)){
		dst = *rd_head;
		if(rd_head == buf_head + len - 1){
			rd_head = buf_head;
		} else {
			rd_head += 1;
		}
		chSysUnlock();
		return true;
	}
	
	chSysUnlock();
	return false;
}

template < class datatype >
bool DataListener< datatype >::pushI(datatype & src){
	if(chSemGetCounterI(&sem) + 1 < len + 1){
		chSemSignalI(&sem);
		*wr_head = src;
		if(wr_head == buf_head + len - 1){
			wr_head = buf_head;
		} else {
			wr_head += 1;
		}
		return true;
	}
	return false;
}













template < class datatype >
void DataSource< datatype >::register_queue(DataListener<datatype> &listener){
	chSysLock();
	listener.next = nullptr;
	if(listener_head == nullptr){
		listener_head = &listener;
		chSysUnlock();
		return;
	}
	
	DataListener<datatype> * iter = listener_head;
	while(iter->next) iter = iter->next;
	iter->next = &listener;
	
	chSysUnlock();
	
}

template < class datatype >
void DataSource< datatype >::unregister_queue(DataListener<datatype> &listener){
	chSysLock();
	// If it's the only item here!
	if(listener_head == &listener){
		listener_head = listener.next;
		listener.resetI();
		chSysUnlock();
		return;
	}
	
	DataListener<datatype> * iter = listener_head;
	
	while(iter->next){
		if(iter->next == &listener){
			iter->next = listener.next;
			chSysUnlock();
			return;
		}
		
		iter = iter->next;
	}
	chSysUnlock();
}

template < class datatype >
void DataSource< datatype >::put(datatype &src, bool reschedule){
	DataListener<datatype> * iter;
	
	chSysLock();
	
	for(iter = listener_head; iter; iter = iter->next){
		iter->pushI(src);
	}
	if(reschedule && listener_head){
		// Reschedule
		chSchRescheduleS();
	}
	chSysUnlock();
}