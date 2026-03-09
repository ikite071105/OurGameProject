#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>

void TRNG_Init(void);
uint32_t TRNG_GetRandom(void);

#endif