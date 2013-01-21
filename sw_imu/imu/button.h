#ifndef __BUTTON_H_
#define __BUTTON_H_

#include "imu/imu.h"
#include "ch.h"
#include "hal.h"

//! @addtogroup IMU
//! @{
//! @addtogroup Button
//! @{

/*!
 @brief A class for a button associated with a GPIO pin and external interrupt
 */
template <EXTDriver &driver>
class Button {
public:
	//! Button handler function type
	typedef void (*button_handler)(uint32_t *);
	//! Current button state type
	typedef enum {ST_PRESSED, ST_RELEASED} state_t;
	//! Button polarity configuration type
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
	
	/*!
	 @brief Assign a new handler to the button's press action
	 @param new_handler The new button handler function
	 @param param A pointer that will be passed to that function
	 */
	void set_press_handler(button_handler new_handler, uint32_t *param = nullptr){
		press_handler = new_handler;
		press_param = param;
	}
	
	/*!
	 @brief Assign a new handler to the button's release action
	 @param new_handler The new button handler function
	 @param param A pointer that will be passed to that function
	 */
	void set_release_handler(button_handler new_handler, uint32_t *param = nullptr){
		release_handler = new_handler;
		release_param = param;
	}
	
	//! Enable the interrupt for this button
	void enable(){
		extChannelEnable(&driver, channel);
	}
	
	//! Disable the interrupt for this button
	void disable(){
		extChannelDisable(&driver, channel);
	}
	
	//! Clear both callback functions
	void clear_callbacks(){
		press_handler = nullptr;
		release_handler = nullptr;
	}
	
	//! The number of channels available for use
	static const size_t num_channels = EXT_MAX_CHANNELS;
	
	//! Read the current state of the GPIO pin
	state_t check_gpio() const {
		switch(polarity){
		case ACTIVE_LOW:
			return (0 == (gpio.IDR & mask)) ? ST_PRESSED : ST_RELEASED;
		case ACTIVE_HIGH:
			return (0 != (gpio.IDR & mask)) ? ST_PRESSED : ST_RELEASED;
		default:
			return ST_RELEASED;
		}
	}
	
	bool is_pressed() const {return state == ST_PRESSED;}
	
	
	/*!
	 @brief Static callback from EXT driver
	 */
	static void callback(EXTDriver *, expchannel_t channel);
	
protected:
	expchannel_t channel;
	
	//! @name Event handlers
	//! @{
	button_handler press_handler;
	button_handler release_handler;
	uint32_t *press_param;
	uint32_t *release_param;
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

//! @} // Button
//! @} // IMU

#include "button.cpp"

#endif // __BUTTON_H_
