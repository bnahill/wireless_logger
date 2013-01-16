/*!

@defgroup Platform Platform Configuration
@{
Drivers in this framework are intended to be as generic as possible without
sacrificing performance. To apply these drivers to a project, each device
must be instantiated with the platform-specific configuration to make it work
properly.

The Platform namespace (in platform.h and platform.cpp) contains such
instantiations. That is the interface between your application and the device 
drivers. One item to note is that the bus drivers don't have pin associations.
These are to be defined in ChibiOS's board.h. This allows for a very quick
pin setup operation on reset and simplifies the drivers.

When enabling a peripheral that uses the HAL from ChibiOS, it is also
important to enable this device in ChibiOS's halconf.h.

@defgroup ChibiOS ChibiOS Configuration
@{
@defgroup Kernel Kernel
@defgroup MCU MCU Configuration
@defgroup Board Board Configuration
@defgroup HAL Hardware Abstraction Layer
@}



@}

*/