#ifndef __IMU_DATASOURCE_H_
#define __IMU_DATASOURCE_H_

#include "ch.h"

template <class datatype>
class DataSource;

/*!
 @brief A queue for receiving data
 @tparam datatype The datatype to be contained
 
 This is a unidirectional data structure for one-to-many communication from a
 sensor to applications which may want to use its data. Each application is
 expected to allocate its own memory for the queue in initialization and then
 register the queue to the sensor's corresponding DataSource.
 */
template <class datatype>
class DataListener {
public:
	/*!
	 @brief Initialize the data structures with a buffer
	 */
	void init(datatype * listener_buf, uint32_t max_len);
	
	/*!
	 @brief Receive a sample of data without copying
	 
	 This will provide a pointer to the sample at the front of the queue but
	 will not move it.
	 
	 @retval data if successful, 0 if timed out
	 */
	datatype * receive(uint32_t timeout_ms = TIME_INFINITE);
	
	/*!
	 @brief Wait for a result but do not pop it off of the queue
	 
	 The data structure returned will be protected until you perform a
	 @ref receive operation to pop it off of the queue
	 
	 @retval data if successful, 0 if timed out
	 */
	datatype * peekI(uint32_t timeout_ms = TIME_INFINITE);
	
	/*!
	 @brief Wait for a sample of data and copy it safely
	 
	 @retval Success
	 */
	bool receive_to(datatype & dst, uint32_t timeout_ms = TIME_INFINITE);
	
	/*!
	 @brief Push a new sample onto the queue
	 
	 @note This is to be called from within system lock zone (while traversing
	 a linked list probably)!
	 */
	bool pushI(datatype & src);

	void resetI(){
		chSemResetI(&sem, 0);
		rd_head = buf_head;
		wr_head = buf_head;
		next = nullptr;
		chSchRescheduleS();
	}
	
protected:
	Semaphore sem;
	
	DataListener * next;
	
	datatype * buf_head;
	datatype * rd_head, * wr_head;
	uint32_t len;
	
	// Let the DataSource class traverse the linked list
	friend class DataSource<datatype>;
};

template <class datatype>
class DataSource {
public:
	DataSource() :
		listener_head(nullptr)
	{}
	
	void register_queue(DataListener<datatype> &listener);
	
	void unregister_queue(DataListener<datatype> &listener);
	
	bool has_listeners(){return listener_head;}

	void put(datatype &src, bool reschedule = true);
protected:
	
	DataListener<datatype> * listener_head;
};

#endif // __IMU_DATASOURCE_H_
