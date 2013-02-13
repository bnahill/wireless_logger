#ifndef __IMU_RTC_H_
#define __IMU_RTC_H_

#include "ch.h"
#include "hal.h"

#include "time.h"
#include "chrtclib.h"

template <RTCDriver &driver>
class RealTimeClock {
public:
	struct rtc_time_t {
		uint32_t days_u       : 4;
		uint32_t days_ten     : 2;
		uint32_t dummy1       : 1;
		uint32_t months_u     : 4;
		uint32_t months_ten   : 1;
		enum {
			CRAP = 0,
			MONDAY = 1,
			TUESDAY = 2,
			WEDNESDAY = 3,
			THURSDAY = 4,
			FRIDAY = 5,
			SATURDAY = 6,
			SUNDAY = 7
		} day_of_week         : 3;
		uint32_t years_u      : 4;
		uint32_t years_ten    : 4;
		uint32_t dummy2       : 8;
		uint32_t seconds_u    : 4;
		uint32_t seconds_ten  : 3;
		uint32_t dummy3       : 1;
		uint32_t minutes_u    : 4;
		uint32_t minutes_ten  : 3;
		uint32_t dummy4       : 1;
		uint32_t hours_u      : 4;
		uint32_t hours_ten    : 2;
		uint32_t dummy5       : 10;
	};

	static rtc_time_t get_time();
	
	static void get_time_text(char * text);

	static void get_time(rtc_time_t &time){
		rtcGetTime(&driver, (RTCTime *)&time);
	}
	
	static void get_time(struct tm * time){
		rtcGetTimeTm(&driver, time);
	}
	
	static void set_time(rtc_time_t &time){
		rtcSetTime(&driver, (RTCTime *)&time);
	}
	
	static void set_time(uint32_t timestamp){
		time_t t;
		t = timestamp;
		rtcSetTimeUnixSec(&driver, t);
	}
};

#include <rtc.cpp>

#endif // __IMU_RTC_H_