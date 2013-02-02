/*!
@defgroup IMU IMU Support
@{
A set of general purpose drivers buit around ChibiOS for various sensors and
peripherals commonly used in an IMU. They are designed for the STM32F4 but 
are intended to be as generic as possible and should work unaltered for any
other device supported by ChibiOS and its HAL.

Also included are some support types and useful arithmetic operations for
IMU operations.

These devices must be instantiated for your specific configuration in the 
platform configuration.


@defgroup RF RF Drivers
@{

@}


@defgroup Sensors Sensor Drivers
@{

@defgroup MMA8452Q Freescale MMA8452Q Accelerometer
@{
The MMA8452Q is a digital accelerometer connected over an I2C interface. This
driver uses the IMU I2C driver to interface with it.
@}

@defgroup L3GD20 ST L3GD20 Gyroscope



@defgroup MAG3110 Freescale MAG3110 Magnetometer
@{
The MAG3110 is a digital magnetometer connected over an I2C interface. This
driver uses the IMU I2C driver to interface with it.
@}


@}



@}
*/
