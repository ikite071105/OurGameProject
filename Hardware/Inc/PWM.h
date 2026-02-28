#ifndef PWM_H
#define PWM_H

#include "gd32f4xx.h"
#include "systick.h"

void pwm_gpio_config(void);
void pwm_config(uint16_t pre,uint16_t per);
void pwm_breathing_lamp(void);

#endif