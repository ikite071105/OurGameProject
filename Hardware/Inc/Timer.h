#ifndef TIMER_H
#define TIMER_H

#include "gd32f4xx.h"
#include "systick.h"
#include "gd32f4xx_rcu.h"
#include <stdio.h>
#include "LED.h"
#include "UsartTest.h"

void basic_timer_config(uint16_t pre,uint16_t per);

#endif