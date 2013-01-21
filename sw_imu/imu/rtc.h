#ifndef __IMU_RTC_H_
#define __IMU_RTC_H_

#include "ch.h"
#include "hal.h"



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

	static rtc_time_t get_time(){
		rtc_time_t time;
		RTCTime t;
		rtcGetTime(&driver, &t);
		
		return time;
	}
	
	static void get_time_text(char * text){
		RTCTime time;
		rtcGetTime(&driver, &time);
		
		text[0] = 0;
		return;
	/*	
		text[0] = '0';
		while(time.hours >= 10){
			text[0] += 1;
			time.hours -= 10;
		}
		text[1] = time.hours + '0';
		
		text[2] = ':';
		
		text[3] = '0';
		while(time.minutes >= 10){
			text[3] += 1;
			time.minutes -= 10;
		}
		text[4] = time.minutes + '0';
		
		text[5] = ':';
		
		text[6] = '0';
		while(time.seconds >= 10){
			text[6] += 1;
			time.seconds -= 10;
		}
		text[7] = time.seconds + '0';
		text[8] = 0;*/
	}

	static void get_time(rtc_time_t &time){
		rtcGetTime(&driver, (RTCTime *)&time);
	}
	
	static void set_time(rtc_time_t &time){
		rtcSetTime(&driver, (RTCTime *)&time);
	}
};

#endif // __IMU_RTC_H_