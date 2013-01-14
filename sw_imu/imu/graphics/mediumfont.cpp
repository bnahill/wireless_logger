#include "graphics/mediumfont.h"

bool MediumFont::write_text(uint8_t * buff, uint8_t line, char const * text,
	                        uint32_t &n_cols, uint32_t max_cols){
	if(line >= n_lines)
		return false;
	
	uint8_t * const buff_orig = buff;
	
	uint8_t const * char_ptr;
	uint32_t i;
	char letter;
	
	max_cols -= char_width + letter_spacing;

	
	// Iterate over each character of text
	while(true){
		if((text[0] < 32) || (text[0] > 127))
			letter = ' ' - 32;
		else
			letter = text[0]-32;
		// First column of letter in font
		char_ptr = &font[(uint8_t)letter][line][0];
		
		// Copy letter data
		for(i = 0; i < char_width; i++){
 			buff[i] = char_ptr[i];
		}
		
		buff += char_width;
		
		
		// Check for exit before adding spacing
		if(*(++text) == 0)
			break;
		
		if((uint32_t)(buff - buff_orig) > max_cols)
			break;
		
		// Add spacing
		for(i = 0; i < letter_spacing; i++){
 			buff[i] = 0;
		}
		
		buff += letter_spacing;
	}
	
	n_cols = buff - buff_orig;
	return true;
}

uint32_t MediumFont::get_num_cols(const char * text){
	uint8_t ret = 0;
	while(true){
		ret += char_width;
		if(*(++text) == 0)
			break;
		ret += letter_spacing;
	}
	return ret;
}

uint8_t const MediumFont::font[num_letters][n_lines][char_width] = {
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0xFC,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x1B,0x1B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x1C,0x1C,0x00,0x1C,0x1C,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x60,0x60,0xF0,0x60,0x60,0xF0,0x60,0x60,0x00,0x00,},
	{0x04,0x04,0x1F,0x04,0x04,0x1F,0x04,0x04,0x00,0x00,},
	},
	{{0x70,0x70,0x8C,0xFF,0xFF,0x8C,0x10,0x10,0x00,0x00,},
	{0x04,0x04,0x18,0x7F,0x7F,0x18,0x07,0x07,0x00,0x00,},
	},
	{{0x10,0x10,0x6C,0x10,0x10,0x80,0x70,0x70,0x0F,0x80,},
	{0x00,0x00,0x00,0x1C,0x1C,0x03,0x00,0x00,0x03,0x04,},
	},
	{{0x70,0x70,0x8C,0x8C,0x8C,0x70,0x00,0x00,0x00,0x00,},
	{0x1F,0x1F,0x18,0x1B,0x1B,0x04,0x1B,0x1B,0x18,0x00,},
	},
	{{0x1C,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0xF0,0xF0,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x07,0x07,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x0C,0x0C,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x18,0x18,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x6C,0x6C,0xF0,0x60,0x60,0xF0,0x6C,0x6C,0x00,0x00,},
	{0x03,0x03,0x00,0x00,0x00,0x00,0x03,0x03,0x00,0x00,},
	},
	{{0x80,0x80,0x80,0xF0,0xF0,0x80,0x80,0x80,0x00,0x00,},
	{0x00,0x00,0x00,0x07,0x07,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x60,0x60,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0xE0,0xE0,0x1C,0x00,0x00,0x00,0x00,},
	{0x78,0x78,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0xF0,0xF0,0x0C,0x0C,0x0C,0x0C,0xF0,0xF0,0x00,0x00,},
	{0x07,0x07,0x18,0x18,0x18,0x18,0x07,0x07,0x00,0x00,},
	},
	{{0x10,0x10,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x18,0x18,0x1F,0x18,0x18,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x10,0x10,0x0C,0x8C,0x8C,0x8C,0x70,0x70,0x00,0x00,},
	{0x1C,0x1C,0x1B,0x18,0x18,0x18,0x18,0x18,0x00,0x00,},
	},
	{{0x10,0x10,0x0C,0x8C,0x8C,0x8C,0x70,0x70,0x00,0x00,},
	{0x04,0x04,0x18,0x18,0x18,0x18,0x07,0x07,0x00,0x00,},
	},
	{{0x80,0x80,0x60,0x10,0x10,0xFC,0x00,0x00,0x00,0x00,},
	{0x03,0x03,0x03,0x03,0x03,0x1F,0x03,0x03,0x00,0x00,},
	},
	{{0xFC,0xFC,0x8C,0x8C,0x8C,0x0C,0x00,0x00,0x00,0x00,},
	{0x18,0x18,0x18,0x18,0x18,0x07,0x00,0x00,0x00,0x00,},
	},
	{{0xF0,0xF0,0x8C,0x8C,0x8C,0x8C,0x10,0x10,0x00,0x00,},
	{0x07,0x07,0x18,0x18,0x18,0x18,0x07,0x07,0x00,0x00,},
	},
	{{0x0C,0x0C,0x0C,0x8C,0x8C,0x6C,0x1C,0x1C,0x00,0x00,},
	{0x00,0x00,0x1C,0x03,0x03,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x70,0x70,0x8C,0x8C,0x8C,0x8C,0x70,0x70,0x00,0x00,},
	{0x07,0x07,0x18,0x18,0x18,0x18,0x07,0x07,0x00,0x00,},
	},
	{{0x70,0x70,0x8C,0x8C,0x8C,0x8C,0xF0,0xF0,0x00,0x00,},
	{0x00,0x00,0x18,0x18,0x18,0x04,0x03,0x03,0x00,0x00,},
	},
	{{0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x18,0x18,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x80,0x80,0x60,0x10,0x10,0x0C,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x03,0x04,0x04,0x18,0x00,0x00,0x00,0x00,},
	},
	{{0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x00,0x00,},
	{0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x00,},
	},
	{{0x0C,0x0C,0x10,0x60,0x60,0x80,0x00,0x00,0x00,0x00,},
	{0x18,0x18,0x04,0x03,0x03,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x10,0x10,0x0C,0x8C,0x8C,0x6C,0x10,0x10,0x00,0x00,},
	{0x00,0x00,0x00,0x1B,0x1B,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0xE0,0xE0,0x10,0x10,0x10,0x90,0x90,0x90,0x10,0xE0,},
	{0x07,0x07,0x18,0x07,0x07,0x04,0x1F,0x1F,0x18,0x1F,},
	},
	{{0xF0,0xF0,0x8C,0x8C,0x8C,0x8C,0xF0,0xF0,0x00,0x00,},
	{0x1F,0x1F,0x00,0x00,0x00,0x00,0x1F,0x1F,0x00,0x00,},
	},
	{{0xFC,0xFC,0x8C,0x8C,0x8C,0x8C,0x70,0x70,0x00,0x00,},
	{0x1F,0x1F,0x18,0x18,0x18,0x18,0x07,0x07,0x00,0x00,},
	},
	{{0xF0,0xF0,0x0C,0x0C,0x0C,0x0C,0x10,0x10,0x00,0x00,},
	{0x07,0x07,0x18,0x18,0x18,0x18,0x04,0x04,0x00,0x00,},
	},
	{{0xFC,0xFC,0x0C,0x0C,0x0C,0x0C,0xF0,0xF0,0x00,0x00,},
	{0x1F,0x1F,0x18,0x18,0x18,0x18,0x07,0x07,0x00,0x00,},
	},
	{{0xFC,0xFC,0x8C,0x8C,0x8C,0x8C,0x0C,0x0C,0x00,0x00,},
	{0x1F,0x1F,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,},
	},
	{{0xFC,0xFC,0x8C,0x8C,0x8C,0x8C,0x0C,0x0C,0x00,0x00,},
	{0x1F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0xF0,0xF0,0x0C,0x0C,0x0C,0x8C,0x90,0x90,0x00,0x00,},
	{0x07,0x07,0x18,0x18,0x18,0x18,0x07,0x07,0x00,0x00,},
	},
	{{0xFC,0xFC,0x80,0x80,0x80,0x80,0xFC,0xFC,0x00,0x00,},
	{0x1F,0x1F,0x00,0x00,0x00,0x00,0x1F,0x1F,0x00,0x00,},
	},
	{{0x0C,0x0C,0x0C,0xFC,0xFC,0x0C,0x0C,0x0C,0x00,0x00,},
	{0x18,0x18,0x18,0x1F,0x1F,0x18,0x18,0x18,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFC,0x00,0x00,},
	{0x07,0x07,0x18,0x18,0x18,0x18,0x07,0x07,0x00,0x00,},
	},
	{{0xFC,0xFC,0x80,0x60,0x60,0x10,0x0C,0x0C,0x00,0x00,},
	{0x1F,0x1F,0x00,0x03,0x03,0x04,0x18,0x18,0x00,0x00,},
	},
	{{0xFC,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x1F,0x1F,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,},
	},
	{{0xFC,0xFC,0x10,0x60,0x60,0x10,0xFC,0xFC,0x00,0x00,},
	{0x1F,0x1F,0x00,0x00,0x00,0x00,0x1F,0x1F,0x00,0x00,},
	},
	{{0xFC,0xFC,0x60,0x80,0x80,0x00,0xFC,0xFC,0x00,0x00,},
	{0x1F,0x1F,0x00,0x00,0x00,0x03,0x1F,0x1F,0x00,0x00,},
	},
	{{0xF0,0xF0,0x0C,0x0C,0x0C,0x0C,0xF0,0xF0,0x00,0x00,},
	{0x07,0x07,0x18,0x18,0x18,0x18,0x07,0x07,0x00,0x00,},
	},
	{{0xFC,0xFC,0x8C,0x8C,0x8C,0x8C,0x70,0x70,0x00,0x00,},
	{0x1F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0xF0,0xF0,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0xF0,0x00,},
	{0x03,0x03,0x04,0x04,0x04,0x04,0x1C,0x1C,0x1B,0x00,},
	},
	{{0xFC,0xFC,0x8C,0x8C,0x8C,0x8C,0x70,0x70,0x00,0x00,},
	{0x1F,0x1F,0x00,0x03,0x03,0x04,0x18,0x18,0x00,0x00,},
	},
	{{0x70,0x70,0x8C,0x8C,0x8C,0x8C,0x10,0x10,0x00,0x00,},
	{0x04,0x04,0x18,0x18,0x18,0x18,0x07,0x07,0x00,0x00,},
	},
	{{0x0C,0x0C,0x0C,0xFC,0xFC,0x0C,0x0C,0x0C,0x00,0x00,},
	{0x00,0x00,0x00,0x1F,0x1F,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0xFC,0xFC,0x00,0x00,0x00,0x00,0xFC,0xFC,0x00,0x00,},
	{0x07,0x07,0x18,0x18,0x18,0x18,0x07,0x07,0x00,0x00,},
	},
	{{0xFC,0xFC,0x00,0x00,0x00,0x00,0xFC,0xFC,0x00,0x00,},
	{0x03,0x03,0x04,0x18,0x18,0x04,0x03,0x03,0x00,0x00,},
	},
	{{0xFC,0xFC,0x00,0xE0,0xE0,0x00,0xFC,0xFC,0x00,0x00,},
	{0x03,0x03,0x1C,0x03,0x03,0x1C,0x03,0x03,0x00,0x00,},
	},
	{{0x1C,0x1C,0x60,0x80,0x80,0x60,0x1C,0x1C,0x00,0x00,},
	{0x1C,0x1C,0x03,0x00,0x00,0x03,0x1C,0x1C,0x00,0x00,},
	},
	{{0x1C,0x1C,0x60,0x80,0x80,0x60,0x1C,0x1C,0x00,0x00,},
	{0x00,0x00,0x00,0x1F,0x1F,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x0C,0x0C,0x0C,0x8C,0x8C,0x6C,0x1C,0x1C,0x00,0x00,},
	{0x1C,0x1C,0x1B,0x18,0x18,0x18,0x18,0x18,0x00,0x00,},
	},
	{{0xFC,0xFC,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x1F,0x1F,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x1C,0x1C,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x07,0x07,0x78,0x00,0x00,0x00,0x00,},
	},
	{{0x0C,0x0C,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x18,0x18,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x1C,0x1C,0x03,0x1C,0x1C,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,},
	},
	{{0x0C,0x0C,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x80,0x80,0x60,0x60,0x60,0xE0,0x00,0x00,0x00,0x00,},
	{0x07,0x07,0x18,0x18,0x18,0x1F,0x18,0x18,0x00,0x00,},
	},
	{{0xFC,0xFC,0x60,0x60,0x60,0x80,0x00,0x00,0x00,0x00,},
	{0x1F,0x1F,0x18,0x18,0x18,0x07,0x00,0x00,0x00,0x00,},
	},
	{{0x80,0x80,0x60,0x60,0x60,0x60,0x00,0x00,0x00,0x00,},
	{0x07,0x07,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00,},
	},
	{{0x80,0x80,0x60,0x60,0x60,0xFC,0x00,0x00,0x00,0x00,},
	{0x07,0x07,0x18,0x18,0x18,0x1F,0x00,0x00,0x00,0x00,},
	},
	{{0x80,0x80,0x60,0x60,0x60,0x60,0x80,0x80,0x00,0x00,},
	{0x07,0x07,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x00,0x00,},
	},
	{{0x60,0x60,0xFC,0x63,0x63,0x63,0x00,0x00,0x00,0x00,},
	{0x60,0x60,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x80,0x80,0x60,0x60,0x60,0xE0,0x00,0x00,0x00,0x00,},
	{0x67,0x67,0x98,0x98,0x98,0x7F,0x00,0x00,0x00,0x00,},
	},
	{{0xFC,0xFC,0x60,0x60,0x60,0x80,0x00,0x00,0x00,0x00,},
	{0x1F,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00,},
	},
	{{0x60,0x60,0xEC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x18,0x18,0x1F,0x18,0x18,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x60,0x60,0xEC,0x00,0x00,0x00,0x00,},
	{0x60,0x60,0x80,0x80,0x80,0x7F,0x00,0x00,0x00,0x00,},
	},
	{{0xFC,0xFC,0x00,0x80,0x80,0x60,0x00,0x00,0x00,0x00,},
	{0x1F,0x1F,0x03,0x03,0x03,0x1C,0x00,0x00,0x00,0x00,},
	},
	{{0xFC,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x1F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0xE0,0xE0,0x60,0x80,0x80,0x60,0x80,0x80,0x00,0x00,},
	{0x1F,0x1F,0x00,0x1F,0x1F,0x00,0x1F,0x1F,0x00,0x00,},
	},
	{{0xE0,0xE0,0x60,0x60,0x60,0x80,0x00,0x00,0x00,0x00,},
	{0x1F,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00,},
	},
	{{0x80,0x80,0x60,0x60,0x60,0x60,0x80,0x80,0x00,0x00,},
	{0x07,0x07,0x18,0x18,0x18,0x18,0x07,0x07,0x00,0x00,},
	},
	{{0xE0,0xE0,0x60,0x60,0x60,0x80,0x00,0x00,0x00,0x00,},
	{0xFF,0xFF,0x18,0x18,0x18,0x07,0x00,0x00,0x00,0x00,},
	},
	{{0x80,0x80,0x60,0x60,0x60,0x80,0x00,0x00,0x00,0x00,},
	{0x07,0x07,0x18,0x18,0x18,0xFF,0x00,0x00,0x00,0x00,},
	},
	{{0xE0,0xE0,0x80,0x60,0x60,0x60,0x00,0x00,0x00,0x00,},
	{0x1F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x80,0x80,0x60,0x60,0x60,0x60,0x00,0x00,0x00,0x00,},
	{0x18,0x18,0x1B,0x1B,0x1B,0x04,0x00,0x00,0x00,0x00,},
	},
	{{0x60,0x60,0xFC,0x60,0x60,0x60,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x07,0x18,0x18,0x18,0x00,0x00,0x00,0x00,},
	},
	{{0xE0,0xE0,0x00,0x00,0x00,0xE0,0x00,0x00,0x00,0x00,},
	{0x07,0x07,0x18,0x18,0x18,0x1F,0x00,0x00,0x00,0x00,},
	},
	{{0xE0,0xE0,0x00,0x00,0x00,0x00,0xE0,0xE0,0x00,0x00,},
	{0x00,0x00,0x07,0x18,0x18,0x07,0x00,0x00,0x00,0x00,},
	},
	{{0xE0,0xE0,0x00,0x80,0x80,0x00,0xE0,0xE0,0x00,0x00,},
	{0x03,0x03,0x1C,0x03,0x03,0x1C,0x03,0x03,0x00,0x00,},
	},
	{{0x60,0x60,0x80,0x00,0x00,0x80,0x60,0x60,0x00,0x00,},
	{0x18,0x18,0x04,0x03,0x03,0x04,0x18,0x18,0x00,0x00,},
	},
	{{0xE0,0xE0,0x00,0x00,0x00,0xE0,0x00,0x00,0x00,0x00,},
	{0x07,0x07,0x98,0x98,0x98,0x7F,0x00,0x00,0x00,0x00,},
	},
	{{0x60,0x60,0x60,0x60,0x60,0xE0,0x60,0x60,0x00,0x00,},
	{0x18,0x18,0x1C,0x1B,0x1B,0x18,0x18,0x18,0x00,0x00,},
	},
	{{0x00,0x00,0xF0,0x10,0x10,0x00,0x00,0x00,0x00,0x00,},
	{0x03,0x03,0x7F,0x60,0x60,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0xFC,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x1F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x10,0x10,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x60,0x60,0x7F,0x03,0x03,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x80,0x80,0x60,0x80,0x80,0x00,0x80,0x80,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,},
	},
};



