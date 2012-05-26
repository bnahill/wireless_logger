#ifndef __I2C_H_
#define __I2C_H_

#include "sensor_config.h"

typedef enum {
	I2C_MODE_DISABLED = 0,
	I2C_MODE_MASTER,
	//! Not implemented
	I2C_MODE_SLAVE
} i2c_mode_t;

typedef enum {
	I2C_OP_READ,
	I2C_OP_WRITE
} i2c_op_t;

typedef enum {
	I2C_ST_IDLE,
	I2C_ST_MASTER_REQ,
	I2C_ST_ADDRESSED,
	I2C_ST_ADDR_TXED,
	I2C_ST_REPEAT_START,
	I2C_ST_REPEAT_ADDR,
	I2C_ST_READING,
	I2C_ST_WRITING,
	I2C_ST_CLOSING_WRITE
} i2c_state_t;

//! @name I2C transfer termination flags
//! @{
#define I2C_XFER_FLAG_DONE 0x01
#define I2C_XFER_FLAG_ERR_MASK 0xFE
#define I2C_XFER_ERR_NOSLAVE 0x02
//! @}

typedef struct _i2c_transfer {
	i2c_op_t op;
	//! The I2C address of the slave
	uint8_t devaddr;
	//! The data address to read or write
	uint8_t addr;
	//! The buffer address
	uint8_t *buffer;
	//! The number of bytes to read or write
	uint8_t count;
	//! A flag to indicate completion and errors
	uint8_t done;
	struct _i2c_transfer *next;
} i2c_transfer_t;

/*!
 @brief A set of hardware-related constants for each I2C device
 */
typedef struct {
	GPIO_TypeDef *const sda_gpio;
	uint16_t const sda_pin;
	GPIO_TypeDef *const scl_gpio;
	uint16_t const scl_pin;
	IRQn_Type const irq_ev;
	IRQn_Type const irq_er;
	uint32_t const clock;
} i2c_config_t;

/*!
 @brief The state and configuration of an I2C device
 */
typedef struct {
	//! The I2C device
	I2C_TypeDef *const i2c;
	//! The mode
	i2c_mode_t mode;
	//! The current state
	i2c_state_t state;
	//! The current transfer
	i2c_transfer_t *xfer;
	//! A pointer to the hardware configuration
	i2c_config_t const * const config;
} i2c_t;

#if USE_I2C1
extern i2c_t i2c1;
#endif

#if USE_I2C2
extern i2c_t i2c2;
#endif

/*!
 @brief Initialize an I2C device
 @param i2c The I2C device to configure
 @param mode The mode to use. Currently only master is implemented
 @param speed The clock speed in Hz
 @return 0 if successful, 1 if already configured (not necessarily a problem)

 Initialize an I2C device. If the device is already initialized, nothing will
 be done and this will return 1. 
 */
int i2c_init(i2c_t *i2c, i2c_mode_t mode, uint32_t speed);

//void i2c_write_byte(i2c_t *i2c, uint8_t devaddr, uint8_t addr, uint8_t value);
void i2c_write_byte(i2c_t *i2c, uint8_t const devaddr, uint8_t const addr, uint8_t const value);

uint8_t i2c_read_byte(i2c_t *i2c, uint8_t devaddr, uint8_t addr);

void i2c_mk_transfer(i2c_transfer_t *xfer,
                            i2c_op_t op,
                            uint8_t devaddr,
                            uint8_t addr,
                            uint8_t *buffer,
                            uint8_t count);

/*!
 @brief Start or queue a transfer
 */
void i2c_transfer(i2c_t *i2c, i2c_transfer_t *xfer);

#endif

