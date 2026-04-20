/*
 * Prelaboratorio5.c
 *
 * Created: 12/04/2026 16:30:02
 * Author: Jorge Diéguez
 * Description: PWM1 servo
 */
/****************************************/
// Encabezado (Libraries)
#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ServoM14/ServoSetup.h"

volatile uint8_t ADCvalor = 0;
/****************************************/
// Function prototypes
void initADC();

/****************************************/
// Main Function
int main(void)
{
	initADC();
	sei();
	servoInitA();
	ADCSRA |= (1<<ADSC);
	while (1)
	{
		uint16_t angle;
		angle = (ADCvalor * 180)/255;
		setServoAngleA(angle);
		_delay_ms(20);
	}
}

/****************************************/
// NON-Interrupt subroutines
void initADC()
{
	ADMUX		= 0;
	//Justificacion a la izquierda y seleccion de ADCC
	ADMUX		|= (1<<REFS0) | (1<<ADLAR) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0);
	ADCSRA		= 0;
	//Habilitar ADC y prescaler = 8
	ADCSRA		|= (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
}

/****************************************/
// Interrupt routines
ISR(ADC_vect)
{
	ADCvalor = ADCH;
	ADCSRA	|= (1<<ADSC);
}
