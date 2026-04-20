/*
 * PWM1.c
 *
 * Created: 12/04/2026 21:09:00
 *  Author: capid
 */ 
#include "PWM1.h"

void initPWM1A(uint8_t invert, uint8_t modo, uint16_t prescaler, uint16_t top)
{
	//Configurar la salida PWM en el PORTB 1, D9
	DDRB |= (1<<DDB1); 
	
	TCCR1A = 0;
	TCCR1B = 0;
	
	if (modo)
	{
		//Modo 10: Phase Correct PWM, TOP = ICR1
		TCCR1A |= (1<<WGM11);
		TCCR1B |= (1<<WGM13);
	}
	else
	{
		//Modo 14: Fast PWM, TOP = ICR1
		TCCR1A |= (1<<WGM11);
		TCCR1B |= (1<<WGM12) | (1<<WGM13);
	}

	if (invert)
	{
		TCCR1A |= (1<<COM1A1);
	}else{
		TCCR1A |= (1<<COM1A1) | (1<<COM1A0);
	}
	
	ICR1 = top;
	
	switch(prescaler)
	{
		case 1:
			TCCR1B |= (1<<CS10);
			break;
		case 8:
			TCCR1B |= (1<<CS11);
			break;
		case 64:
			TCCR1B |= (1<<CS11) | (1<<CS10);
			break;
		case 256:
			TCCR1B |= (1<<CS12);
			break;
		case 1024:
			TCCR1B |= (1<<CS12) | (1<<CS10);
			break;
		default:
			TCCR1B |= (1<<CS10);
			break;
	}
}

void initPWM1B(uint8_t invert, uint8_t modo, uint16_t prescaler, uint16_t top)
{
	DDRB |= (1<<DDB2); // OC1B (D10)

	// Inversión
	if (invert)
	{
		TCCR1A |= (1<<COM1B1);
	}else{
		TCCR1A |= (1<<COM1B1) | (1<<COM1B0);
	}
}

void updateDutyCicle1A(uint16_t duty)
{
	OCR1A = duty;
}

void updateDutyCicle1B(uint16_t duty)
{
	OCR1B = duty;
}