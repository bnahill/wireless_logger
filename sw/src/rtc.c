#include "rtc.h"

static const datetime_t initial_datetime = {
	1985,
	10,
	26,
	1,
	15,
	0
};

static uint8_t days_in_month[12] = {
	// Jan
	31,
	// Feb (fill in later)
	0,
	// March
	31,
	// April
	30,
	// May
	31,
	// June
	30,
	// July
	31,
	// August
	31,
	// September
	30,
	// October
	31,
	// November
	30,
	//December
	31
};

void rtc_init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	
	/* Reset Backup Domain */
	BKP_DeInit();
	
	// Enable external 32kHz crystal
	RCC_LSEConfig(RCC_LSE_ON);
	
	// Wait till LSE is ready
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	
	// Select LSE as RTC Clock Source
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	// Enable RTC Clock
	RCC_RTCCLKCmd(ENABLE);
	
	// Wait for RTC registers synchronization
	RTC_WaitForSynchro();
	
	// Wait until last write operation on RTC registers has finished
	RTC_WaitForLastTask();
	
	// Enable the RTC Second
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	// Wait until last write operation on RTC registers has finished
	RTC_WaitForLastTask();
	
	// Set RTC prescaler: set RTC period to 1sec
	// RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
	RTC_SetPrescaler(32767);
	
	// Wait until last write operation on RTC registers has finished
	RTC_WaitForLastTask();
	
	rtc_set_datetime(&initial_datetime);
}

timestamp_t rtc_dt2ts(datetime_t const * const datetime){
	timestamp_t ts;
	uint8_t *month;
	
	// Start with number of years
	ts = (datetime->year - 1970) * 24 * 3600 * 365;
	
	// Correct for past leap years
	ts += ((datetime->year - 1969) / 4) * 24 * 3600;
	
	// Correct each month for current leap year
	if((datetime->year & 3) == 0)
		days_in_month[1] = 29;
	else 
		days_in_month[1] = 28;
	
	// Iterate through each month, adding up the days behind it
	for(month = days_in_month; month < (days_in_month + datetime->month - 1); month++){
		// Add the full current month
		ts += *month * 24 * 3600;
	}
	
	// Add days from current month
	ts += (datetime->day - 1) * 24 * 3600;
	
	// Add hours, minutes, and seconds
	ts += datetime->hour * 3600 + datetime->minute * 60 + datetime->second;
	
	return ts;
}

void rtc_set_time(timestamp_t ts){
	RTC_WaitForLastTask();
	// Change the current time
	RTC_SetCounter(ts);
	// Wait until last write operation on RTC registers has finished
	RTC_WaitForLastTask();
}

void rtc_set_datetime(datetime_t const * datetime){
	rtc_set_time(rtc_dt2ts(datetime));
}

void rtc_get_datetime(datetime_t *RESTRICT datetime){
	timestamp_t ts;
	ts = RTC_GetCounter();
	rtc_ts2dt(datetime, ts);
}

void rtc_ts2dt(datetime_t *RESTRICT datetime, timestamp_t timestamp){
	uint8_t extra_days;
	uint8_t year;
	uint16_t day;
	uint32_t seconds;
	uint8_t *month;
	
	// Years since 1970
	year = timestamp / (3600 * 24 * 365);
	datetime->year = year + 1970;
	
	// How many extra days are there from PREVIOUS leap years?
	// This should be such that 1973 was the first year to have this set
	extra_days = (year + 1) / 4;
	
	// Second of the year
	seconds = (timestamp - year * (3600 * 24 * 365)) - (extra_days * 24 * 3600);
	// Day of the year
	day = seconds / (3600 * 24);
	
	// Correct for leap year
	// 1970 was 2 years from a leap year so this is adjusted accordingly
	if(((year + 2) & 3) == 0)
		days_in_month[1] = 29;
	else 
		days_in_month[1] = 28;

	for(month = days_in_month; month < (days_in_month + 12); month++){
		if(day >= *month){
			day -= *month;
		} else {
			// Iterator is now on the current month and day now
			// is the day of the month
			break;
		}
	}
	
	// Months are indexed from 1
	datetime->month = 1 + (month - days_in_month);
	// As are days of months
	datetime->day = 1 + day;
	
	// Seconds in current day
	seconds = seconds % (24 * 3600);
	
	datetime->hour = seconds / 3600;
	
	// Seconds in current hour
	seconds -= datetime->hour * 3600;
	
	datetime->minute = seconds / 60;
	
	seconds -= datetime->minute * 60;
	
	datetime->second = seconds;  
}
