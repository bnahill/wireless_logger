/*!
 @file framebuffer.h
 @brief Class declaration for \ref FrameBuffer class
 @author Ben Nahill <bnahill@gmail.com>
 */

#ifndef __IMU_FRAMEBUFFER_H_
#define __IMU_FRAMEBUFFER_H_

#include "imu/imu.h"
#include "imu/imu_math.h"
#include "ch.h"
#include "hal.h"

#include "graphics/smallfont.h"
#include "graphics/mediumfont.h"
#include "graphics/comicsans4.h"
#include "graphics/courier3.h"

//! @addtogroup IMU
//! @{
//! @addtogroup FrameBuffer
//! @{

/*!
 @brief A framebuffer for OLED or LCD displays where the data is stored in
 pages of 8 bits (pixels)
 @tparam pages The number of pages in the display
 @tparam columns The number of columns in the display
 */
template <uint32_t pages, uint32_t columns>
class FrameBuffer {
public:
	FrameBuffer< pages, columns >();
	
	/*!
	 @brief Limits to a region in the framebuffer
	 */
	typedef struct {
		uint32_t x_min;
		uint32_t x_max;
		uint32_t y_min;
		uint32_t y_max;
	} limits_t;
	
	/*!
	 @brief Clear an area (write zeros) on the screen
	 */
	void clear_area(uint32_t start_page = 0, uint32_t end_page = pages - 1,
	                uint32_t start_column = 0, uint32_t end_column = columns - 1);
	/*!
	 @brief Clear the entire screen
	 */
	void clear();
	
	/*!
	 @brief Write text on the framebuffer
	 @tparam font_class The class of font to write with
	 @param text The text to write
	 @param page The page to start on
	 @param column The column to start on
	 @param max_end_column A column to limit print length
	 
	 The height of the result depends on the font. It is the responsibility of
	 the caller to make sure that this doesn't overflow the buffer.
	 */
	template<class font_class>
	uint32_t write_text(char const * text, uint8_t page, uint8_t column,
	                uint32_t max_end_column = columns){
		return write_text_generic(font_class::write_text, text, page,
		                          column, max_end_column);
	}
	
	/*!
	 @brief Write text on the framebuffer centered at a given point
	 @tparam font_class The class of font to write with
	 @param text The text to write
	 @param page The page to start on
	 @param center_column The column of the center of the text
	 @param max_width The maximum acceptable width of the text. It will be
	 truncated after this.
	 
	 The height of the result depends on the font. It is the responsibility of
	 the caller to make sure that this doesn't overflow the buffer.
	 */
	template<class font_class>
	void write_text_centered(char const * text, uint8_t page,
	                         uint8_t center_column = columns/2,
	                         uint32_t max_width = columns){
		uint32_t width, start;
		do {
			width = font_class::get_num_cols(text);
			start = max((int)(center_column - (width / 2)), (int)0);
			write_text_generic(font_class::write_text, text, page, start,
			                   max(start + max_width, columns));
			// Iterate to one character past the end of the string
			while(*(text++) >= ' ');
			page += font_class::n_lines;
		} while (*(text-1) != '\0');
	}
	
	
	/*!
	 @brief Draw a horizontally with a bit mask
	 @param page The page to mask
	 @param mask The mask to use
	 @param col_start The first column to apply this to
	 @param length The number of columns to write
	 */
	void draw_horizontal_mask(uint8_t page, uint8_t mask,
	                          uint32_t col_start, uint32_t length);
	
	/*!
	 @brief Draw a vertical line across pages
	 @param start_page The top of the line
	 @param end_page The botton of the line
	 @param column The column in which to draw the line
	 */
	void draw_vertical(uint8_t start_page, uint8_t end_page, uint32_t column);
	
	/*!
	 @brief Draw a progress bar-like graphic
	 @param value The value from 0 to 1 to display
	 @param page The page to draw it in
	 @param start_column The position of the left end of the bar
	 @param end_column The position of the right end of the bar
	 */
	void draw_progress_bar(float value, uint8_t page,
	                       uint32_t start_column, uint32_t end_column);
	
	/*!
	 @brief Draw a progress bar-like graphic where the bar originates at
	 the center
	 @param value The value from -1 to 1 to display
	 @param page The page to draw it in
	 @param start_column The position of the left end of the bar
	 @param end_column The position of the right end of the bar
	 */
	void draw_pos_neg_bar(float value, uint8_t page,
	                      uint32_t start_column, uint32_t end_column);
	
	/*!
	 @brief Reset bounds of area to update
	 */
	void sync(bool do_lock = true);
	void lock(){
		chMtxLock(&mutex);
	}
	
	void unlock(){
		chMtxUnlock();
	}

	//! The number of columns
	static constexpr uint32_t num_columns = columns;
	//! The number of pages
	static constexpr uint32_t num_pages = pages;
	
	//! View the framebuffer as a const linear array
	uint8_t const * get_fb(uint32_t page = 0) const {return fb[page];}
	
	//! Get the current dirty region limits
	limits_t const &get_limits() const {return limits;}
protected:
	//! The actual framebuffer itself
	uint8_t fb[pages][columns];
	
	//! The limits of the dirty region which needs to be updated
	limits_t limits;
	
	//! A lock to serialize access to the update bounds of the framebuffer
	Mutex mutex;
	
	typedef bool (write_text_func_t(uint8_t *, uint8_t, char const *, uint32_t &, uint32_t));
	
	/*!
	 @brief Write text on the framebuffer
	 @param f The write function to use
	 @param text The text to write
	 @param page The page to start on
	 @param column The column to start on
	 @param max_end_column A column to limit print length

	 The height of the result depends on the font. It is the responsibility of
	 the caller to make sure that this doesn't overflow the buffer.
	*/
	uint32_t write_text_generic(write_text_func_t f, char const * text,
	                            uint8_t page, uint8_t column,
	                            uint32_t max_end_column = columns);
};

//! @} @}

#endif
