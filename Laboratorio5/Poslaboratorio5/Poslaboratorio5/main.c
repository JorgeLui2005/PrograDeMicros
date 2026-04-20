/*
 * Poslaboratorio5.c
 *
 * Created: 19/04/2026 13:08:52
 * Author: Jorge Diéguez
 * Description: Controlar servos y led con PWM
 */
/****************************************/
// Encabezado (Libraries)
#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Servo/ServoSetup.h"
#include "PWM/PWM0.h"

volatile uint8_t ADCvalor1 = 0;
volatile uint8_t ADCvalor2 = 0;
volatile uint8_t ADCvalor3 = 0;
volatile uint8_t dummy = 0;
volatile uint8_t canal = 0;

/****************************************/
// Function prototypes
void initADC();
void initTMR0(); 
void setServoAngle2A(uint8_t angulo);
void initServo2A();

/****************************************/
// Main Function
int main(void)
{
	//Inicializacion del modulo ADC
	initADC();
	//Seteo PWM Manual
	initTMR0();
	sei();
	//Inicio Servos (libreria)
	servoInit1A();
	initServo2A();
	uint8_t dutycicle = ADCvalor3;
	ADCSRA |= (1<<ADSC);
	while (1)
	{
		dutycicle = ADCvalor3;
		//Refrescar DutyCicle
		updateDutyCicle0A(dutycicle);
		
		//Mapeo de angulos
		uint16_t angle1;
		uint16_t angle2;
		angle1 = (ADCvalor1 * 180)/255;
		angle2 = (ADCvalor2 * 180)/255;
		
		//Colocacion de angulo
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
	ADMUX |= 0x07; // ADC6 (A6)

	canal = 7;

	ADCSRA = 0;
	//Habilitar ADC y prescaler = 128
	ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
}

void initTMR0()
{
	// Colocar el puerto que sera la salida
	DDRB |= (1<<DDB0);
	PORTB &= ~(1<<PORTB0);
	
	// Colocar el prescaler
	TCCR0A = 0;
	TCCR0B = (1<<CS02) | (1<<CS00);
	
	//Activar interrupciones
	TIMSK0 = (1<<OCIE0A) | (1<<TOIE0);
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
	switch (canal){
		case 7:
			ADCvalor1 = ADCH;
			canal = 6;
			ADMUX = (ADMUX & 0xF0) | 0x06; // ADC6
			break;
			
		case 6:
			ADCvalor2 = ADCH;
			canal = 5;
			ADMUX = (ADMUX & 0xF0) | 0x05; // ADC5
			break;
			
		case 5:
			ADCvalor3 = ADCH;
			canal = 7;
			ADMUX = (ADMUX & 0xF0) | 0x07; // ADC7
			break;
		default:
			ADCvalor3 = ADCH;
			canal = 7;
			ADMUX = (ADMUX & 0xF0) | 0x07; // ADC7
			break;
	}
		
	ADCSRA |= (1<<ADSC); // Nueva conversión
}

ISR (TIMER0_OVF_vect)
{
	//Si hay overflow se enciende el puerto
	PORTB |= (1<<PORTB0);
}

ISR (TIMER0_COMPA_vect)
{
	//Cuando la comparacion se cumpla queremos que se apague
	PORTB &= ~(1<<PORTB0);
}