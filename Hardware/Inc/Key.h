#ifndef KEY_H
#define KEY_H

#include "gd32f4xx.h"
#include "systick.h"
#include <stdbool.h>
#include <stdio.h>
#include "LED.h"
#include "UsartTest.h"

extern volatile bool L_pressed;
extern volatile bool R_pressed;
extern volatile bool A_pressed;
extern volatile bool B_pressed;

void key_init();
void scan_KEYL();

#endif