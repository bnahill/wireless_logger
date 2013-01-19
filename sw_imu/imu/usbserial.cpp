#include "usbserial.h"
#include "platform.h"

template <USBDriver &usb_driver>
void USBSerial< usb_driver >::usb_event(USBDriver *usbp, usbevent_t event) {
	switch (event) {
	case USB_EVENT_RESET:
		return;
	case USB_EVENT_ADDRESS:
		return;
	case USB_EVENT_CONFIGURED:
		chSysLockFromIsr();

		/* Enables the endpoints specified into the configuration.
		 Note, this callback is invoked from an ISR so I-Class functions
		 must be used.*/
		usbInitEndpointI(usbp, USB_CDC_DATA_REQUEST_EP, &this_device.ep1config);
		usbInitEndpointI(usbp, USB_CDC_INTERRUPT_REQUEST_EP, &this_device.ep2config);

		/* Resetting the state of the CDC subsystem.*/
		sduConfigureHookI(usbp);

		chSysUnlockFromIsr();
		return;
	case USB_EVENT_SUSPEND:
		return;
	case USB_EVENT_WAKEUP:
		return;
	case USB_EVENT_STALLED:
		return;
	}
	return;
}


template <USBDriver &usb_driver>
USBDescriptor const *USBSerial< usb_driver >::get_descriptor(USBDriver *usbp,
	                                                         uint8_t dtype,
	                                                         uint8_t dindex,
	                                                         uint16_t lang) {
	(void)usbp;
	(void)lang;
	switch (dtype) {
	case USB_DESCRIPTOR_DEVICE:
		return &this_device.vcom_device_descriptor;
	case USB_DESCRIPTOR_CONFIGURATION:
		return &this_device.vcom_configuration_descriptor;
	case USB_DESCRIPTOR_STRING:
		if (dindex < 4)
			return &USBSerial::vcom_strings[dindex];
	}
	return nullptr;
}

template <USBDriver &usb_driver>
const SerialUSBConfig USBSerial< usb_driver >::conf = {
	&usb_driver
};

template <USBDriver &usb_driver>
const USBEndpointConfig USBSerial< usb_driver >::ep1config = {
	USB_EP_MODE_TYPE_BULK,
	NULL,
	sduDataTransmitted,
	sduDataReceived,
	0x0040,
	0x0040,
	&this_device.ep1instate,
	&this_device.ep1outstate,
	2,
	NULL
};

template <USBDriver &usb_driver>
const USBEndpointConfig USBSerial< usb_driver >::ep2config = {
	USB_EP_MODE_TYPE_INTR,
	NULL,
	sduInterruptTransmitted,
	NULL,
	0x0010,
	0x0000,
	&this_device.ep2instate,
	NULL,
	1,
	NULL
};

template <USBDriver &usb_driver>
const USBConfig USBSerial< usb_driver >::usbcfg = {
	usb_event,
	get_descriptor,
	sduRequestsHook,
	NULL
};

