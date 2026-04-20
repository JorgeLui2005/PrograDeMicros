/*
 * PWM0.c
 *
 * Created: 12/04/2026 19:43:01
 *  Author: capid
 */ 
#include "PWM0.h"

void initPWM0A(uint8_t invert, uint8_t modo, uint16_t prescaler)
{
	//Configurar la salida PWM en el PORTD 6
	DDRD |= (1<<DDD6);
	
	TCCR0A &= ~((1<<COM0A1) | (1<<COM0A0));
	TCCR0B = 0;
	
	if (invert)
	{
		TCCR0A |= (1<<COM0A1) | (1<<COM0A0); //Invertido
	} else {
		TCCR0A |= (1<<COM0A1); //No invertido
	}

	if (modo)
	{
		TCCR0A |= (1<<WGM00); //Phase mode
	}else{
		TCCR0A |= (1<<WGM01) | (1<<WGM00); //Fast mode
	}

	switch(prescaler)
	{
		case 1: 
			TCCR0B |= (1<<CS00);
			break;
		case 8:
			TCCR0B |= (1<<CS01);
			break;
		case 64:
			TCCR0B |= (1<<CS01) | (1<<CS00);
			break;
		case 256:
			TCCR0B |= (1<<CS02);
			break;
		case 1024:
			TCCR0B |= (1<<CS02) | (1<<CS00);
			break;
		default: 
			TCCR0B |= (1<<CS00);
			break;
	}
}

void initPWM0B(uint8_t invert, uint8_t modo, uint16_t prescaler)
{
	//Configurar la salida PWM en el PORTD 6
	DDRD |= (1<<DDD5);
	
	TCCR0A &= ~((1<<COM0B1) | (1<<COM0B0));
	TCCR0B = 0;
	
	if (invert)
	{
		TCCR0A |= (1<<COM0B1) | (1<<COM0B0); //Invertido
		} else {
		TCCR0A |= (1<<COM0B1); //No invertido
	}

	if (modo)
	{
		TCCR0A |= (1<<WGM00); //Phase mode
		}else{
		TCCR0A |= (1<<WGM01) | (1<<WGM00); //Fast mode
	}

	switch(prescaler)
	{
		case 1:
		TCCR0B |= (1<<CS00);
		break;
		case 8:
		TCCR0B |= (1<<CS01);
		break;
		case 64:
		TCCR0B |= (1<<CS01) | (1<<CS00);
		break;
		case 256:
		TCCR0B |= (1<<CS02);
		break;
		case 1024:
		TCCR0B |= (1<<CS02) | (1<<CS00);
		break;
		default:
		TCCR0B |= (1<<CS00);
		break;
	}
}

void updateDutyCicle0A(uint8_t duty)
{
	OCR0A = duty;
}

void updateDutyCicle0B(uint8_t duty)
{
	OCR0B = duty;
}