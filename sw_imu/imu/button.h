#ifndef __BUTTON_H_
#define __BUTTON_H_

#include "imu/imu.h"
#include "ch.h"
#include "hal.h"

class Button {
public:
	//! Button handler function type
	typedef void (*button_handler)(uint32_t *);
	typedef enum {ST_PRESSED, ST_RELEASED} state_t;
	typedef enum {ACTIVE_LOW, ACTIVE_HIGH} polarity_t;
	
	Button(GPIO_TypeDef *gpio, expchannel_t channel, polarity_t polarity) :
		channel(channel),
		press_handler(nullptr),
		release_handler(nullptr),
		gpio(*gpio),
		mask(BIT(channel)),
		polarity(polarity)
	{
		buttons[channel] = this;
		state = check_gpio();
	}
	
	void set_press_handler(button_handler new_handler){
		press_handler = new_handler;
	}
	
	void set_release_handler(button_handler new_handler){
		release_handler = new_handler;
	}
	
	void set_callback_param(uint32_t *new_param){
		param = new_param;
	}
	
	void enable(){
		extChannelEnable(&driver, channel);
	}
	
	void disable(){
		extChannelDisable(&driver, channel);
	}
	
	//! The number of channels available for use
	static const size_t num_channels = EXT_MAX_CHANNELS;
	
	state_t check_gpio() const {
		switch(polarity){
		case ACTIVE_LOW:
			return (0 == (gpio.IDR & mask)) ? ST_PRESSED : ST_RELEASED;
		case ACTIVE_HIGH:
			return (0 != (gpio.IDR & mask)) ? ST_PRESSED : ST_RELEASED;
		}
	}
	
	bool is_pressed() const {return state == ST_PRESSED;}
	
	
	/*!
	 @brief Static callback from EXT driver
	 */
	static void callback(EXTDriver *, expchannel_t channel);
	
protected:
	expchannel_t channel;
	
	static EXTDriver &driver;
	
	//! @name Event handlers
	//! @{
	button_handler press_handler;
	button_handler release_handler;
	uint32_t *param;
	//! @}
	
	state_t state;
	
	GPIO_TypeDef &gpio;
	const uint32_t mask;
	
	void handle_callback();
	
	const polarity_t polarity;
	
	//! Array of all buttons
	static Button *buttons[num_channels];
	
	/*!
	 @brief EXT driver configuration to apply on initialization
	 @note Platform configuration
	 */
	static const EXTConfig extcfg;
};

#endif // __BUTTON_H_
