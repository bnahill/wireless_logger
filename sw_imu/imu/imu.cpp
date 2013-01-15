#include "imu.h"

char *uint_to_string(uint32_t i, char * s, bool ignore_leading){
	uint32_t div;
	uint32_t place = 1000000;
	
	bool start_printing = !ignore_leading;
	
	for(place = 1000000; place; place /= 10){
		div = i / place;
		if(!div){
			if(start_printing || (place == 1))
				*(s++) = '0';
			continue;
		}
		*(s++) = div + '0';
		start_printing = true;
		i = i % place;
	}
	*s = 0;
	return s;
}

void float_to_string(float f, char * s){
	uint32_t frac = (int32_t)(f * (1 << 10));
	if(frac & 0x80000000){
		*(s++) = '-';
		frac = -frac;
	}
	s = uint_to_string(frac >> 10, s);
	f = frac & ((1 << 10) - 1);
	f *= 10000000.0 / ((float)0x3FF);
	*(s++) = '.';
	s = uint_to_string((uint32_t) f, s, false);
	*s = 0;
}