/*
 * ServoSetup.c
 *
 * Created: 12/04/2026 21:50:33
 *  Author: capid
 */ 
#include "ServoSetup.h"

#define SERVO_MIN 800 //1 ms
#define SERVO_MAX 5000 //2 ms
#define SERVO_TOP 39999 //20 ms

void servoInitA()
{
	DDRB |= (1<<DDB1); //PORTB 1, D9

	TCCR1A = 0;
	TCCR1B = 0;

	//Configuracion fast para modo 14
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM12) | (1<<WGM13);

	TCCR1A |= (1<<COM1A1); //Modo no invertido

	TCCR1B |= (1<<CS11); // Prescaler 8

	ICR1 = SERVO_TOP; //Para un periodo de 20 ms
}

void servoInitB()
{
	DDRB |= (1<<DDB2); // PORTB 2, D10

	TCCR1A |= (1<<COM1B1); //Solo habilita la salida ya que comparte reloj
}

void setServoAngleA(uint8_t angulo)
{
	//Conversion por regla de tres del angulo y sumatoria con el minimo para modificar posicion
	uint16_t valor = SERVO_MIN + ((uint32_t)angulo * (SERVO_MAX - SERVO_MIN) / 180);
	//Cargar la posicion
	OCR1A = valor;
}

void setServoAngleB(uint8_t angulo)
{
	//Misma conversion que a pero para el servo en puerto D10
	uint16_t valor = SERVO_MIN + ((uint32_t)angulo * (SERVO_MAX - SERVO_MIN) / 180);
	//Cargar el valor a la salida D10
	OCR1B = valor;
}

void setServoPulseA(uint16_t pulso)
{
	OCR1A = pulso * 2; // porque 1 tick = 0.5us
}

void setServoPulseB(uint16_t pulso)
{
	OCR1B = pulso * 2;
}