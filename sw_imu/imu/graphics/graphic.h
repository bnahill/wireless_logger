#ifndef __IMU_GRAPHIC_H_
#define __IMU_GRAPHIC_H_

#include "imu/imu.h"
#include "ch.h"
#include "hal.h"

/*!
 @brief A simple drawing
 */
class Graphic {
public:
	Graphic(uint32_t rows, uint32_t columns, uint8_t const * buffer) :
		buffer(*buffer), rows(rows), columns(columns)
	{}
	uint8_t const rows;
	uint8_t const columns;
	uint8_t const &buffer;
};

namespace Graphics {
	
};

#endif // __IMU_GRAPHIC_H_
