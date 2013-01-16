#ifndef __IMU_COURIER3_H_
#define __IMU_COURIER3_H_

#include "imu/imu.h"
#include "ch.h"
#include "hal.h"

//! @addtogroup IMU
//! @{
//! @addtogroup FrameBuffer
//! @{

/*!
 @brief A font class for a larger full-screen font
 
 On the 4-page OLED display, this can fit 1 lines of text.
 This is based on comic sans.
 */
class Courier3 {
public:
	static bool write_text(uint8_t * buff, uint8_t line, char const * text,
	                       uint32_t &n_cols, uint32_t max_cols = 0 - 1);
	static uint32_t get_num_cols(char const * text);
	
	static constexpr uint32_t n_lines = 3;
	static constexpr uint32_t char_width = 18;
	static constexpr uint32_t letter_spacing = 2;
	static constexpr uint32_t num_letters = 127-32;
	
	static const uint8_t font[num_letters][n_lines][char_width];
};

//! @} @}

#endif // __IMU_COURIER3_H_
