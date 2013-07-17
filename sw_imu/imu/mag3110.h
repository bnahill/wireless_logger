/*!
 @file mag3110.h
 
 @brief Class definition for Freescale \ref MAG3110 magnetometer
 
 @author Ben Nahill <bnahill@gmail.com>
 */

#ifndef __MAG3110_H_
#define __MAG3110_H_

#include "imu/imu_math.h"
#include "imu/i2c.h"

//! @addtogroup IMU
//! @{
//! @addtogroup Sensors
//! @{
//! @addtogroup MAG3110
//! @{

class MAG3110 {
public:
	typedef enum {
		MODE_STANDBY = 0,
		MODE_ACTIVE = 1
	} mode_t;
	
	/*!
	 @brief The output data rate
	 */
	typedef enum {
		DR_80 = 0,
		DR_40 = 1,
		DR_20 = 2,
		DR_10 = 3,
		DR_5   = 4,
		DR_2_5 = 5,
		DR_1_25 = 6,
		DR_0_63 = 7
	} dr_t;
	
	/*!
	 @brief Oversampling ratio. Deviation from OR_16 will reduce the output
	 rate accordingly.
	 */
	typedef enum {
		OS_16  = 0,
		OS_32 = 1,
		OS_64 = 2,
		OS_128 = 3
	} os_t;
	
	typedef enum {
		AUTORESET_OFF = 0,
		AUTORESET_ON = 1
	} autoreset_mode_t;
	
	typedef enum {
		RAW_MODE_DISABLE = 0,
		RAW_MODE_ENABLE = 1
	} raw_mode_t;
	
	MAG3110(I2C &i2c, uint8_t devaddr) : 
		i2c(i2c), devaddr(devaddr),
		dr(DR_80), os(OS_128), mode(MODE_STANDBY),
		autoreset_mode(AUTORESET_ON), raw_mode(RAW_MODE_DISABLE)
		{}

	/*!
	 @brief Initialize MAG3110 device
	 @return True if successful
	 @pre OS initialized
	 @post I2C device and MAG3110 intialized
	 */
	bool init();
	
	
	/*!
	 @brief Take a reading
	 */
	void read();
	
	//! @name Configuration
	//! @{
	dr_t dr;
	os_t os;
	mode_t mode;
	autoreset_mode_t autoreset_mode;
	raw_mode_t raw_mode;
	//! @}

	/*!
	 @brief Set the data rate
	 @param new_dr New data rate
	 @param update Actually update registers?
	 */
	void set_dr(dr_t new_dr, bool update = true){
		dr = new_dr;
		if(update)
			write_ctrl_reg1();
	}
	
	/*!
	 @brief Set oversampling ratio
	 @param new_os New oversampling ratio
	 @param update Actually update registers?
	 */
	void set_os(os_t new_os, bool update = true){
		os = new_os;
		if(update)
			write_ctrl_reg1();
	}
	
	/*!
	 @brief Set operating mode
	 @param new_mode New mode
	 @param update Actually update registers?
	 */
	void set_mode(mode_t new_mode, bool update = true){
		mode = new_mode;
		if(update)
			write_ctrl_reg1();
	}
	
	/*!
	 @brief Set operating mode
	 @param new_autoreset_mode New mode
	 */
	void set_autoreset_mode(autoreset_mode_t new_autoreset_mode){
		autoreset_mode = new_autoreset_mode;
		write_ctrl_reg2();
	}
	
	
	void get_reading(Euclidean3_f32 &dst){
		chSemWait(&result_lock);
		dst = reading;
		chSemSignal(&result_lock);
	}
	
private:
	/*!
	 @brief Write current settings to CTRL_REG1
	 */
	void write_ctrl_reg1();
	
	/*!
	 @brief Write current settings to CTRL_REG2
	 */
	void write_ctrl_reg2();
	
	//! @brief Register address map
	typedef enum {
		REG_DR_STATUS     = 0x00,
		REG_OUT_X_MSB     = 0x01,
		REG_OUT_X_LSB     = 0x02,
		REG_OUT_Y_MSB     = 0x03,
		REG_OUT_Y_LSB     = 0x04,
		REG_OUT_Z_MSB     = 0x05,
		REG_OUT_Z_LSB     = 0x06,
		REG_WHO_AM_I      = 0x07,
		REG_SYSMOD        = 0x08,
		
		REG_OFF_X_MSB     = 0x09,
		REG_OFF_X_LSB     = 0x0A,
		REG_OFF_Y_MSB     = 0x0B,
		REG_OFF_Y_LSB     = 0x0C,
		REG_OFF_Z_MSB     = 0x0D,
		REG_OFF_Z_LSB     = 0x0E,
		
		REG_DIE_TEMP      = 0x0F,
		REG_CTRL_REG1     = 0x10,
		REG_CTRL_REG2     = 0x11
	} reg_t;
	
	//! The I2C slave address
	uint8_t const devaddr;
	
	//! A transfer to use for periodic reads
	uint8_t xfer_buffer[6];
	
	//! I2C device to use
	I2C &i2c;
	
	//! The value expected at REG_WHO_AM_I
	static constexpr uint8_t whoami = 0xC4;
	
	//! Reading in uT
	Euclidean3_f32 reading;
	
	Semaphore result_lock;
	
};

//! @} @} @}

#endif
