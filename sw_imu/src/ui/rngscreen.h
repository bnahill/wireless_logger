#ifndef __UI_RNG_SCREEN_H_
#define __UI_RNG_SCREEN_H_

#include "ui/ui.h"

class RNGScreen {
public:
	RNGScreen();
	
	void exec();
protected:
	uint32_t num_choices;
	msg_t terminate();
	
	static void print_number(uint32_t num, char * where);
};

#endif // __UI_RNGSCREEN_H_
