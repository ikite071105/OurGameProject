#ifndef __EEPROM_H
#define __EEPROM_H

#include <stdint.h>
#include <stdbool.h>

void EEPROM_Init(void);

bool EEPROM_WriteByte(uint8_t addr, uint8_t data);
bool EEPROM_ReadByte(uint8_t addr, uint8_t *data);

bool EEPROM_WriteBuffer(uint8_t addr, uint8_t *buf, uint16_t len);
bool EEPROM_ReadBuffer(uint8_t addr, uint8_t *buf, uint16_t len);

#endif