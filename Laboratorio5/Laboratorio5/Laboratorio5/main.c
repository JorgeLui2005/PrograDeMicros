/*
 * Laboratorio5.c
 *
 * Created: 13/04/2026 17:16:46
 * Author: Jorge Diéguez
 * Description: PWM1 servo
 */
/****************************************/
// Encabezado (Libraries)
#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Servo/ServoSetup.h"

volatile uint8_t ADCvalor1 = 0;
volatile uint8_t ADCvalor2 = 0;
volatile uint8_t canal = 0;

/****************************************/
// Function prototypes
void initADC();
void initServo2A();
void setServoAngle2A(uint8_t angulo);

/****************************************/
// Main Function
int main(void)
{
	initADC();
	sei();
	servoInit1A();
	initServo2A();
	ADCSRA |= (1<<ADSC);
	while (1)
	{
		uint16_t angle1;
		uint16_t angle2;
		angle1 = (ADCvalor1 * 180)/255;
		angle2 = (ADCvalor2 * 180)/255;
		setServoAngle1A(angle1);
		setServoAngle2A(angle2);
		_delay_ms(20);
	}
}

/****************************************/
// NON-Interrupt subroutines
void initADC()
{
	ADMUX = 0;
	//Justificacion a la izquierda
	ADMUX |= (1<<REFS0) | (1<<ADLAR);
	ADMUX |= 0x06; // ADC6 (A6)

	canal = 6;

	ADCSRA = 0;
	//Habilitar ADC y prescaler = 8
	ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
}

void initServo2A()
{
	//Salida del puerto D3
	DDRB |= (1<<DDB3);
	
	TCCR2A = 0;
	// Timer en modo Fast PWM
	TCCR2A = (1<<WGM21) | (1<<WGM20) | (1<<COM2A1);
	
	//Prescaler de 1024 ya que no se usa prescaler en el sistema
	TCCR2B = (1<<CS22) | (1<<CS21) | (1<<CS20);
}

void setServoAngle2A(uint8_t angulo)
{
	uint16_t valor = 8 + ((uint32_t)angulo * (39 - 8) / 180);
	//Cargar el valor a la salida B3
	OCR2A = valor;
}
/****************************************/
// Interrupt routines
ISR(ADC_vect)
{
	if (canal == 0)
	{
		ADCvalor1 = ADCH;
		canal = 1;
		ADMUX = (ADMUX & 0xF0) | 0x07; //ADC7
	}
	else
	{
		ADCvalor2 = ADCH;
		canal = 0;
		ADMUX = (ADMUX & 0xF0) | 0x06; //ADC6
	}
	
	ADCSRA |= (1<<ADSC); //Nueva conversión
}
