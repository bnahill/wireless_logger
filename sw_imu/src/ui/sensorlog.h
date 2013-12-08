#ifndef __UI_SENSORLOG_H_
#define __UI_SENSORLOG_H_

#include "ui/ui.h"
#include "flogfs.h"

class SensorEditView {
public:
	SensorEditView(char const * name, uint32_t rate, bool enabled=false) :
		name(name), rate(rate), enabled(enabled)
	{
	}

	void draw(uint32_t margins=12);

	eventmask_t handle_event(eventmask_t event){
		switch(event){
		case UI::MASK_SELECT:
			enabled = !enabled;
			draw();
			break;
		default:
			break;
		}
		return event;
	}

	uint32_t get_rate() const {return rate;}
	bool get_enabled() const {return enabled;}

protected:
	bool enabled;
	char const * name;
	uint32_t rate;
};

class SensorLogUI {
public:
	SensorLogUI() :
		sensors({
		    {"Accelerometer",1},{"Gyroscope",1},
		    {"Magnetometer",4},{"Pressure",4},{"Temperature",4}
		}), current_index(0), log_state(LOG_STOPPED)
	{}

	void exec();
protected:
	void draw();
	enum {
		LOG_STOPPED,
		LOG_RUNNING
	} log_state;

	msg_t terminate();

	uint8_t current_index;

	static constexpr uint32_t num_sensors = 5;
	SensorEditView sensors[num_sensors];

	char err_msg[32];

	static constexpr uint32_t stack_size = 4096;

	void write_header();

	flog_write_file_t write_file;

	Thread * thread;

	WORKING_AREA(SLThread, stack_size);

	typedef enum {
		ACTION_START = 1,
		ACTION_STOP = 2
	} action_evt_t;
	msg_t thread_action();

	static msg_t start_thread(SensorLogUI * sl){
		sl->thread_action();
		return 0;
	}
};

#endif // __UI_SENSORLOG_H_
