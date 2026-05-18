/*
 * EEPROM.h
 *
 * Created: 16/05/2026 21:39:24
 *  Author: capid
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include <avr/io.h>

void eepromWrite(uint16_t direccion, uint8_t dato);
uint8_t eepromRead(uint16_t direccion);


#endif /* EEPROM_H_ */