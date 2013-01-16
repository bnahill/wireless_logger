#ifndef __IMU_SMALLFONT_H_
#define __IMU_SMALLFONT_H_

#include "imu/imu.h"
#include "ch.h"
#include "hal.h"

//! @addtogroup IMU
//! @{
//! @addtogroup FrameBuffer
//! @{

/*!
 @brief A font class for a small single-page font
 
 On the 4-page OLED display, this can fit 4 lines of text.
 */
class SmallFont {
public:
	static bool write_text(uint8_t * buff, uint8_t line, char const * text,
	                       uint32_t &n_cols, uint32_t max_cols = 0 - 1);
	static uint32_t get_num_cols(char const * text);
	
	static constexpr uint32_t n_lines = 1;
	static constexpr uint32_t char_width = 5;
	static constexpr uint32_t letter_spacing = 1;
	
	static const uint8_t font[];
};

//! @} @}

#endif // __IMU_SMALLFONT_H_
