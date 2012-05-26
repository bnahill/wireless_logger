#include "stm32f10x.h"
#include "i2c.h"

#if USE_I2C1
static const i2c_config_t i2c1_config = {
	// SDA
	GPIOB,
	BIT(7),
	// SCL
	GPIOB,
	BIT(6),
	// IRQs
	I2C1_EV_IRQn,
	I2C1_ER_IRQn,
	// Clock
	RCC_APB1Periph_I2C1
};

i2c_t i2c1 = {
	I2C1,
	I2C_MODE_DISABLED,
	I2C_ST_IDLE,
	NULL,
	&i2c1_config
};
#endif

#if USE_I2C2
static const i2c_config_t i2c2_config = {
	// SDA
	GPIOB,
	BIT(11),
	// SCL
	GPIOB,
	BIT(10),
	// IRQs
	I2C2_EV_IRQn,
	I2C2_ER_IRQn,
	// Clock
	RCC_APB1Periph_I2C2
};

i2c_t i2c2 = {
	I2C2,
	I2C_MODE_DISABLED,
	I2C_ST_IDLE,
	NULL,
	&i2c2_config
};
#endif

int i2c_init(i2c_t *i2c, i2c_mode_t mode, uint32_t speed){
	i2c_config_t const * const conf = i2c->config;
	GPIO_InitTypeDef gpio_init_s;
	I2C_InitTypeDef i2c_init_s;
	NVIC_InitTypeDef nvic_init_s;

	__disable_irq();
	if(i2c->mode != I2C_MODE_DISABLED){
		__enable_irq();
		return 1;
	}

	GPIO_StructInit(&gpio_init_s);
	I2C_StructInit(&i2c_init_s);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	// All I2C peripherals are on APB1
	RCC_APB1PeriphClockCmd(conf->clock, ENABLE);
	
	// Reset
	RCC_APB1PeriphResetCmd(conf->clock, ENABLE);
	RCC_APB1PeriphResetCmd(conf->clock, DISABLE);

	////////////////////////////////////////////////////////////////////
	// GPIO Config
	////////////////////////////////////////////////////////////////////

	// Mode configuration
	gpio_init_s.GPIO_Mode = GPIO_Mode_AF_OD;
	gpio_init_s.GPIO_Speed = GPIO_Speed_10MHz;

	// For some reason the order here matters and it really shouldn't
	gpio_init_s.GPIO_Pin = conf->scl_pin;
	GPIO_Init(conf->scl_gpio, &gpio_init_s);
	gpio_init_s.GPIO_Pin = conf->sda_pin;
	GPIO_Init(conf->sda_gpio, &gpio_init_s);

	////////////////////////////////////////////////////////////////////
	// I2C Config
	////////////////////////////////////////////////////////////////////

	i2c_init_s.I2C_Mode = I2C_Mode_I2C;
	i2c_init_s.I2C_DutyCycle = I2C_DutyCycle_2;
	i2c_init_s.I2C_ClockSpeed = 100000;
	i2c_init_s.I2C_OwnAddress1 = 0xA0;
	i2c_init_s.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	i2c_init_s.I2C_Ack = I2C_Ack_Enable;

	////////////////////////////////////////////////////////////////////
	// Interrupt Config
	////////////////////////////////////////////////////////////////////

	nvic_init_s.NVIC_IRQChannel = conf->irq_er;
	nvic_init_s.NVIC_IRQChannelSubPriority = 2;
	nvic_init_s.NVIC_IRQChannelPreemptionPriority = 2;
	nvic_init_s.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_init_s);

	nvic_init_s.NVIC_IRQChannel = conf->irq_ev;
	nvic_init_s.NVIC_IRQChannelSubPriority = 3;
	nvic_init_s.NVIC_IRQChannelPreemptionPriority = 3;
	nvic_init_s.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_init_s);

	I2C_ITConfig(i2c->i2c, I2C_IT_ERR, ENABLE);
	I2C_ITConfig(i2c->i2c, I2C_IT_BUF, ENABLE);
	I2C_ITConfig(i2c->i2c, I2C_IT_EVT, ENABLE);

	////////////////////////////////////////////////////////////////////
	// Finalize
	////////////////////////////////////////////////////////////////////

	I2C_Cmd(i2c->i2c, ENABLE);

	I2C_Init(i2c->i2c, &i2c_init_s);

	i2c->mode = mode;

	__enable_irq();
	return 1;
}

/*!
 @brief Begin running this transfer, skipping all checks
 @param i2c The I2C device
 @param xfer The transfer to start

 This is intended to be used internally to start each transfer
 */
extern INLINE void i2c_run_xfer(i2c_t *i2c, i2c_transfer_t *xfer){
	i2c->xfer = xfer;
	i2c->state = I2C_ST_MASTER_REQ;
	while(i2c->i2c->SR2 & I2C_SR2_BUSY);
	i2c->i2c->CR1 &= ~I2C_CR1_PE;
	i2c->i2c->CR1 |= I2C_CR1_PE;
	
	I2C_AcknowledgeConfig(i2c->i2c, ENABLE);
	I2C_GenerateSTART(i2c->i2c, ENABLE);
}

void i2c_transfer(i2c_t *i2c, i2c_transfer_t *xfer){
	i2c_transfer_t *old_xfer;
	// Clear the 'done' flag for this chain of transfers
	for(old_xfer = xfer; old_xfer != NULL; old_xfer = old_xfer->next){
		old_xfer->done = 0;
	}

	// Disable interrupts to modify the I2C transfer list
	__disable_irq();
	old_xfer = i2c->xfer;
	if(old_xfer != NULL){
		for(;old_xfer->next != NULL; old_xfer = old_xfer->next);
		old_xfer->next = xfer;
	} else {
		i2c_run_xfer(i2c, xfer);	
	}
	__enable_irq();
}

INLINE void i2c_mk_transfer(i2c_transfer_t *xfer,
                           i2c_op_t op,
                           uint8_t devaddr,
                           uint8_t addr,
                           uint8_t *buffer,
                           uint8_t count){
	xfer->op = op;
	xfer->devaddr = devaddr;
	xfer->addr = addr;
	xfer->buffer = buffer;
	xfer->count = count;
	xfer->next = NULL;
}

void i2c_write_byte(i2c_t *i2c, uint8_t devaddr, uint8_t addr, uint8_t value){
	i2c_transfer_t xfer;
	i2c_mk_transfer(&xfer,
	                I2C_OP_WRITE,
	                devaddr,
	                addr,
	                &value,
	                1
	);
	i2c_transfer(i2c, &xfer);
	while(!xfer.done);
}

uint8_t i2c_read_byte(i2c_t *i2c, uint8_t devaddr, uint8_t addr){
	uint8_t result;
	i2c_transfer_t xfer;
	i2c_mk_transfer(&xfer,
	                I2C_OP_READ,
	                devaddr,
	                addr,
	                &result,
	                1
	);
	i2c_transfer(i2c, &xfer);
	while(!xfer.done);
	return result;
}

/*
void i2c_write(i2c_t *i2c, uint8_t devaddr, uint8_t addr, uint8_t *buffer, uint8_t count){
	i2c_spinlock(i2c);
	i2c->op = I2C_OP_WRITE;
	i2c->addr = addr;
	i2c->devaddr = devaddr;
	i2c->count = count;
	i2c->buffer = buffer;
	i2c->state = I2C_ST_MASTER_REQ;
	i2c->done = 0;

	while(i2c->i2c->SR2 & I2C_SR2_BUSY);

	I2C_AcknowledgeConfig(i2c->i2c, ENABLE);
	I2C_GenerateSTART(i2c->i2c, ENABLE);
}

void i2c_read(i2c_t *i2c, uint8_t devaddr, uint8_t addr, uint8_t *buffer, uint8_t count){
	i2c_spinlock(i2c);
	i2c->op = I2C_OP_READ;
	i2c->addr = addr;
	i2c->devaddr = devaddr;
	i2c->count = count;
	i2c->buffer = buffer;
	i2c->state = I2C_ST_MASTER_REQ;
	i2c->done = 0;

	while(i2c->i2c->SR2 & I2C_SR2_BUSY);

	I2C_AcknowledgeConfig(i2c->i2c, ENABLE);
	I2C_GenerateSTART(i2c->i2c, ENABLE);
}
*/

extern INLINE int i2c_check_evt(uint32_t event1, uint32_t event2){
	if((event1 & event2) == event2)
		return 1;
	return 0;
}

extern INLINE void i2c_next_xfer(i2c_t *RESTRICT const i2c){
	i2c->state = I2C_ST_IDLE;
	if(i2c->xfer->next){
		i2c_run_xfer(i2c, i2c->xfer->next);
	} else {
		i2c->i2c->CR1 &= ~I2C_CR1_PE;
		i2c->xfer = NULL;
	}
}
extern INLINE void i2c_read_isr_evt(i2c_t *RESTRICT const i2c){
	uint32_t const event = I2C_GetLastEvent(i2c->i2c);
	switch(i2c->state){
	case I2C_ST_MASTER_REQ:
		if(i2c_check_evt(event, I2C_EVENT_MASTER_MODE_SELECT)){
			I2C_AcknowledgeConfig(i2c->i2c, ENABLE);
			I2C_Send7bitAddress(i2c->i2c, i2c->xfer->devaddr, I2C_Direction_Transmitter);
			i2c->state = I2C_ST_ADDRESSED;
		}
		break;
	case I2C_ST_ADDRESSED:
		if(event == I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED){
			(void)i2c->i2c->SR1;
			I2C_SendData(i2c->i2c, i2c->xfer->addr);
			i2c->state = I2C_ST_ADDR_TXED;
		}
		break;
	case I2C_ST_ADDR_TXED:
		if(I2C_GetFlagStatus(i2c->i2c, I2C_FLAG_BTF) == SET){
			I2C_GenerateSTART(i2c->i2c, ENABLE);
			i2c->state = I2C_ST_REPEAT_START;
		}
		break;
	case I2C_ST_REPEAT_START:
		if(event == I2C_EVENT_MASTER_MODE_SELECT){
			(void)i2c->i2c->SR1;
			I2C_Send7bitAddress(i2c->i2c, i2c->xfer->devaddr, I2C_Direction_Receiver);
			I2C_ITConfig(i2c->i2c, I2C_IT_BUF , ENABLE);
			i2c->state = I2C_ST_REPEAT_ADDR;
		}
		break;
	case I2C_ST_REPEAT_ADDR:
		if(event == I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED){
			if(i2c->xfer->count  == 1){
				// NACK the next byte
				I2C_AcknowledgeConfig(i2c->i2c, DISABLE);
			}
			i2c->state = I2C_ST_READING;
		}
		break;
	case I2C_ST_READING:
		if(event == I2C_EVENT_MASTER_BYTE_RECEIVED){
			*(i2c->xfer->buffer++) = I2C_ReceiveData(i2c->i2c);
			if(--i2c->xfer->count == 1){
				// NACK the next byte
				I2C_AcknowledgeConfig(i2c->i2c, DISABLE);
			} else if (i2c->xfer->count == 0){
				i2c->state = I2C_ST_IDLE;
				I2C_GenerateSTOP(i2c->i2c, ENABLE);
				i2c->xfer->done = 1;
				i2c_next_xfer(i2c);
			}
		}
		break;
	default:
		break;
	}

}

extern INLINE void i2c_write_isr_evt(i2c_t *RESTRICT const i2c){
	uint32_t const event = I2C_GetLastEvent(i2c->i2c);
	switch(i2c->state){
	case I2C_ST_MASTER_REQ:
		if(i2c_check_evt(event, I2C_EVENT_MASTER_MODE_SELECT)){
			I2C_Send7bitAddress(i2c->i2c, i2c->xfer->devaddr, I2C_Direction_Transmitter);
			i2c->state = I2C_ST_ADDRESSED;
		}
		break;
	case I2C_ST_ADDRESSED:
		if(i2c_check_evt(event, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
			I2C_SendData(i2c->i2c, i2c->xfer->addr);
			i2c->state = I2C_ST_WRITING;
		}
		break;
	case I2C_ST_WRITING:
		if(I2C_GetFlagStatus(i2c->i2c, I2C_FLAG_BTF)){
			I2C_SendData(i2c->i2c, *(i2c->xfer->buffer++));
			if(--i2c->xfer->count == 0){
				I2C_GenerateSTOP(i2c->i2c, ENABLE);
				i2c->state = I2C_ST_CLOSING_WRITE;
			}
		}
		break;
	case I2C_ST_CLOSING_WRITE:
		i2c->xfer->done = 1;
		i2c->state = I2C_ST_IDLE;
		i2c_next_xfer(i2c);
		break;
	default:
		break;
	}
}

extern INLINE void i2c_read_isr_err(i2c_t *RESTRICT const i2c){
	while(1);
}

extern INLINE void i2c_write_isr_err(i2c_t *RESTRICT const i2c){
	uint32_t const event = I2C_GetLastEvent(i2c->i2c);
	switch(i2c->state){
	case I2C_ST_IDLE:
		if((i2c->i2c->SR1 & 0xFF00) != 0){
			i2c->i2c->SR1 &= 0xFF00;
		} else if(event){
			while(1);
		}
		break;
	case I2C_ST_ADDRESSED:
		if(event & I2C_SR1_AF){
			i2c->xfer->done = I2C_XFER_ERR_NOSLAVE;
			i2c->i2c->SR1 = 0;
			i2c_next_xfer(i2c);
		}
		break;
	default:
		if(event)
			while(1);
		break;
	}
}
	
extern INLINE void i2c_isr_evt(i2c_t *RESTRICT const i2c){
	if(!i2c->xfer){
		i2c->i2c->SR1 = 0;
		i2c->i2c->SR2 = 0;
		return;
	}
	switch(i2c->xfer->op){
	case I2C_OP_WRITE:
		i2c_write_isr_evt(i2c);
		break;
	case I2C_OP_READ:
		i2c_read_isr_evt(i2c);
		break;
	default:
		i2c->i2c->SR1 = 0;
		//while(1);
	}
}

extern INLINE void i2c_isr_err(i2c_t *RESTRICT i2c){
	switch(i2c->xfer->op){
	case I2C_OP_WRITE:
		i2c_write_isr_err(i2c);
		break;
	case I2C_OP_READ:
		i2c_read_isr_err(i2c);
		break;
	default:
		while(1);
	}
}

#if USE_I2C1
void I2C1_ER_IRQHandler(void){i2c_isr_err(&i2c1);}
void I2C1_EV_IRQHandler(void){i2c_isr_evt(&i2c1);}
#endif
#if USE_I2C2
void I2C2_ER_IRQHandler(void){i2c_isr_err(&i2c2);}
void I2C2_EV_IRQHandler(void){i2c_isr_evt(&i2c2);}
#endif
