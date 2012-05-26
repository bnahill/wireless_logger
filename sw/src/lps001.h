#ifndef __LPS001_H_
#define __LPS001_H_

#include "i2c.h"

//! @addtogroup lps LPS001 Pressure Sensor
//! @{

//! The state and configuration of a sensor
typedef struct {
	//! The I2C device it is connected to
	i2c_t *const i2c;
	//! The most recent pressure reading in mbar
	float pressure;
	//! The most recent temperature reading in C
	float temperature;
	//! The transfer state object
	i2c_transfer_t xfer;
	//! A buffer for read data
	uint8_t buffer[4];
} lps001_t;

/*!
 @brief Initialize the sensor and related peripherals
 */
void lps_init(void);

/*!
 @brief Begin a read of the sensor
 
 This will begin a transfer and immediately return. Check with
 lps_xfer_complete to check if it has finished. To perform all of this in a
 single function, see lps_read_sync.
 */
void lps_read(void);

/*!
 @brief Check to see if a recent transfer has completed
 @return 1 if complete
 
 Once this returns 1, it is okay to run lps_update_all
 */
int lps_xfer_complete(void);

/*!
 @brief Update the pressure and temperature values based on the data from a 
 recent read
 */
void lps_update(void);

/*!
 @brief Perform all steps from starting a read to writing the results to the
 device state registers
 */
void lps_read_sync(void);

//! @}

extern lps001_t pressure;

#endif
