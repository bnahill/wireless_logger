/*!
@defgroup UI
@{

@defgroup Screens
@brief Screens for the OLED display
@{

@}




@defgroup USB
@{

@defgroup USBTerm USB Terminal
@brief A human- and machine-friendly multi-purpose USB serial terminal

Defines a verbose USB terminal intended for an intelligent (or human) client
to be able to retrieve data and configure the device. As this is intended to
be a device in constant development, this device produces extra information
defining log versions, buffer types, introspection, and command parameter
marshalling.


@par Commands from the PC follow the following format:

1-byte packet length | 0-terminated command root in ASCII | argument list

String arguments are 0-terminated

Integers and floating point values are represented in their native
binary representation in little-endian byte order (default for STM32).

Array types are dealt with by first passing a 4-byte integer containing the
number of array elements present.

@par Return Codes:

Return values come in a similar format but with a 1-byte unsigned return code
at the end. By convention, this return code is 0 in the case of no error.
Also by convention, a value of 255 indicates that more data exists.

@defgroup USBFile USB File Access
@brief Virtual file management for USB access

The devices provides many means of logging various forms of data. This unit
provides a number of facilities for abstract data access over USB.


@}
@}
*/