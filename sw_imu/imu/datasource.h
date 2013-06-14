#ifndef __IMU_DATASOURCE_H_
#define __IMU_DATASOURCE_H_

#include "ch.h"

template <class datatype>
class DataSource;

template <class datatype>
class DataListener {
public:
	void init(datatype * listener_buf, uint32_t max_len){
		len = max_len;
		chSemInit(&sem, 0);
		rd_head = buf_head;
		wr_head = buf_head;
	}
	
	datatype * receive(uint32_t timeout_ms = TIME_INFINITE){
		datatype * dst;
		if(RDY_OK == chSemWaitTimeout(&sem, timeout_ms)){
			dst = rd_head;
			if(rd_head == buf_head + len - 1){
				rd_head = buf_head;
			} else {
				rd_head += 1;
			}
			return dst;
		}

		return nullptr;
	}
	
	bool receive_to(datatype & dst, uint32_t timeout_ms = TIME_INFINITE){
		if(RDY_OK == chSemWaitTimeout(&sem, timeout_ms)){
			dst = *rd_head;
			if(rd_head == buf_head + len - 1){
				rd_head = buf_head;
			} else {
				rd_head += 1;
			}
			return true;
		}

		return false;
	}
	
	bool push(datatype & src){
		if(chSemGetCounterI(&sem) < len){
			chSemFastSignalI(&sem);
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

protected:
	Semaphore sem;
	
	DataListener * next;
	
	datatype * buf_head;
	datatype * rd_head, * wr_head;
	uint32_t len;
	
	void reset(){
		chSemReset(&sem, 0);
		rd_head = buf_head;
		wr_head = buf_head;
	}
	
	friend class DataSource<datatype>;
};

template <class datatype>
class DataSource {
public:
	DataSource() :
		listener_head(nullptr)
	{}
	
	void register_queue(DataListener<datatype> &listener){
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
	
	void unregister_queue(DataListener<datatype> &listener){
		chSysLock();
		// If it's the only item here!
		if(listener_head == &listener){
			listener_head = listener.next;
			listener.reset();
			chSysUnlock();
			return;
		}
		
		DataListener<datatype> * iter = listener_head;
		
		chSysLock();
		while(iter){
			if(iter->next == &listener){
				iter->next = listener.next;
				chSysUnlock();
				return;
			}
			
			iter = iter->next;
		}
		chSysUnlock();
	}
	
	bool has_listeners(){return listener_head;}

	void put(datatype &src, bool reschedule = true){
		DataListener<datatype> * iter;
		
		chSysLock();
		
		for(iter = listener_head; iter; iter = iter->next){
			iter->push(src);
		}
		if(reschedule && listener_head){
			// Reschedule
			chSchRescheduleS();
		}
		chSysUnlock();
	}
protected:
	
	DataListener<datatype> * listener_head;
};

#endif // __IMU_DATASOURCE_H_
