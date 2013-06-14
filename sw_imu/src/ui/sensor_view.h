#ifndef __UI_SENSOR_VIEW_H_
#define __UI_SENSOR_VIEW_H_

#include "platform.h"
#include "acquisition.h"
#include "ui/ui.h"

//! @addtogroup UI
//! @{
//! @addtogroup Screens
//! @{

/*!
 @brief A screen displaying pretty sensor readings either numerically
 or in bar graphs
 */
class SensorView {
public:
	typedef enum {
		DISP_NUMBER,
		DISP_PROGRESS
	} display_mode_t;
	
	SensorView(display_mode_t display_mode) :
		display_mode(display_mode),
		mode(MODE_ACC)
	{}
	
	void exec();
protected:
	static constexpr uint32_t stack_size = 1024;
	static constexpr uint32_t buffer_len = 8;
	
	WORKING_AREA(SVThread, stack_size);

	
	msg_t terminate();
	
	typedef enum {
		MODE_ACC,
		MODE_GYRO,
		MODE_MAG
	} mode_t;
	
	Thread * thread;
	
	void thread_action();
	
	static msg_t start_thread(SensorView * sv){
		sv->thread_action();
		return 0;
	}
	
	DataListener<Euclidean3_f32> data_listener;
	Euclidean3_f32 data_buffer[buffer_len];
	
	mode_t mode;
	display_mode_t display_mode;
};

//! @} @}

#endif // __UI_SENSOR_VIEW_H_
