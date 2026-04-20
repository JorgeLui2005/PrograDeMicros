/*
 * ServoSetup.h
 *
 * Created: 12/04/2026 21:50:17
 *  Author: capid
 */ 


#ifndef SERVOSETUP_H_
#define SERVOSETUP_H_

#include <avr/io.h>

void servoInitA(); //Servo canal A (D9)
void servoInitB(); //Servo canal B (D10)

// Control por ángulo
void setServoAngleA(uint8_t angulo);
void setServoAngleB(uint8_t angulo);

// Control directo por tiempo de pulso (en us)
void setServoPulseA(uint16_t pulso);
void setServoPulseB(uint16_t pulso);

#endif /* SERVOSETUP_H_ */