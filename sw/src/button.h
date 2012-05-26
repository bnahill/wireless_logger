#ifndef __BUTTON_H_
#define __BUTTON_H_

#include "stm32f10x.h"
#include "sensor_config.h"

void button_init(void);

int button_check_press(void);

#endif
