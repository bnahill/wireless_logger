#ifndef __RTC_H_
#define __RTC_H_

#include "stm32f10x.h"
#include "toolchain.h"

/*!
 @addtogroup rtc Real-Time Clock
 @brief Keep track of time even when the CPU is off
 @{
 */

//! Type for the internal timestamp used
typedef uint32_t timestamp_t;

//! Structure for a human-readable date and time
typedef struct {
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} datetime_t;

/*!
 @brief Initialize the RTC hardware and software components
 */
void rtc_init(void);

/*!
 @brief Convert a date and time to a timestamp to be given to the RTC
 @param datetime The date and time to convert
 @return A timestamp representative of the provided time
 */
timestamp_t rtc_dt2ts(datetime_t const * const datetime);

/*!
 @brief Convert a UNIX timestamp to a date and time
 @param datetime The datetime object to populate
 @param timestamp The timestamp to convert
 */
void rtc_ts2dt(datetime_t *RESTRICT datetime, timestamp_t timestamp);

/*!
 @brief Set the time to the provided timestamp
 @param ts The timestamp to use
 */
void rtc_set_time(timestamp_t ts);

/*!
 @brief Set the time to the provided datetime stucture
 @param datetime The datetime structure to use
 */
void rtc_set_datetime(datetime_t const * datetime);

/*!
 @brief Get the current date and time from the RTC
 @param datetime The datetime structure to populate
 */
void rtc_get_datetime(datetime_t *RESTRICT datetime);

#endif
