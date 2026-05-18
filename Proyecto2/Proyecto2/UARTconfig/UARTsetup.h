/*
 * UARTsetup.h
 *
 * Created: 22/04/2026 16:13:39
 *  Author: capid
 */ 


#ifndef UARTSETUP_H_
#define UARTSETUP_H_

#include <avr/io.h>

void initUART();
void writeChar(char caracter);
void writeString(char* string);
void writeNumber(uint8_t num);

#endif /* UARTSETUP_H_ */