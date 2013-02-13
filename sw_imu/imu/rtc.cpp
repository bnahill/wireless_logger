#include "imu/rtc.h"

template <RTCDriver &driver>
void RealTimeClock< driver >::get_time_text(char * text){
	union {
		rtc_time_t time;
		RTCTime rtctime;
	};
	rtcGetTime(&driver, &rtctime);
	
	text[0] = '0' + time.hours_ten;
	text[1] = '0' + time.hours_u;
	text[2] = ':';
	
	text[3] = '0' + time.minutes_ten;
	text[4] = '0' + time.minutes_u;
	text[5] = ':';
	
	text[6] = '0' + time.seconds_ten;
	text[7] = '0' + time.seconds_u;
	text[8] = 0;
	
	
	return;
}


template <RTCDriver &driver>
typename RealTimeClock< driver >::rtc_time_t RealTimeClock< driver >::get_time(){
	rtc_time_t time;
	RTCTime t;
	rtcGetTime(&driver, &t);
	
	return time;
}