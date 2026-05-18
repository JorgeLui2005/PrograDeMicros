/*
 * ServoSetupT2.h
 *
 * Created: 26/04/2026 18:22:11
 *  Author: capid
 */ 


#ifndef SERVOSETUPT2_H_
#define SERVOSETUPT2_H_

#include <avr/io.h>

void initServo2A();
void initServo2B();
void setServoAngle2A(uint8_t angulo);
void setServoAngle2B(uint8_t angulo);

#endif /* SERVOSETUPT2_H_ */