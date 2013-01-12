#include "graphics/courier3.h"

bool Courier3::write_text(uint8_t * buff, uint8_t line, char const * text,
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
		char_ptr = &font[letter][line][0];
		
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

uint32_t Courier3::get_num_cols(const char * text){
	uint8_t ret = 0;
	while(true){
		ret += char_width;
		if(*(++text) == 0)
			break;
		ret += letter_spacing;
	}
	return ret;
}


uint8_t const Courier3::font[num_letters][n_lines][char_width] = {
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xF8,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x7F,0x7F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0xF8,0xF8,0xF8,0xF8,0x00,0x00,0xF8,0xF8,0xF8,0xF8,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x0F,0x0F,0x0F,0x0F,0x00,0x00,0x0F,0x0F,0x0F,0x0F,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0xC0,0xC0,0xC0,0xFF,0xFF,0xFF,0xC0,0xC0,0xC0,0xFF,0xFF,0xDF,0xC0,0xC0,0x00,0x00,},
	{0x00,0x00,0xE3,0xE3,0xE3,0xFF,0xFF,0xEF,0xE3,0xE3,0xFB,0xFF,0xFF,0xE3,0xE3,0xE3,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x3E,0x3F,0x3F,0x00,0x00,0x00,0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0xC0,0xE0,0xF0,0x70,0x38,0x3F,0x3F,0x3F,0x38,0xF0,0xF8,0xF8,0x08,0x00,0x00,},
	{0x00,0x00,0x00,0x83,0x87,0xCF,0x0E,0x1C,0x1C,0x1C,0x38,0x38,0xF1,0xF0,0xE0,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x0F,0x0F,0x0F,0x06,0x06,0xFE,0xFE,0xFE,0x06,0x07,0x03,0x01,0x00,0x00,0x00,},
	},
	{{0xF8,0xFE,0x06,0x06,0x06,0xFC,0xE0,0x00,0x00,0x00,0xC0,0xF8,0x1E,0x04,0x00,0x00,0x00,0x00,},
	{0x03,0x07,0x0C,0x0C,0x0C,0x07,0xC0,0xF0,0x3C,0x0F,0x03,0xF8,0xFE,0x06,0x06,0x06,0xFE,0xFC,},
	{0x00,0x00,0x00,0x00,0x1C,0x0F,0x03,0x00,0x00,0x00,0x00,0x03,0x07,0x0C,0x08,0x0C,0x0F,0x07,},
	},
	{{0x00,0x00,0x00,0xE0,0xF0,0xF8,0x38,0x38,0x38,0x78,0x70,0x60,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0xE0,0xF9,0xFF,0x3F,0x1E,0x3C,0x78,0xE0,0xE0,0xFC,0x7C,0x1C,0x1C,0x1C,0x00,0x00,},
	{0x00,0x00,0x03,0x07,0x0F,0x0E,0x0E,0x0E,0x0E,0x07,0x07,0x0F,0x0E,0x0E,0x0E,0x0E,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xF8,0xF8,0xF8,0x78,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x0F,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xF8,0xFE,0x1F,0x07,0x01,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x0F,0x3F,0x7C,0x70,0x40,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x01,0x07,0x1F,0xFE,0xF8,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x40,0x70,0x7C,0x3F,0x0F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x80,0xE0,0xE0,0xE0,0xC0,0xF8,0xF8,0xF8,0xC0,0xC0,0xE0,0xE0,0xC0,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x08,0x1D,0x3F,0x0F,0x07,0x0F,0x3F,0x1F,0x1C,0x10,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xE0,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x0E,0x0E,0x0E,0x0E,0x0E,0xFF,0xFF,0xFF,0x0E,0x0E,0x0E,0x0E,0x0E,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x70,0x7E,0x7F,0x3F,0x0F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x0F,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xF0,0xFC,0x3F,0x0F,0x03,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xF0,0xFC,0x3F,0x0F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x40,0x70,0x7C,0x3F,0x0F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x80,0xE0,0xF0,0xF0,0x78,0x38,0x38,0x78,0xF0,0xF0,0xE0,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0xFF,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,0x80,0xFF,0xFF,0xFF,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x03,0x07,0x07,0x0E,0x0E,0x0E,0x0E,0x07,0x07,0x03,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0xE0,0x60,0x70,0x70,0x30,0xF8,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x0E,0x0E,0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x0E,0x0E,0x00,0x00,},
	},
	{{0x00,0x00,0x30,0x38,0x38,0x18,0x08,0x08,0x08,0x08,0x08,0x10,0xF0,0xE0,0xC0,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xF0,0x78,0x78,0x3C,0x1E,0x9F,0x8F,0x83,0x00,0x00,0x00,},
	{0x00,0x00,0x0E,0x0F,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x30,0x30,0x18,0x18,0x08,0x08,0x08,0x18,0xF0,0xF0,0xE0,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x1C,0x1C,0x1E,0x1F,0xFF,0xF8,0xF0,0x00,0x00,},
	{0x00,0x00,0x00,0x04,0x07,0x07,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x07,0x07,0x03,0x01,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xE0,0x70,0xF8,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0xC0,0xF0,0xF8,0xFE,0xCF,0xC3,0xC1,0xC0,0xFF,0xFF,0xFF,0xC0,0xC0,0xC0,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0xF8,0xF8,0xF8,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x0F,0x1F,0x0F,0x0E,0x0E,0x0E,0x0E,0x0E,0x1C,0xFC,0xF8,0xF0,0x00,0x00,0x00,},
	{0x00,0x00,0x07,0x07,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x07,0x07,0x03,0x01,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x80,0xC0,0xE0,0x70,0x30,0x30,0x18,0x18,0x18,0x18,0x18,0x10,0x00,0x00,0x00,},
	{0x00,0x00,0x18,0xFF,0xFF,0xFF,0x1C,0x0E,0x0E,0x0E,0x0E,0x1E,0xFC,0xF8,0xF0,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x01,0x03,0x07,0x0F,0x0E,0x0E,0x0E,0x0E,0x0F,0x07,0x03,0x01,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0xF8,0xF8,0xF8,0x38,0x38,0x38,0x38,0x38,0x38,0xF8,0xF8,0xF8,0x38,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xF8,0xFE,0x3F,0x07,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x0F,0x0F,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0xC0,0xE0,0xF0,0x78,0x38,0x38,0x38,0x38,0x78,0xF0,0xE0,0xC0,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0xE3,0xF7,0xFF,0x3E,0x1C,0x1C,0x1C,0x1C,0x3E,0xFF,0xF7,0xE3,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x01,0x07,0x07,0x0F,0x0E,0x0E,0x0E,0x0E,0x0F,0x07,0x07,0x01,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0xC0,0xE0,0xF0,0x38,0x18,0x18,0x18,0x18,0x30,0xF0,0xE0,0xC0,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x07,0x0F,0x1F,0x3C,0x38,0x38,0x38,0x38,0x9C,0xFF,0xFF,0xFF,0x0C,0x00,0x00,},
	{0x00,0x00,0x00,0x02,0x0E,0x0C,0x0C,0x0E,0x0E,0x0E,0x07,0x07,0x03,0x01,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x8F,0x8F,0x8F,0x8F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x0F,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x87,0x87,0x87,0x87,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x70,0x7E,0x7F,0x3F,0x0F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xE0,0x70,0x38,0x30,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x08,0x0C,0x1E,0x3E,0x7F,0xF3,0xE3,0xC1,0x80,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0E,0x06,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x73,0x73,0x73,0x73,0x73,0x73,0x73,0x73,0x73,0x73,0x73,0x73,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x30,0x38,0x70,0xE0,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC1,0xE3,0xF3,0x7F,0x3E,0x1E,0x0C,0x08,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x06,0x0E,0x07,0x03,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0xF0,0xF0,0xF8,0x38,0x38,0x18,0x38,0x38,0x70,0xF0,0xE0,0xC0,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x78,0x78,0x1C,0x1C,0x0F,0x0F,0x03,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x80,0xE0,0xE0,0x30,0x10,0x18,0x88,0xC8,0xC8,0xC8,0xD8,0xD8,0x10,0x30,0xE0,0xC0,0x00,},
	{0x7E,0xFF,0xFF,0x00,0x00,0x7C,0xFF,0xC7,0xC3,0x71,0x7F,0xFF,0xE1,0xE0,0xF0,0x7F,0x3F,0x1F,},
	{0x00,0x01,0x03,0x07,0x06,0x0C,0x0C,0x08,0x08,0x08,0x08,0x0C,0x0C,0x06,0x06,0x06,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x08,0x08,0x88,0xE8,0xF8,0xF8,0x78,0xF0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x80,0xF0,0xFC,0xFF,0x7F,0x73,0x70,0x70,0x73,0x7F,0x7F,0xF8,0xE0,0x80,0x00,0x00,},
	{0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x00,0x00,0x00,0x00,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,},
	},
	{{0x00,0x38,0x38,0xF8,0xF8,0xF8,0x38,0x38,0x38,0x38,0x38,0x38,0x70,0xF0,0xE0,0xC0,0x00,0x00,},
	{0x00,0x00,0x00,0xFF,0xFF,0xFF,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1E,0x3F,0xFF,0xF3,0xE0,0x00,},
	{0x00,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x07,0x03,0x01,0x00,},
	},
	{{0x00,0x00,0xC0,0xE0,0xF0,0x70,0x38,0x18,0x18,0x18,0x18,0x18,0x38,0xF0,0xF8,0xF8,0x18,0x00,},
	{0x00,0x3F,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x83,0x81,0x01,0x00,},
	{0x00,0x00,0x01,0x03,0x07,0x07,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x07,0x07,0x03,0x01,0x00,},
	},
	{{0x00,0x38,0x38,0xF8,0xF8,0xF8,0x38,0x38,0x38,0x38,0x38,0x38,0x70,0xF0,0xE0,0xC0,0x00,0x00,},
	{0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x81,0xFF,0xFF,0x7F,0x00,},
	{0x00,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x07,0x07,0x03,0x01,0x00,0x00,},
	},
	{{0x00,0x00,0x38,0x38,0xF8,0xF8,0xF8,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0xF8,0xF8,0xF8,0x00,},
	{0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x1C,0x1C,0x7F,0x7F,0x7F,0x00,0x00,0xC3,0xC3,0xC3,0x00,},
	{0x00,0x00,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x00,},
	},
	{{0x00,0x00,0x38,0x38,0xF8,0xF8,0xF8,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0xF8,0xF8,0xF8,0x00,},
	{0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x1C,0x1C,0x7F,0x7F,0x7F,0x00,0x00,0x03,0x03,0x03,0x00,},
	{0x00,0x00,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0xC0,0xE0,0xF0,0x70,0x38,0x18,0x18,0x18,0x18,0x18,0x38,0x70,0xF8,0xF8,0x98,0x00,},
	{0x00,0x7F,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0x38,0x38,0x38,0x38,0xF8,0xF9,0xF9,0x39,0x00,},
	{0x00,0x00,0x01,0x03,0x07,0x07,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x07,0x07,0x07,0x00,0x00,},
	},
	{{0x00,0x38,0x38,0xF8,0xF8,0xF8,0x38,0x38,0x00,0x00,0x38,0x38,0xF8,0xF8,0xF8,0x38,0x38,0x00,},
	{0x00,0x00,0x00,0xFF,0xFF,0xFF,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0xFF,0xFF,0xFF,0x00,0x00,0x00,},
	{0x00,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x00,0x00,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x00,},
	},
	{{0x00,0x00,0x38,0x38,0x38,0x38,0x38,0xF8,0xF8,0xF8,0x38,0x38,0x38,0x38,0x38,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x0E,0x0E,0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x0E,0x0E,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x38,0x38,0x38,0x38,0x38,0x38,0xF8,0xF8,0xF8,0x38,0x38,0x38,0x38,},
	{0x00,0xF8,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,},
	{0x00,0x03,0x07,0x07,0x06,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x07,0x07,0x01,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x38,0x38,0xF8,0xF8,0xF8,0x38,0x38,0x00,0x00,0xB8,0xF8,0xF8,0xF8,0x78,0x38,0x38,0x38,},
	{0x00,0x00,0x00,0xFF,0xFF,0xFF,0x1C,0x0E,0x3E,0x7F,0xF3,0xC1,0x80,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x00,0x00,0x01,0x03,0x0F,0x0F,0x0E,0x0E,0x0E,0x0E,},
	},
	{{0x00,0x38,0x38,0x38,0xF8,0xF8,0xF8,0x38,0x38,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xF0,0xF0,0x00,},
	{0x00,0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x00,},
	},
	{{0x18,0x18,0xF8,0xF8,0xF8,0xF8,0xE0,0x00,0x00,0x00,0x00,0xE0,0xF8,0xF8,0xF8,0xF8,0x18,0x18,},
	{0x00,0x00,0xFF,0xFF,0xFF,0x00,0x07,0x3F,0x78,0x78,0x3F,0x07,0x00,0xFF,0xFF,0xFF,0x00,0x00,},
	{0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x00,0x00,0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,},
	},
	{{0x00,0x38,0x38,0xF8,0xF8,0xF8,0xF0,0xC0,0x80,0x00,0x38,0x38,0x38,0xF8,0xF8,0xF8,0x38,0x38,},
	{0x00,0x00,0x00,0xFF,0xFF,0xFF,0x01,0x07,0x1F,0x3E,0xF8,0xE0,0x80,0xFF,0xFF,0xFF,0x00,0x00,},
	{0x00,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x00,0x00,0x03,0x07,0x0F,0x0F,0x0F,0x00,0x00,},
	},
	{{0x00,0x00,0xC0,0xE0,0xF0,0xF0,0x78,0x38,0x38,0x38,0x38,0x78,0xF0,0xF0,0xE0,0xC0,0x00,0x00,},
	{0x00,0x7F,0xFF,0xFF,0x81,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC1,0xFF,0xFF,0x7F,0x00,},
	{0x00,0x00,0x01,0x03,0x07,0x07,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x07,0x07,0x03,0x01,0x00,0x00,},
	},
	{{0x00,0x00,0x38,0x38,0xF8,0xF8,0xF8,0x38,0x38,0x38,0x38,0x38,0x78,0x70,0xF0,0xE0,0xC0,0x00,},
	{0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x38,0x38,0x38,0x38,0x38,0x38,0x1C,0x1F,0x0F,0x07,0x00,},
	{0x00,0x00,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0xC0,0xE0,0xF0,0xF0,0x78,0x38,0x38,0x38,0x38,0x78,0xF0,0xF0,0xE0,0xC0,0x00,},
	{0x00,0x00,0x7F,0xFF,0xFF,0x81,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x81,0xFF,0xFF,0x7F,},
	{0x00,0x00,0x00,0x01,0x63,0xE7,0x7F,0x7E,0x7C,0x7C,0x7C,0xEC,0xEE,0xE7,0xE7,0x73,0x41,0x00,},
	},
	{{0x00,0x38,0x38,0xF8,0xF8,0xF8,0x38,0x38,0x38,0x38,0x38,0x38,0x78,0xF0,0xF0,0xC0,0x00,0x00,},
	{0x00,0x00,0x00,0xFF,0xFF,0xFF,0x1C,0x1C,0x1C,0x7C,0xFC,0xFC,0xDC,0x8F,0x07,0x03,0x00,0x00,},
	{0x00,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x00,0x00,0x01,0x07,0x0F,0x0F,0x0E,0x0E,0x0E,},
	},
	{{0x00,0x00,0xE0,0xF0,0xF0,0x38,0x18,0x18,0x18,0x18,0x18,0x38,0xFC,0xF8,0xF8,0x08,0x00,0x00,},
	{0x00,0x40,0xC3,0xC7,0x8F,0x0E,0x0C,0x0C,0x1C,0x18,0x18,0x18,0x19,0xF9,0xF1,0xE0,0x00,0x00,},
	{0x00,0x00,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0F,0x07,0x07,0x01,0x00,0x00,},
	},
	{{0x00,0xF8,0xF8,0xF8,0x38,0x38,0x38,0x38,0xF8,0xF8,0xF8,0x38,0x38,0x38,0xF8,0xF8,0xF8,0x00,},
	{0x00,0x07,0x07,0x07,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x07,0x07,0x07,0x00,},
	{0x00,0x00,0x00,0x00,0x0E,0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x38,0x38,0xF8,0xF8,0xF8,0x38,0x38,0x38,0x00,0x38,0x38,0x38,0xF8,0xF8,0xF8,0x38,0x38,},
	{0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,},
	{0x00,0x00,0x00,0x01,0x03,0x07,0x07,0x0E,0x0E,0x0E,0x0E,0x0E,0x07,0x07,0x03,0x01,0x00,0x00,},
	},
	{{0x38,0x38,0x78,0xF8,0xF8,0xB8,0x38,0x38,0x00,0x00,0x00,0x38,0x38,0xF8,0xF8,0xF8,0x38,0x38,},
	{0x00,0x00,0x00,0x03,0x0F,0x7F,0xFC,0xE0,0x80,0x00,0xC0,0xF8,0x7F,0x1F,0x07,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x0F,0x0F,0x0F,0x0F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x18,0x38,0xF8,0xF8,0x98,0x18,0x18,0x80,0xC0,0xC0,0x80,0x18,0x18,0x98,0xF8,0xF8,0x38,0x18,},
	{0x00,0x00,0x3F,0xFF,0xFF,0xE0,0xFC,0x7F,0x0F,0x0F,0xFF,0xFC,0xE0,0xFF,0xFF,0x1F,0x00,0x00,},
	{0x00,0x00,0x00,0x0F,0x0F,0x0F,0x07,0x00,0x00,0x00,0x00,0x07,0x0F,0x0F,0x0F,0x00,0x00,0x00,},
	},
	{{0x00,0x38,0x38,0x78,0xF8,0xF8,0xF8,0xB8,0x00,0x00,0xB8,0xF8,0xF8,0xF8,0x78,0x38,0x38,0x00,},
	{0x00,0x00,0x00,0x00,0x80,0xC1,0xF3,0x7F,0x3E,0x3E,0x7F,0xF3,0xC1,0x80,0x00,0x00,0x00,0x00,},
	{0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x0F,0x0E,0x00,0x00,0x0E,0x0F,0x0F,0x0F,0x0F,0x0E,0x0E,0x00,},
	},
	{{0x00,0x38,0x38,0x78,0xF8,0xF8,0xF8,0x38,0x00,0x00,0x00,0x38,0xF8,0xF8,0xF8,0x38,0x38,0x38,},
	{0x00,0x00,0x00,0x00,0x00,0x03,0x07,0x1F,0xFC,0xF8,0xFE,0x1F,0x07,0x03,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x0E,0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0xF8,0xF8,0xF8,0x38,0x38,0x38,0x38,0x38,0xB8,0xF8,0xF8,0x78,0x38,0x00,0x00,0x00,},
	{0x00,0x00,0x03,0x03,0x83,0xE0,0xF0,0x78,0x3C,0x0F,0x07,0x03,0x01,0xE0,0xE0,0xE0,0x00,0x00,},
	{0x00,0x00,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x07,0x07,0x07,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x7F,0x7F,0x70,0x70,0x70,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x01,0x07,0x1F,0x7E,0xF8,0xE0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x07,0x1F,0xFE,0xF8,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x0F,0x3F,0xFC,0xF0,0x40,0x00,0x00,},
	},
	{{0x00,0x00,0x07,0x07,0x07,0x07,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x70,0x70,0x70,0x70,0x7F,0x7F,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0xC0,0xE0,0xF8,0x78,0x78,0xF8,0xE0,0xC0,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x01,0x01,0x03,0x07,0x07,0x0E,0x1C,0x18,0x18,0x10,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00,},
	{0x00,0xC0,0xE0,0xF1,0x73,0x3B,0x39,0x39,0x39,0x39,0x39,0x39,0xFF,0xFF,0xFF,0x00,0x00,0x00,},
	{0x00,0x03,0x07,0x0F,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0F,0x07,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,},
	},
	{{0x06,0x06,0x06,0xFE,0xFE,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0xFF,0xFF,0xFF,0x03,0x01,0x01,0x01,0x01,0x01,0x03,0x87,0xFF,0xFE,0xFC,0x00,},
	{0x0E,0x0E,0x0E,0x0F,0x0F,0x07,0x07,0x0E,0x0E,0x0E,0x0E,0x0E,0x0F,0x07,0x07,0x03,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0xC0,0xC0,0x40,0x00,},
	{0x00,0x00,0xFC,0xFF,0xFF,0x87,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x07,0x8F,0x8F,0x08,0x00,},
	{0x00,0x00,0x00,0x01,0x03,0x07,0x07,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x07,0x07,0x03,0x01,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xC0,0xC6,0xC6,0x86,0x86,0xFE,0xFE,0xFE,0x00,0x00,},
	{0x00,0xFC,0xFE,0xFF,0x87,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0xFF,0xFF,0xFF,0x00,0x00,},
	{0x00,0x00,0x03,0x07,0x07,0x0F,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x07,0x0F,0x0F,0x0F,0x0E,0x0E,},
	},
	{{0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00,},
	{0x00,0x78,0xFE,0xFF,0xFF,0x73,0x71,0x71,0x71,0x71,0x71,0x71,0x73,0x7F,0xFF,0x7C,0x00,0x00,},
	{0x00,0x00,0x01,0x03,0x07,0x07,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x06,0x07,0x03,0x02,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0xC0,0xC0,0xC0,0xF8,0xFC,0xFE,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x0E,0x00,0x00,},
	{0x00,0x00,0x00,0x03,0x03,0x03,0xFF,0xFF,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,},
	{0x00,0x7C,0xFF,0xFF,0x87,0x01,0x01,0x01,0x01,0x01,0x83,0xC7,0xFF,0xFF,0xFF,0x01,0x01,0x00,},
	{0x00,0x00,0x70,0xE1,0xE3,0xC3,0xC3,0xC7,0xC3,0xC3,0xC3,0xE3,0xFF,0x7F,0x3F,0x00,0x00,0x00,},
	},
	{{0x00,0x06,0x06,0xFE,0xFE,0xFE,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0xFF,0xFF,0xFF,0x03,0x01,0x01,0x01,0x01,0x01,0xFF,0xFF,0xFF,0x00,0x00,0x00,},
	{0x00,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x00,0x00,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0xDF,0xDF,0xDF,0xDF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x0E,0x0E,0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x0E,0x0E,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0xDF,0xDF,0xDF,0xDF,0xC0,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0xF0,0xE0,0xC0,0xC0,0xC0,0xC0,0xC0,0xE0,0xFF,0x7F,0x3F,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x06,0x06,0xFE,0xFE,0xFE,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x00,},
	{0x00,0x00,0x00,0xFF,0xFF,0xFF,0x38,0x7C,0xFC,0xE6,0xC7,0x83,0x01,0x01,0x01,0x01,0x00,0x00,},
	{0x00,0x0E,0x0E,0x0F,0x0F,0x0F,0x00,0x00,0x00,0x0F,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x0E,0x00,},
	},
	{{0x00,0x00,0x00,0x06,0x06,0x06,0x06,0xFE,0xFE,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x0E,0x0E,0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x0E,0x0E,0x00,0x00,0x00,},
	},
	{{0xC0,0xC0,0xC0,0xC0,0x80,0xC0,0xC0,0xC0,0xC0,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00,},
	{0x01,0x01,0xFF,0xFF,0x03,0x01,0x01,0x01,0xFF,0xFF,0xFF,0x01,0x01,0x01,0xFF,0xFF,0xFF,0x00,},
	{0x0E,0x0E,0x0F,0x0F,0x0E,0x0E,0x00,0x00,0x0F,0x0F,0x0F,0x0E,0x0E,0x00,0x0F,0x0F,0x0F,0x0E,},
	},
	{{0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,},
	{0x00,0x01,0x01,0xFF,0xFF,0xFF,0x03,0x01,0x01,0x01,0x01,0x01,0xFF,0xFF,0xFF,0x00,0x00,0x00,},
	{0x00,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x00,0x00,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00,},
	{0x00,0xFC,0xFE,0xFF,0x87,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x87,0xFF,0xFE,0xFC,0x00,},
	{0x00,0x00,0x01,0x03,0x07,0x07,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x07,0x07,0x03,0x01,0x00,0x00,},
	},
	{{0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,},
	{0x00,0x01,0x01,0xFF,0xFF,0xFF,0x03,0x01,0x01,0x01,0x01,0x01,0x03,0x87,0xFF,0xFF,0xFC,0x00,},
	{0xC0,0xC0,0xC0,0xFF,0xFF,0xC7,0xC7,0xCE,0xCE,0xCE,0x0E,0x0E,0x07,0x07,0x03,0x01,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,},
	{0x00,0xFC,0xFF,0xFF,0x87,0x03,0x01,0x01,0x01,0x01,0x01,0x03,0xFF,0xFF,0xFF,0x01,0x01,0x01,},
	{0x00,0x00,0x01,0x03,0x07,0x0E,0x0E,0x0E,0xCE,0xCE,0xCE,0xC7,0xFF,0xFF,0xFF,0xC0,0xC0,0x00,},
	},
	{{0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0x00,},
	{0x00,0x01,0x01,0x01,0x01,0xFF,0xFF,0xFF,0x07,0x03,0x03,0x01,0x01,0x01,0x01,0x00,0x00,0x00,},
	{0x00,0x0E,0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x0E,0x0E,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xA0,0xC0,0xC0,0x40,0x00,0x00,},
	{0x00,0x00,0x80,0x9F,0xFF,0x3F,0x79,0x71,0x71,0x71,0x71,0x71,0x73,0xEF,0xE7,0xC4,0x00,0x00,},
	{0x00,0x00,0x08,0x0F,0x0F,0x07,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x07,0x07,0x03,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0xC0,0xC0,0xF8,0xF8,0xF8,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x03,0x03,0xFF,0xFF,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x80,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x03,0x07,0x07,0x0E,0x0E,0x0E,0x0E,0x0E,0x06,0x07,0x03,0x03,0x00,},
	},
	{{0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0x00,},
	{0x00,0x01,0x01,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x01,0x01,0xFF,0xFF,0xFF,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x03,0x07,0x0F,0x0F,0x0E,0x0E,0x0E,0x0E,0x07,0x0F,0x0F,0x0F,0x0E,0x0E,0x00,},
	},
	{{0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,},
	{0x01,0x01,0x01,0x07,0x1F,0x7F,0xFD,0xF1,0x80,0x80,0xE0,0xF9,0x7F,0x1F,0x07,0x01,0x01,0x01,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x07,0x0F,0x0F,0x0F,0x01,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,},
	{0x01,0x01,0x1F,0xFF,0xFF,0xE1,0xE0,0xFC,0xFF,0x7F,0xFE,0xF0,0xE1,0xFD,0xFF,0x1F,0x03,0x01,},
	{0x00,0x00,0x00,0x00,0x07,0x0F,0x0F,0x07,0x00,0x00,0x03,0x0F,0x0F,0x0F,0x01,0x00,0x00,0x00,},
	},
	{{0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,},
	{0x00,0x01,0x01,0x01,0x83,0xC7,0xEF,0xFF,0x7C,0x7C,0xFF,0xEF,0xC7,0x83,0x01,0x01,0x01,0x00,},
	{0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x0F,0x0E,0x00,0x00,0x0E,0x0F,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,},
	},
	{{0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,},
	{0x01,0x01,0x01,0x07,0x1F,0x7F,0xF9,0xE0,0xC0,0xC0,0xF0,0xFD,0x3F,0x0F,0x03,0x01,0x01,0x00,},
	{0x00,0xE0,0xE0,0xE0,0xE0,0xE0,0xF1,0xFF,0x3F,0x0F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x07,0x07,0x87,0xC1,0xF1,0x79,0x3D,0x1F,0x0F,0x87,0x83,0x81,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x0E,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0xFE,0xFE,0xCF,0x07,0x07,0x07,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x1C,0x3C,0xFF,0xFF,0xF7,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x3F,0x3F,0x79,0x70,0x70,0x70,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x07,0x07,0x07,0xCF,0xFE,0xFE,0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0xF7,0xFF,0xFF,0x3C,0x1C,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x00,0x70,0x70,0x70,0x79,0x3F,0x3F,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	{0x00,0x00,0x10,0x7C,0x3E,0x1E,0x0E,0x0E,0x1E,0x1C,0x3C,0x38,0x3C,0x3F,0x1E,0x04,0x00,0x00,},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	},
};
