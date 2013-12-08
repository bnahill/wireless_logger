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
	static constexpr uint32_t stack_size = 2048;
	static constexpr uint32_t buffer_len = 8;
	
	WORKING_AREA(SVThread, stack_size);

	
	msg_t terminate();
	
	typedef enum {
		MODE_ACC,
		MODE_GYRO,
		MODE_MAG,
		MODE_PRS
	} mode_t;
	
	Thread * thread;
	
	void thread_action();
	
	static msg_t start_thread(SensorView * sv){
		sv->thread_action();
		return 0;
	}
	
	union {
		decltype(Platform::mag_source)::listener_t mag_listener;
		decltype(Platform::acc_source)::listener_t acc_listener;
		decltype(Platform::gyro_source)::listener_t gyro_listener;
		decltype(Platform::prs_source)::listener_t prs_listener;
	};

	union{
		decltype(Platform::acc_source)::data_t acc_buffer[buffer_len];
		decltype(Platform::mag_source)::data_t mag_buffer[buffer_len];
		decltype(Platform::gyro_source)::data_t gyro_buffer[buffer_len];
		decltype(Platform::prs_source)::data_t prs_buffer[buffer_len];
	};

	union{
		decltype(Platform::acc_source)::data_t acc_measurement;
		decltype(Platform::mag_source)::data_t mag_measurement;
		decltype(Platform::gyro_source)::data_t gyro_measurement;
		decltype(Platform::prs_source)::data_t prs_measurement;
	};

	mode_t mode;
	display_mode_t display_mode;
};

//! @} @}

#endif // __UI_SENSOR_VIEW_H_
