#include "imu/framebuffer.h"

template <uint32_t pages, uint32_t columns>
FrameBuffer< pages, columns >::FrameBuffer(){
	limits.x_min = columns - 1;
	limits.y_min = pages - 1;
	limits.x_max = 0;
	limits.y_max = 0;
	chMtxInit(&mutex);
}

template <uint32_t pages, uint32_t columns>
void FrameBuffer< pages, columns >::clear_area(
	  uint32_t start_page, uint32_t end_page,
	  uint32_t start_column, uint32_t end_column){
	uint32_t i, j;
	lock();
	for(i = start_page; i < end_page; i++){
		for(j = start_column; j < end_column; j++){
			fb[i][j] = 0;
		}
	}
	limits.y_min = min(start_page, limits.y_min);
	limits.y_max = max(end_page, limits.y_max);
	limits.x_min = start_column;
	limits.x_max = end_column;
	
	unlock();
}

template <uint32_t pages, uint32_t columns>
void FrameBuffer< pages, columns >::clear(){
	clear_area();
}

/*!
	@brief Write text on the framebuffer
	@tparam font_class The class of font to write with
	@param text The text to write
	@param page The page to start on
	@param column The column to start on
	
	The height of the result depends on the font. It is the responsibility of
	the caller to make sure that this doesn't overflow the buffer.
	*/
template <uint32_t pages, uint32_t columns>
template<typename font_class>
uint32_t FrameBuffer< pages, columns >::write_text(char const * text,
	  uint8_t page, uint8_t column, uint32_t max_end_column){
	uint32_t i = 0;
	uint32_t n_cols = 0;
	lock();
	
	limits.y_min = min(page, limits.y_min);
	limits.x_min = min(column, limits.x_min);
	while(page < pages){
		// Take take the i-th row of the printed text
		if(!font_class::write_text(fb[page] + column, i, text, n_cols,
			                       max_end_column - column)){
			limits.x_max = max(min(n_cols + column, columns), limits.x_max);
			limits.y_max = max(min(page, pages), limits.y_max);
			break;
		}
		page += 1;
		i += 1;
	}
	
	unlock();
	
	return n_cols + column;
}



/*!
	@brief Draw a horizontally with a bit mask
	@param page The page to mask
	@param mask The mask to use
	@param col_start The first column to apply this to
	@param length The number of columns to write
	*/
template <uint32_t pages, uint32_t columns>
void FrameBuffer< pages, columns >::draw_horizontal_mask(uint8_t page,
	  uint8_t mask, uint32_t col_start, uint32_t length){
	uint8_t * iter = &fb[page][col_start];
	lock();
	
	limits.x_min = min(limits.x_min, col_start);
	limits.x_max = max(limits.x_max, col_start + length);
	limits.y_min = min(limits.y_min, page);
	limits.y_max = max(limits.y_max, page);
	while(0 != length--){
		*(iter++) |= mask;
	}
	
	unlock();
}
template <uint32_t pages, uint32_t columns>
void FrameBuffer< pages, columns >::draw_vertical(uint8_t start_page,
	  uint8_t end_page, uint32_t column){
	lock();
	limits.x_min = min(limits.x_min, column);
	limits.x_max = max(limits.x_max, column);
	limits.y_min = min(limits.y_min, start_page);
	limits.x_max = max(limits.y_max, end_page);
	while(start_page != end_page + 1){
		fb[start_page++][column] = 0xFF;
	}
	unlock();
}

/*!
	@brief Reset bounds of area to update
	*/
template <uint32_t pages, uint32_t columns>
void FrameBuffer< pages, columns >::sync(bool do_lock){
	if(do_lock)
		lock();
	
	limits.x_min = columns - 1;
	limits.y_min = pages - 1;
	limits.x_max = 0;
	limits.y_max = 0;
	
	if(do_lock)
		unlock();
}