#ifndef LED_H
#define LED_H

#include "gd32f4xx.h"
#include "systick.h"
#include <stdbool.h>

void LEDenable();
void SetLED1(bool a);
void SetLED2(bool a);
void SetLED3(bool a);
void SetLED4(bool a);

#endif