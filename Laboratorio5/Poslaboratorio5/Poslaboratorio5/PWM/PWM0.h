/*
 * PWM0.h
 *
 * Created: 12/04/2026 19:42:05
 *  Author: capid
 */ 


#ifndef PWM0_H_
#define PWM0_H_

#include <avr/io.h>

#define no_invertido 0
#define invertido 1
#define fastPWM 0
#define phasePWM 1

void initPWM0A(uint8_t invert, uint8_t modo, uint16_t prescaler);
void initPWM0B(uint8_t invert, uint8_t modo, uint16_t prescaler);
void updateDutyCicle0A(uint8_t duty);
void updateDutyCicle0B(uint8_t duty);

#endif /* PWM0_H_ */