/*
 * Prelab4.c
 *
 * Created: 5/04/2026 15:32:30
 * Author: Jorge Luis Diéguez Hegel
 * Description: contador de 8 bits en C
 */
/****************************************/
// Encabezado (Libraries)
#include <avr/io.h>
#include <avr/interrupt.h>

/****************************************/
// Function prototypes
void setup();
void initPC();

volatile uint8_t contador = 0;

/****************************************/
// Main Function
int main(void)
{
	cli();
	setup();
	initPC();
	sei();
	
	while (1) 
	{
		PORTC = 0;
		PORTB &= ~((1 << PINB4) | (1 << PINB5)); // Limpiar unicamente estos bits

		if (contador & (1 << 0)) PORTC |= (1 << PC5);
		if (contador & (1 << 1)) PORTC |= (1 << PC4);
		if (contador & (1 << 2)) PORTC |= (1 << PC3);
		if (contador & (1 << 3)) PORTC |= (1 << PC2);
		if (contador & (1 << 4)) PORTC |= (1 << PC1);
		if (contador & (1 << 5)) PORTC |= (1 << PC0);

		if (contador & (1 << 6)) PORTB |= (1 << PB5);
		if (contador & (1 << 7)) PORTB |= (1 << PB4);
	}
	
}
/****************************************/
// NON-Interrupt subroutines
void setup()
{
	//cambiar frecuencia del sistema
	CLKPR		= (1<<CLKPCE);
	CLKPR		= (1<<CLKPS2);
	//CONIFGURAR SALIDAS Y ENTRADAS
	DDRD		= 0xFF;
	PORTD		= 0xFF;
	DDRB		= 0b00110011;
	PORTB		= 0b00001100;
	DDRC		= 0xFF;
	PORTC		= 0x00;
		
	UCSR0B		= 0X00;
}

void initPC()
{
	PCICR		|= (1<<PCIE0);
	PCMSK0		|= (1<<PCINT2) | (1<<PCINT3);
}

/****************************************/
// Interrupt routines
ISR(PCINT0_vect)
{
	if(!(PINB & (1<<PINB2)))
	{
		contador++;
	}
	if(!(PINB & (1<<PINB3)))
	{
		contador--;
	}
}
