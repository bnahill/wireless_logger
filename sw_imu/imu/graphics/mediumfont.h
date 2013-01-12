#ifndef __IMU_MEDIUMFONT_H_
#define __IMU_MEDIUMFONT_H_

#include "imu/imu.h"
#include "ch.h"
#include "hal.h"

/*!
 @brief A font class for a larger double-page font
 
 On the 4-page OLED display, this can fit 2 lines of text.
 */
class MediumFont {
public:
	static bool write_text(uint8_t * buff, uint8_t line, char const * text,
	                       uint32_t &n_cols, uint32_t max_cols = 0 - 1);
	static uint32_t get_num_cols(char const * text);
	
	static constexpr uint32_t n_lines = 2;
	static constexpr uint32_t char_width = 10;
	static constexpr uint32_t letter_spacing = 1;
	static constexpr uint32_t num_letters = 127-32;
	
	static const uint8_t font[num_letters][n_lines][char_width];
};

#endif // __IMU_MEDIUMFONT_H_
