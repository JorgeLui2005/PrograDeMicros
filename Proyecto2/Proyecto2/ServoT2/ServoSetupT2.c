/*
 * ServoSetupT2.c
 *
 * Created: 26/04/2026 18:22:29
 *  Author: capid
 */ 
#include <avr/io.h>
void initServo2A()
{
	DDRB |= (1<<DDB3); // D11

	TCCR2A = 0;
	TCCR2B = 0;

	// Fast PWM
	TCCR2A |= (1<<WGM21) | (1<<WGM20);

	// Canal A
	TCCR2A |= (1<<COM2A1);

	// Prescaler 1024
	TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);
}

void initServo2B()
{
	 DDRD |= (1<<DDD3); // D3

	 TCCR2A |= (1<<COM2B1);
}

void setServoAngle2A(uint8_t angulo)
{
	uint16_t valor = 8 + ((uint32_t)angulo * (39 - 8) / 180);
	//Cargar el valor a la salida B3
	OCR2A = valor;
}

void setServoAngle2B(uint8_t angulo)
{
	uint16_t valor = 8 + ((uint32_t)angulo * (39 - 8) / 180);
	//Cargar el valor a la salida B3
	OCR2B = valor;
}