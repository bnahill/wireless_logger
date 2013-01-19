#ifndef __IMU_USB_SERIAL_H_
#define __IMU_USB_SERIAL_H_

#include "ch.h"
#include "hal.h"
#include "usb_cdc.h"
#include "chprintf.h"
#include "imu.h"

//! @addtogroup IMU
//! @{
//! @addtogroup USBSerial
//! @{


/*!
 @brief An abstracted USB CDC device driver
 @tparam usb_driver The ChibiOS USB driver object to use
 */
template <USBDriver &usb_driver>
class USBSerial {
public:
	USBSerial< usb_driver >(gpio_pin_t const &vusb_pin) :
		vusb_pin(vusb_pin)
	{
		sduObjectInit(&driver);
	}
	
	/*!
	 @brief Initialize the hardware
	 */
	void init(){
		sduStart(&driver, &conf);
		
		usbDisconnectBus(conf.usbp);
		chThdSleepMilliseconds(1000);
		usbStart(conf.usbp, &usbcfg);
	}
	
	void stop(){
		usbStop(conf.usbp);
		sduStop(&driver);
	}
	
	bool is_connected() const { return vusb_pin.read(); }
	
	BaseSequentialStream * stream() const {
		return (BaseSequentialStream *)&driver;
	}

protected:
	//! @name Implemtented in driver
	//! @{	
	SerialUSBDriver driver;
	gpio_pin_t const vusb_pin;
	
	USBInEndpointState ep1instate;
	USBOutEndpointState ep1outstate;
	USBInEndpointState ep2instate;
	USBOutEndpointState ep2outstate;
	//! @}
	
	//! @name Implemtented in driver
	//! @{

	static USBDescriptor const * get_descriptor(USBDriver *usbp,
	                                            uint8_t dtype,
	                                            uint8_t dindex,
	                                            uint16_t lang);
	
	static const SerialUSBConfig conf;	
	
	static const USBEndpointConfig ep1config;
	static const USBEndpointConfig ep2config;
	
	static const USBDescriptor vcom_strings[];
	
	static const USBConfig usbcfg;
	
	static void usb_event(USBDriver *usbp, usbevent_t event);
	//! @}
	
	//! @name To implement as template specialization
	//! @{

	//! A reference to this device accessible from static context
	static USBSerial &this_device;
	
	static const uint8_t vcom_device_descriptor_data[18];
	static const USBDescriptor vcom_device_descriptor;
	static const uint8_t vcom_configuration_descriptor_data[];
	static const uint8_t vcom_string0[];
	static const uint8_t vcom_string1[];
	static const uint8_t vcom_string2[];
	static const uint8_t vcom_string3[];
	static const USBDescriptor vcom_configuration_descriptor;
	//! @}
};

#include "imu/usbserial.cpp"

//! @} @}

#endif // __IMU_USB_SERIAL_H_
