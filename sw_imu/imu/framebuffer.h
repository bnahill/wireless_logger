#ifndef __IMU_FRAMEBUFFER_H_
#define __IMU_FRAMEBUFFER_H_

#include "imu/imu.h"
#include "imu/imu_math.h"
#include "ch.h"
#include "hal.h"

template <uint32_t pages, uint32_t columns>
class FrameBuffer {
public:
	typedef struct {
		uint32_t x_min;
		uint32_t x_max;
		uint32_t y_min;
		uint32_t y_max;
	} limits_t;
	
	
	FrameBuffer<pages,columns>(){
		limits.x_min = columns - 1;
		limits.y_min = pages - 1;
		limits.x_max = 0;
		limits.y_max = 0;
		chMtxInit(&mutex);
	}
	
	void clear_page(uint32_t page){
		uint32_t i;
		lock();
		for(i = 0; i < columns; i++){
			fb[page][i] = 0;
		}
		limits.y_min = min(page, limits.y_min);
		limits.y_max = max(page, limits.y_max);
		limits.x_min = 0;
		limits.x_max = columns - 1;
		
		unlock();
	}
	
	void clear(){
		uint32_t i;
		for(i = 0; i < pages; i++){
			clear_page(i);
		}
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
	template<typename font_class>
	void write_text(char const * text, uint8_t page, uint8_t column){
		uint32_t i = 0;
		uint32_t n_cols = 0;
		lock();
		
		limits.y_min = min(page, limits.y_min);
		limits.x_min = min(column, limits.x_min);
		while(page < pages){
			// Take take the i-th row of the printed text
			if(!font_class::write_text(fb[page] + column, i, text, n_cols)){
				limits.x_max = max(min(n_cols + column, columns), limits.x_max);
				limits.y_max = max(min(page, pages), limits.y_max);
				break;
			}
			page += 1;
			i += 1;
		}
		
		unlock();
	}
	
	
	
	/*!
	 @brief Draw a horizontally with a bit mask
	 @param page The page to mask
	 @param mask The mask to use
	 @param col_start The first column to apply this to
	 @param length The number of columns to write
	 */
	void draw_horizontal_mask(uint8_t page, uint8_t mask, uint32_t col_start, uint32_t length){
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
	
	void draw_vertical(uint8_t start_page, uint8_t end_page, uint32_t column){
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
	void sync(bool do_lock = true){
		if(do_lock)
			lock();
		
		limits.x_min = columns - 1;
		limits.y_min = pages - 1;
		limits.x_max = 0;
		limits.y_max = 0;
		
		if(do_lock)
			unlock();
	}
	
	void lock(){
		chMtxLock(&mutex);
	}
	
	void unlock(){
		chMtxUnlock();
	}
	
	uint8_t const * get_fb() const {return fb[0];}
	uint8_t const * get_fb(uint32_t page) const {return fb[page];}
	limits_t const &get_limits() const {return limits;}
protected:
	
	
	
	uint8_t fb[pages][columns];
	limits_t limits;
	Mutex mutex;
};

#endif
