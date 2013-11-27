/*!
 @file imu_docs.h
 @brief Doxygen-compatible high-level documentation
 @author Ben Nahill <bnahill@gmail.com>
 */

/*!
@defgroup IMU IMU Support Library
@brief Library of reusable and parameterized blocks for an IMU and more
@{

A set of general purpose drivers buit around ChibiOS for various sensors and
peripherals commonly used in an IMU. They are designed for the STM32F4 but 
are intended to be as generic as possible and should work unaltered for any
other device supported by ChibiOS and its HAL.

Also included are some support types and useful arithmetic operations for
IMU operations.

These devices must be instantiated for your specific configuration in the 
platform configuration.


@defgroup Util Utilities
@{
	
@}

@defgroup RF RF Drivers
@{

@}


@defgroup CGM Continuous Glucose Monitor Interface
@{

@}

@defgroup DataAcquisition Acquisition Framework
@{
Sensor data is provided through the acquisition framework through queues. Each
sensor is managed in its own thread in the \ref Acquisition class. Sensors can
be individually enabled and disabled through public functions. While acquisition
threads are running, they present \ref DataSource instances, to which an
application may register \ref DataListener instances. These will safely receive
copies of samples with accurate timing. DataListeners may create a queue as
well to allow for increased safety for low-priority tasks.
@}

@defgroup Sensors Sensor Drivers
@{

@defgroup MMA845x Freescale MMA845x Accelerometer
@{
The MMA845x is a digital accelerometer family connected over an I2C interface.
This driver uses the IMU I2C driver to interface with it. Both the MMA8452Q and
MMA8451Q variants are supported.
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
