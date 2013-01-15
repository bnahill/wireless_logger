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

template <uint32_t pages, uint32_t columns>
class FrameBuffer {
public:
	FrameBuffer< pages, columns >();
	
	typedef struct {
		uint32_t x_min;
		uint32_t x_max;
		uint32_t y_min;
		uint32_t y_max;
	} limits_t;
	
	void clear_area(uint32_t start_page = 0, uint32_t end_page = pages - 1,
	                uint32_t start_column = 0, uint32_t end_column = columns - 1);
	
	void clear();
	
	/*!
	 @brief Write text on the framebuffer
	 @tparam font_class The class of font to write with
	 @param text The text to write
	 @param page The page to start on
	 @param column The column to start on
	 
	 The height of the result depends on the font. It is the responsibility of
	 the caller to make sure that this doesn't overflow the buffer.
	 */
	template<class font_class>
	uint32_t write_text(char const * text, uint8_t page, uint8_t column,
	                uint32_t max_end_column = columns);
	
	template<class font_class>
	uint32_t write_text_centered(char const * text, uint8_t page, uint8_t center_column,
	                uint32_t max_width);
	
	
	
	/*!
	 @brief Draw a horizontally with a bit mask
	 @param page The page to mask
	 @param mask The mask to use
	 @param col_start The first column to apply this to
	 @param length The number of columns to write
	 */
	void draw_horizontal_mask(uint8_t page, uint8_t mask,
	                          uint32_t col_start, uint32_t length);
	
	void draw_vertical(uint8_t start_page, uint8_t end_page, uint32_t column);
	
	void draw_progress_bar(float value, uint8_t page,
	                       uint32_t start_column, uint32_t end_column);
	
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
	
	uint32_t get_center_col() const {return columns / 2;}
	uint32_t get_num_columns() const {return columns;}
	uint32_t get_num_pages() const {return pages;}
	
	uint8_t const * get_fb() const {return fb[0];}
	uint8_t const * get_fb(uint32_t page) const {return fb[page];}
	limits_t const &get_limits() const {return limits;}
protected:
	
	uint8_t fb[pages][columns];
	limits_t limits;
	Mutex mutex;
};

#include "framebuffer.cpp"

#endif
