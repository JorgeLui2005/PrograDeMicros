/*
 * EEPROM.c
 *
 * Created: 16/05/2026 21:40:28
 *  Author: capid
 */ 
#include "EEPROM.h"

void eepromWrite(uint16_t direccion, uint8_t dato)
{
	while (EECR & (1<<EEPE));
	//Setear direccion y dato
	EEAR = direccion;
	EEDR = dato;
	EECR |= (1<<EEMPE);
	EECR |= (1<<EEPE);
}

uint8_t eepromRead(uint16_t direccion)
{
	while (EECR & (1<<EEPE));
	//Setear direccion
	EEAR = direccion;
	EECR |= (1<<EERE);
	return EEDR;
}