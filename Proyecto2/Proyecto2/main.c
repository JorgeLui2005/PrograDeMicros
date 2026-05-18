/*
 * Proyecto2.c
 *
 * Created: 4/05/2026 16:54:18
 * Author: Jorge Diéguez
 * Description: Proyecto final del curso de programación de microcontroladores
 */
/****************************************/
// Encabezado (Libraries)
#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Servo/ServoSetup.h"
#include "PWM/PWM0.h"
#include "ADCsetup/ADCsetup.h"
#include "ServoT2/ServoSetupT2.h"
#include "UARTconfig/UARTsetup.h"
#include "EEPROMlib/EEPROM.h"

volatile uint8_t ADCvalor1 = 0; //Servo A TMR2
volatile uint8_t ADCvalor2 = 0; //Servo B TMR2
volatile uint8_t ADCvalor3 = 0; //Servo A y B TMR1
volatile uint8_t ADCvalor4 = 0; //Control de brillo de LED
volatile uint8_t dummy = 0;
volatile uint8_t canal = 7; //Numero de canal del ADC
volatile uint8_t modo = 0;//Numero de modo (1 Manual, 2 Python, 3 EEPROM)
volatile uint8_t MSK_estado = 0;
volatile uint8_t save_pos = 0;
volatile uint8_t servoBase = 90;
volatile uint8_t servoCodo = 90;
volatile uint8_t servoMSK = 0;

//Instrucciones para la terminal 
const char* modo0 = "MOD:0"; //Manual
const char* modo1 = "MOD:1"; //UART
const char* modo2 = "MOD:2"; //EEPROM
const char* MSK_cerrado = "MSK:0";
const char* MSK_abierto = "MSK:1";
const char* ARM_1 = "ARM:0";
const char* ARM_2 = "ARM:1";
const char* LED_OFF = "LED:0";
const char* LED_ON = "LED:1";
const char* BASE_POS = "B:---";
const char* CODO_POS = "C:---";
const char* MSK_POS = "M:---";
char comando[10]  = "---------";

const uint8_t MSK_CLSD = 10;
const uint8_t MSK_OPEN = 11;
const uint8_t ARM1_POS1 = 12;
const uint8_t ARM1_POS2 = 13;
const uint8_t ARM2_POS1 = 14;
const uint8_t ARM2_POS2 = 15;

uint8_t num_received = 0;
uint8_t dir_eeprom = 0;

/****************************************/
// Function prototypes
void initPuertos();
void initPC();
void check_MOD();
void check_MSK();
void check_ARM();
void check_LED();
void mod_manual();
void mod_uart();
void mod_eeprom();
void guardarPos ();
uint8_t extraerNumero();

/****************************************/
// Main Function
int main(void)
{
	//Inicializacion del modulo ADC
	initADC(izquierda, 7, no_auto_t, 128);
	
	//Inicio Servos (libreria)
	servoInit1A();
	servoInit1B();
	initServo2A();
	initServo2B();
	
	//Inicio PWM TMR0
	initPWM0A(no_invertido, fastPWM, 8);
	
	//Inicializacion de puertos
	initPuertos();
	
	//Inicializacion del UART
	initUART();
	
	//Interrupciones
	initPC();
	
	sei();
	ADCSRA |= (1<<ADSC);
	writeString("Para cambiar el modo ingrese la instreccion *MOD:* seguida del numero del modo deseado \n");
	
	//eepromWrite(MSK_CLSD, 0);
	//eepromWrite(MSK_OPEN, 180);
	
	while (1)
	{
		//Guarda la posicion en la EEPROM
		guardarPos();
		
		//Maquina de estados para el modo
		switch(modo){
			case 0:
				PORTB |= (1<<PORTB0);
				PORTD &= ~(1<<PORTD7);
				mod_manual();
				break;
			case 1:
				PORTB &= ~(1<<PORTB0);
				PORTD |= (1<<PORTD7);
				mod_uart();
				break;
			case 2:
				PORTB |= (1<<PORTB0);
				PORTD |= (1<<PORTD7);
				mod_eeprom();
		}
	}
}

/****************************************/
// NON-Interrupt subroutines
void initPuertos()
{
	//Seteo de puertos como entrada (PORTC)
	DDRC &= ~((1<<DDC0) | (1<<DDC1));
	PORTC |= (1<<PORTC0) | (1<<PORTC1);
	
	//Seteo de puertos como salida (PORTB)
	DDRB |= (1<<DDB4) | (1<<DDB0);
	PORTB &= ~((1<<PORTB0) | (1<<PORTB4));
	
	//Seteo de puertos como salida (PORTD)
	DDRD |= (1<<DDD7);
	PORTD &= ~(1<<PORTD7);
}

void initPC() 
{
	PCICR |= (1<<PCIE1);
	PCMSK1 |= (1<<PCINT8) | (1<< PCINT9);
}

void check_MOD()
{
	//Identificador de comando para modo
	if (*(comando+0) == *(modo0+0) &&
		*(comando+1) == *(modo0+1) &&
		*(comando+2) == *(modo0+2)&&
		*(comando+3) == *(modo0+3))
	{
		if (*(comando+4) == *(modo0+4))
		{
			modo = 0;
		}else if(*(comando+4) == *(modo1+4)){
			modo = 1;
		}else if(*(comando+4) == *(modo2+4)){
			modo = 2;
		}
	}
}

void check_MSK()
{
	//Identificador de comando para posiciones de la mascara
	if (*(comando+0) == *(MSK_cerrado+0) &&
		*(comando+1) == *(MSK_cerrado+1) &&
		*(comando+2) == *(MSK_cerrado+2)&&
		*(comando+3) == *(MSK_cerrado+3))
	{
		if (*(comando+4) == *(MSK_cerrado+4))
		{
			MSK_estado &= ~(1<<0x00);
			writeString("Cerrando máscara \n");
		}else if(*(comando+4) == *(MSK_abierto+4)){
			MSK_estado |= (1<<0x00);
			writeString("Abriendo máscara \n");
		}
	}
}

void check_ARM()
{
	//Identificador de comando para posiciones del brazo
	if (*(comando+0) == *(ARM_1+0) &&
		*(comando+1) == *(ARM_1+1) &&
		*(comando+2) == *(ARM_1+2)&&
		*(comando+3) == *(ARM_1+3))
	{
		if (*(comando+4) == *(ARM_1+4))
		{
			MSK_estado &= ~(1<<0x01);
		}else if(*(comando+4) == *(ARM_2+4)){
			MSK_estado |= (1<<0x01);
		}
	}
}

void check_SERVO()
{
	//Identificador de comando para cambiar angulo manualmente
	
	uint8_t valor = extraerNumero();
	
	if (valor > 180)
	{
		valor = 180;
	}
	
	if (*(comando+0) == *(BASE_POS+0) &&
	*(comando+1) == *(BASE_POS+1))
	{
		servoBase = valor;
		
		writeString("Base = ");
		writeNumber(servoBase);
		writeString("\n");
	}
	
	if (*(comando+0) == *(CODO_POS+0) &&
	*(comando+1) == *(CODO_POS+1))
	{
		servoCodo = valor;
		
		writeString("Codo = ");
		writeNumber(servoCodo);
		writeString("\n");
	}
	
	if (*(comando+0) == *(MSK_POS+0) &&
	*(comando+1) == *(MSK_POS+1))
	{
		servoMSK = valor;
		
		writeString("Máscara = ");
		writeNumber(servoMSK);
		writeString("\n");
	}
}

uint8_t extraerNumero()
{
	return ((*(comando+2) - '0') * 100) +
	((*(comando+3) - '0') * 10) +
	(*(comando+4) - '0');
}

void check_LED()
{
	//Identificador de comando para encendido de la LED
	if (*(comando+0) == *(LED_OFF+0) &&
	*(comando+1) == *(LED_OFF+1) &&
	*(comando+2) == *(LED_OFF+2)&&
	*(comando+3) == *(LED_OFF+3))
	{
		if (*(comando+4) == *(LED_OFF+4))
		{
			MSK_estado &= ~(1<<0x02);
			writeString("Apagando \n");
			}else if(*(comando+4) == *(LED_ON+4)){
			MSK_estado |= (1<<0x02);
			writeString("Encendiendo \n");
		}
	}
}

void mod_manual()
{	
	//Refrescar DutyCicle para el LED
	uint8_t dutycicle = ADCvalor4;
	updateDutyCicle0A(dutycicle);
	
	//Mapeo de angulos
	uint16_t angle1;
	uint16_t angle2;
	uint16_t angle3;
	angle1 = (ADCvalor1 * 180)/255;
	angle2 = (ADCvalor2 * 180)/255;
	angle3 = (ADCvalor3 * 180)/255;
	
	//Colocacion de angulo
	setServoAngle2A(angle1);
	setServoAngle2B(angle2);
	setServoAngle1A(angle3); //Estos tienen el mismo angulo ya que se mueven en conjunto
	setServoAngle1B(angle3);
	
	_delay_ms(20);	
}

void mod_uart()
{
	setServoAngle1A(servoBase);
	setServoAngle1B(servoBase);
	setServoAngle2A(servoMSK);
	setServoAngle2B(servoCodo);
	
	_delay_ms(20);	
}

void mod_eeprom()
{
	uint8_t temp_MSK = MSK_estado & 0x01;
	uint8_t temp_ARM = MSK_estado & 0x02;
	uint8_t temp_LED = MSK_estado & 0x04;
	
	//Usar comando para buscar la posicion de la mascara en la EEPROM
	if (temp_MSK == (1<<0x00))
	{
		setServoAngle2A(eepromRead(MSK_OPEN));
	}else if (temp_MSK == 0)
	{
		setServoAngle2A(eepromRead(MSK_CLSD));
	}
	
	uint16_t angle_A1P1;
	uint16_t angle_A2P1;
	uint16_t angle_A1P2;
	uint16_t angle_A2P2;
	angle_A1P1 = (eepromRead(ARM1_POS1) * 180)/255;
	angle_A2P1 = (eepromRead(ARM2_POS1) * 180)/255;
	angle_A1P2 = (eepromRead(ARM1_POS2) * 180)/255;
	angle_A2P2 = (eepromRead(ARM2_POS2) * 180)/255;
	
	//Ir a la posicion guardada en EEPROM
	if (temp_ARM == (1<<0x01))
	{
		setServoAngle2B(angle_A1P1);
		setServoAngle1A(angle_A2P1);
		setServoAngle1B(angle_A2P1);
		
	}else if (temp_ARM == 0)
	{
		setServoAngle2B(angle_A1P2);
		setServoAngle1A(angle_A2P2);
		setServoAngle1B(angle_A2P2);
	}
	
	if (temp_LED == (1<<0x02))
	{
		updateDutyCicle0A(255);
	}else if (temp_LED == 0)
	{
		updateDutyCicle0A(0);
	}
}

void guardarPos()
{
	//Guardar la nueva posicion en EEPROM
	if (save_pos == 1)
	{
		eepromWrite(ARM2_POS1, ADCvalor3);
		eepromWrite(ARM1_POS1, ADCvalor2);
	} else if (save_pos == 2)
	{
		eepromWrite(ARM2_POS2, ADCvalor3);
		eepromWrite(ARM1_POS2, ADCvalor2);
	}
	save_pos = 0;
}
/****************************************/
// Interrupt routines
ISR(ADC_vect)
{
	//Cambio entre canales para la lectura
	 switch (canal)
	 {
		 case 7:
			 ADCvalor1 = ADCH;
			 canal = 6;
			 break;

		 case 6:
			 ADCvalor2 = ADCH;
			 canal = 5;
			 break;

		 case 5:
			 ADCvalor3 = ADCH;
			 canal = 4;
			 break;

		 case 4:
			 ADCvalor4 = ADCH;
			 canal = 7;
			 break;
	 }

	 ADMUX = (ADMUX & 0xF0) | canal;

	 ADCSRA |= (1<<ADSC);
}

ISR(USART_RX_vect)
{
	char bufferRX = UDR0;
	
	//Colocar el modo con el UART
	if (bufferRX != '\n' && bufferRX != '\r')
	{
		if (num_received < 9)
		{
			//Guardar comando completo
			*(comando+num_received) = bufferRX;
			num_received++;
		}
		
	}else{
		*(comando+num_received) = '\0';
		
		//Deteccion del modo actual
		check_MOD();
		if (modo == 1 || modo == 2)
		{
			//Deteccion de instrucciones en modo UART
			check_MSK();
			check_ARM();
			check_LED();
			check_SERVO();
		} else {
			check_MOD();
		}
		
		//Limpieza del buffer
		for (uint8_t i = 0; i < 9; i++)
		{
			writeChar(*(comando+i));
			*(comando+i) = '-';
		}
		num_received = 0;
	}
}

ISR(PCINT1_vect)
{
	//Detectar que boton esta presionado
	if (!(PINC & (1<<PINC0)))
	{
		save_pos = 1;
	}
	
	if (!(PINC & (1<<PINC1)))
	{
		save_pos = 2;
	}
}