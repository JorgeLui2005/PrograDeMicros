/*
 * Laboratorio4.c
 *
 * Created: 6/04/2026 16:41:43
 * Author: Jorge Luis Diéguez Hegel
 * Description: medidor de voltaje
 */
/****************************************/
// Encabezado (Libraries)
#include <avr/io.h>
#include <avr/interrupt.h>

/****************************************/
// Function prototypes
void setup();
void initPC();  
void initADC();
void initTMR0();
void mostrarADC();
void contador8bits();

#define T0Value 0xD8

volatile uint8_t contador = 0;
volatile uint8_t encenderDisp = 0; 
volatile uint8_t ADCvalor = 0;

const uint8_t tabla_disp[16] =
{
	0x40, 0x79, 0x24, 0x30,	0x19, 0x12,	0x02, 0x78, 0x00, 0x10, 0x08, 0x03, 0x46, 0x21, 0x06, 0x0E
};

/****************************************/
// Main Function
int main(void)
{
	cli();
	setup();
	initPC();
	initADC();
	initTMR0();
	//Habilitar interrupciones
	ADCSRA	|= (1<<ADSC) | (1<<ADIE);
	TIMSK0	|= (1<<TOIE0);
	sei();
	PORTB |= (1<<PORTB4)|(1<<PORTB5);
	while (1) 
	{
		contador8bits();
		mostrarADC();
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

void contador8bits()
{
	uint8_t tempB = PORTB; //Estado actual

	tempB &= ~((1 << PB4) | (1 << PB5));
	
	PORTC = 0;

	if (contador & (1 << 0)) PORTC |= (1 << PC5);
	if (contador & (1 << 1)) PORTC |= (1 << PC4);
	if (contador & (1 << 2)) PORTC |= (1 << PC3);
	if (contador & (1 << 3)) PORTC |= (1 << PC2);
	if (contador & (1 << 4)) PORTC |= (1 << PC1);
	if (contador & (1 << 5)) PORTC |= (1 << PC0);
	
	if (contador & (1 << 6)) tempB |= (1 << PB5);
	if (contador & (1 << 7)) tempB |= (1 << PB4);

	PORTB = (PORTB & 0b11001111) | (tempB & 0b00110000);
}

void mostrarADC()
{
	uint8_t tempB = PORTB;
	tempB &= 0b11111100; // SOLO limpiar PB0 y PB1
	
	uint8_t ADCvalorH;
	uint8_t ADCvalorL;
	
	ADCvalorL = (ADCvalor & 0x0F);
	ADCvalorH = ((ADCvalor & 0xF0)>>4);
	
	if (encenderDisp == 1)
	{
		tempB |= (1<<PORTB1);
		tempB |= (1<<PORTB0);
		PORTD = tabla_disp[ADCvalorL];
		tempB &= ~(1<<PORTB0);
	}
	else
	{
		tempB |= (1<<PORTB1);
		tempB |= (1<<PORTB0);
		PORTD = tabla_disp[ADCvalorH];
		tempB &= ~(1<<PORTB1);
	}

	PORTB = tempB;
}

void initPC()
{
	PCICR		|= (1<<PCIE0);
	PCMSK0		|= (1<<PCINT2) | (1<<PCINT3);
}

void initADC()
{
	ADMUX		= 0;
	//Justificacion a la izquierda y seleccion de ADCC
	ADMUX		|= (1<<REFS0) | (1<<ADLAR) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0);
	ADCSRA		= 0;
	//Habilitar ADC y prescaler = 8
	ADCSRA		|= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
}

void initTMR0()
{
	TCCR0A		= 0x00;
	TCCR0B		= 0x00;
	TCCR0B		= (1<<CS02);
	TCNT0		= T0Value;
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

ISR(ADC_vect)
{
	ADCvalor = ADCH;
	ADCSRA	|= (1<<ADSC);
}

ISR(TIMER0_OVF_vect)
{
	TCNT0 = T0Value;
	encenderDisp ^= 1;
}