/*
 * PWM1.h
 *
 * Created: 12/04/2026 21:07:43
 *  Author: capid
 */ 


#ifndef PWM1_H_
#define PWM1_H_

#include <avr/io.h>

#define no_invertido 0
#define invertido 1
#define fastPWM 0
#define phasePWM 1

void initPWM1A(uint8_t invert, uint8_t modo, uint16_t prescaler, uint16_t top);
void initPWM1B(uint8_t invert, uint8_t modo, uint16_t prescaler, uint16_t top);
void updateDutyCicle1A(uint16_t duty);
void updateDutyCicle1B(uint16_t duty);

#endif /* PWM1_H_ */