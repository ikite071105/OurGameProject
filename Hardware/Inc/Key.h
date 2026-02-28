#ifndef KEY_H
#define KEY_H

#include "gd32f4xx.h"
#include "systick.h"
#include <stdbool.h>
#include <stdio.h>
#include "LED.h"
#include "UsartTest.h"

void key_init();
void scan_KEYL();

#endif