/*
 * ServoSetup.h
 *
 * Created: 12/04/2026 21:50:17
 *  Author: capid
 */ 


#ifndef SERVOSETUP_H_
#define SERVOSETUP_H_

#include <avr/io.h>

void servoInit1A(); //Servo canal A (D9)
void servoInit1B(); //Servo canal B (D10)

// Control por ángulo
void setServoAngle1A(uint8_t angulo);
void setServoAngle1B(uint8_t angulo);

// Control directo por tiempo de pulso (en us)
void setServoPulse1A(uint16_t pulso);
void setServoPulse1B(uint16_t pulso);

#endif /* SERVOSETUP_H_ */