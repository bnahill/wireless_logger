#ifndef __LPRY_H_
#define __LPRY_H_

#include "stm32f10x.h"
#include "sensor_config.h"

/*!
 @addtogroup lpry LPRYxxxAL Gyroscopes
 @brief The pair of 2-axis pitch/roll gyro with a 1-axis yaw gyro
 @note Much of this code is derived from the the iNEMO platform driver
 @{
 */

//! The datatype for a 3-axis gyroscope unit
typedef struct {
	//! The most recent pitch, roll, and yaw readings
	euler3_t   reading;
	//! Flag to indicate transfer complete
	uint8_t    done;
	//! Buffer to read all necessary values from ADC
	uint16_t   dma_buffer[5];
} lpry_t;

/*!
 @brief Initialize the hardware and power it on
 @pre None
 @post Hardware is initialized and ready to handle lpry_read calls
 */
void lpry_init(void);

/*!
 @brief Enable power to the gyros
 */
void lpry_power_on(void);

/*!
 @brief Power down the gyros
 */
void lpry_power_off(void);

/*!
 @brief Enable the self-test mode of the gyros
 */
void lpry_self_test_on(void);

/*!
 @brief Disable the self-test mode of they gyros
 */
void lpry_self_test_off(void);

/*!
 @brief Start a read on the gyros
 
 This will return almost immediately. Check lpry_xfer_complete to see if data
 is ready. If you want to perform the full read/wait/update sequence in one
 call, use lpry_read_sync.
 */
void lpry_read(void);

/*!
 @brief Check to see if the most recent read has been completed
 @return 1 if complete
 */
int lpry_xfer_complete(void);

/*!
 @brief Update the gyro state based on the most recent readings
 @pre lpry_xfer_complete returns 1
 */
void lpry_update(void);

/*!
 @brief Perform the complete read/wait/update process to get gyro data
 */
void lpry_read_sync(void);

//@}

#ifdef HAS_GYRO
extern lpry_t gyro;
#endif

#endif
