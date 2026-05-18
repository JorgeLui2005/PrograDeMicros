/*
 * ADCsetup.h
 *
 * Created: 26/04/2026 17:45:43
 *  Author: capid
 */ 


#ifndef ADCSETUP_H_
#define ADCSETUP_H_

#include <avr/io.h>

#define derecha 0
#define izquierda 1
#define auto_t 1
#define no_auto_t 0

void initADC(uint8_t justif, uint8_t chanel, uint8_t autoTrig, uint16_t presc);

#endif /* ADCSETUP_H_ */