/*
 * Laboratorio6.c
 *
 * Created:  20/04/2026 16:53:10
 * Author: Jorge Dieguez
 * Description: 
 */
/****************************************/
// Encabezado (Libraries)
#include <avr/io.h>
#include <avr/interrupt.h>

/****************************************/
// Function prototypes
void initUART();
void writeChar(char caracter);
void writeString(char* string);

/****************************************/
// Main Function
int main(void)
{
	cli();
	initUART();
	sei();
	
	writeString("Hola");
}

/****************************************/
// NON-Interrupt subroutines
void initUART()
{
	//Inicializacion de puertos
	DDRC = 0x0F;
	DDRB = 0x0F;
	
	PORTB = 0;
	PORTC = 0;
	
	UCSR0A = 0;
	
	//Se habilita la interrupcion y RX y TX 
	UCSR0B |= (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	//Asincrono, pariedad deshabilitada, 1 stopbit, 8 data bits
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	
	//Setear UBRR0
	UBRR0 = 103; 
	
}

void writeChar(char caracter)
{
	while(!((UCSR0A)& (1<<UDRE0)));
	UDR0 = caracter;
	
	//Division y salida del codigo ASCII
	uint8_t ASCII_H = ((caracter & 0xF0)>>4);
	uint8_t ASCII_L = (caracter & 0x0F);
	
	PORTB = ASCII_H;
	PORTC = ASCII_L;
}

void writeString (char* string)
{
		for(uint8_t i=0; string[i] != '\0'; i++)
		{
			writeChar(string[i]);
		}
}
/****************************************/
// Interrupt routines
ISR(USART_RX_vect)
{
	char bufferRX = UDR0;
	
	writeChar(bufferRX);
}