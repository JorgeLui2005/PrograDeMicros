/*
 * UARTsetup.c
 *
 * Created: 22/04/2026 16:13:56
 *  Author: capid
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "UARTsetup.h"

void initUART()
{
	//Pines PD0 y PD1
	DDRD &= ~(1<<DDD0);
	DDRD |= (1<<DDD1);
	
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
}

void writeString(char* string)
{
	for(uint8_t i=0; string[i] != '\0'; i++)
	{
		writeChar(string[i]);
	}
}

void writeNumber(uint8_t num)
{
	char buffer[4]; //Ya que el numero mas grande es de 3 digitos + '\0'
	
	uint8_t i = 0;
	if(num == 0)
	{
		//Caso especial para cuando el numero es 0
		writeChar('0');
		return;
	}

	while(num > 0)
	{
		//Se le suma una posicion a i con el fin de recorrer todo buffer
		//Se obtiene el utlimo digito y se le suma a '0' para convertir a ASCII
		buffer[i++] = (num % 10) + '0';
		//Se elimina el ultimo digito para guardar el siguiente
		num /= 10;
	}
	// Se invierte el buffer ya que se guardo el numero al reves
	for(int8_t j = i-1; j >= 0; j--)
	{
		writeChar(buffer[j]);
	}
}