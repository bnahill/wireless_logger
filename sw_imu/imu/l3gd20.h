#ifndef __IMU_L3GD20_H_
#define __IMU_L3GD20_H_

#include "ch.h"
#include "hal.h"
#include "imu/imu_math.h"
#include "imu/spi.h"

//! @addtogroup IMU
//! @{
//! @addtogroup Sensors
//! @{
//! @addtogroup L3GD20
//! @{

template <class spi_class>
class L3GD20 {
public:
	L3GD20<spi_class>(spi_class &spi, typename spi_class::slave_config_t spi_config);
	
	typedef enum {
		ODR_95 = 0,
		ODR_190 = 1,
		ODR_380 = 2,
		ODR_760 = 3
	} odr_t;

	typedef enum {
		LP_0 = 0,
		LP_1 = 1,
		LP_2 = 2,
		LP_3 = 3
	} lp_cutoff_t;
	
	typedef enum {
		POWER_DOWN = 0,
		POWER_NORMAL = 1
	} power_mode_t;
	
	//! High-pass filter mode
	typedef enum {
		HP_MODE_NORMAL_RST = 0,
		HP_MODE_REF        = 1,
		HP_MODE_NORMAL     = 2,
		HP_MODE_AUTORST    = 3
	} hp_mode_t;
	
	//! High-pass cutoff frequency selection (see datasheet)
	typedef uint8_t hp_cutoff_t;
	
	//! Full scale range
	typedef enum {
		FS_250 = 0,
		FS_500 = 1,
		FS_2000 = 2
	} fs_t;
	
	//! Interrupt line drive type
	typedef enum {
		DRIVE_PUSH_PULL = 0,
		DRIVE_OPEN_DRAIN = 1
	} irq_drive_t;
	
	//! Interrupt line polarity
	typedef enum {
		POLARITY_HIGH = 0,
		POLARITY_LOW = 1
	} irq_polarity_t;
	
	//! Available interrupts
	typedef enum {
		INT_I1 = 0x80,
		INT_I1_BOOT = 0x40,
		INT_I2_DRDY = 0x08,
		INT_I2_WTM = 0x04,
		INT_I2_ORUN = 0x02,
		INT_I2_EMPTY = 0x01,
		INT_ALL = 0xCF
	} irq_t;
	
	typedef enum {
		ENDIAN_LITTLE = 0,
		ENDIAN_BIG = 1
	} endian_mode_t;
	
	typedef enum {
		SIG_SRC_LPF1 = 0,
		SIG_SRC_HPF = 1,
		SIG_SRC_LPF2 = 2
	} signal_source_t;
	
	/*!
	 @brief Initialize device and confirm proper operation
	 @return Success
	 */
	bool init();
	
	/*!
	 @brief Read a new set of gyro values
	 @post A new set of values will be available in readings
	 */
	void read();
	
	/*!
	 @brief Read the 8-bit temperature value
	 @post temperature will be updated
	 @return The temperature in C
	 */
	int8_t read_temperature();
	
	/*!
	 @brief Set the power mode of the device
	 @param new_pm The new power mode
	 @param write Write to the device
	 */
	void set_power_mode(power_mode_t new_pm, bool write = true);
	
	/*!
	 @brief Set the full-scale range of the device
	 @param new_fs The new full-scale range
	 @param write Write to the device
	 */
	void set_full_scale(fs_t new_fs, bool write = true);
	
	//! The latest temperature reading
	uint8_t get_temperature() const{return temperature;}
	
	//! The latest angular velocity reading
	void get_reading(Euclidean3_f32 &dst){
		chSemWait(&result_lock);
		dst = reading;
		chSemSignal(&result_lock);
	}
	
protected:
	//! @brief Register address map
	typedef enum {
		REG_WHO_AM_I    = 0x0F,
		REG_CTRL_REG1   = 0x20,
		REG_CTRL_REG2   = 0x21,
		REG_CTRL_REG3   = 0x22,
		REG_CTRL_REG4   = 0x23,
		REG_CTRL_REG5   = 0x24,
		REG_REF_DATACAP = 0x25,
		REG_OUT_TEMP    = 0x26,
		REG_STATUS      = 0x27,
		REG_OUT_X_L     = 0x28,
		REG_OUT_X_H     = 0x29,
		REG_OUT_Y_L     = 0x2A,
		REG_OUT_Y_H     = 0x2B,
		REG_OUT_Z_L     = 0x2C,
		REG_OUT_Z_H     = 0x2D,
		REG_FIFO_CTRL   = 0x2E,
		REG_FIFO_SRC    = 0x2F,
		REG_INT1_CFG    = 0x30,
		REG_INT1_SRC    = 0x31,
		REG_INT1_THS_XH = 0x32,
		REG_INT1_THS_XL = 0x33,
		REG_INT1_THS_YH = 0x34,
		REG_INT1_THS_YL = 0x35,
		REG_INT1_THS_ZH = 0x36,
		REG_INT1_THS_ZL = 0x37,
		REG_INT1_DURA   = 0x38
	} reg_t;
	
	/*!
	 @brief Read from the SPI device
	 @param addr The virtual address to start read
	 @param n The number of bytes to read after the first
	 @param dst The destination buffer of at least (n+1) bytes
	 */
	void spi_read(reg_t addr, size_t n, uint8_t *dst);
	
	/*!
	 @brief Read a single register
	 @param addr The virtual address to read
	 @return The value at the specified address
	 */
	uint8_t spi_read_reg(reg_t addr);
	
	/*!
	 @brief Write a register on the device
	 @param addr The virtual address to write
	 @param value The value to write
	 */
	void spi_write_reg(reg_t addr, uint8_t value);
	
	/*!
	 @name Control register operations
	 Generate the correct value for each control register and, if desired,
	 write that value to the device
	 @{
	 */
	uint8_t update_reg_ctrl1(bool write = true);
	
	uint8_t update_reg_ctrl2(bool write = true);
	
	uint8_t update_reg_ctrl3(bool write = true);
	
	uint8_t update_reg_ctrl4(bool write = true);
	
	uint8_t update_reg_ctrl5(bool write = true);
	
	void update_ctrl_regs();
	/*!
	 @}
	 */
	
	/*!
	 @brief Enable an interrupt generator
	 @param irq_mask The mask of interrupts to enable
	 @param write Actually write to the device
	 */
	void enable_irq(irq_t irq_mask, bool write=true);
	
	/*!
	 @brief Disable an interrupt generator
	 @param irq_mask The mask of interrupts to disable
	 @param write Actually write to the device
	 */
	void disable_irq(irq_t irq_mask, bool write=true);
	
	//! The SPI peripheral to use
	spi_class &spi;
	
	//! The SPI configuration to use
	typename spi_class::slave_config_t spi_config;
	
	//! The current reading
	Euclidean3_f32 reading;
	
	
	//! The current temperature
	int8_t temperature;
	
	//! @name Device configuration
	//! @{
	odr_t odr;
	lp_cutoff_t lp_cutoff;
	power_mode_t power_mode;
	
	hp_mode_t   hp_mode;
	hp_cutoff_t hp_cutoff;
	uint8_t interrupts;
	
	irq_drive_t irq_drive;
	irq_polarity_t irq_polarity;
	bool bdu_mode;
	endian_mode_t endian_mode;
	bool fifo_enabled;
	bool hp_enabled;
	signal_source_t output_selection;
	signal_source_t int1_selection;
	
	fs_t fs;
	//! @}
	
	//! The current sensitivity in degrees/lsb
	float dps_scale;
	
	Semaphore result_lock;
	
	
	static float get_dps_scale(fs_t fs);
	
	//! Callback for asynchronous SPI transfers
	static void callback(void *);
	
	static constexpr uint8_t whoami = 0xD4;
	
	/*!
	 @name Flags for SPI transactions
	 @{
	 */
	static constexpr uint8_t FLAG_READ = 0x80;
	static constexpr uint8_t FLAG_SEQ = 0x40;
	/*!
	 @}
	 */
};

#include "l3gd20.cpp"

//! @brief A 4-wire SPI L3GD20 configuration
typedef L3GD20<SPI> L3GD20_SPI;

//! @} @} @}

#endif // __IMU_L3GD20_H_
