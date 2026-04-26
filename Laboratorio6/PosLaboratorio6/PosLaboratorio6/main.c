/*
 * PosLaboratorio6.c
 *
 * Created:  25/04/2026 19:25:53
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
void writeNumber(uint8_t num);
void initADC();
void outASCII(char caracter);

volatile uint8_t valorADC = 0;
volatile uint8_t queLetra = 0;
/****************************************/
// Main Function
int main(void)
{
	cli();
	initUART();
	initADC();
	ADCSRA	|= (1<<ADSC) | (1<<ADIE);
	sei();
	
	writeString("Para mostrar el valor del potenciometro ingrese a\r\n");
	writeString("Para mostrar el codigo ASCII ingrese b\r\n");
	while(1)
	{
		
	}
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

void initADC()
{
	ADMUX		= 0;
	//Justificacion a la izquierda y seleccion de canal 7
	ADMUX		|= (1<<REFS0) | (1<<ADLAR) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0);
	ADCSRA		= 0;
	//Habilitar ADC y prescaler = 8
	ADCSRA		|= (1<<ADEN) | (1<<ADPS1);
}

void writeChar(char caracter)
{
	while(!((UCSR0A)& (1<<UDRE0)));
	UDR0 = caracter;
}

void writeString (char* string)
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

void outASCII(char caracter)
{
	//Division y salida del codigo ASCII
	uint8_t ASCII_H = ((caracter & 0xF0)>>4);
	uint8_t ASCII_L = (caracter & 0x0F);
	
	PORTB = ASCII_H;
	PORTC = ASCII_L;
}
/****************************************/
// Interrupt routines
ISR(USART_RX_vect)
{
	uint8_t bufferRX = UDR0;
	if (queLetra == 0){
			if (bufferRX == 'a'){
				writeChar('\n');
				writeNumber(valorADC);
			}else if (bufferRX == 'b') {
				writeChar('\n');
				writeString("Ingrese la letra:");
				queLetra = 1;
			}
		} else {
			writeChar('\n');
			writeChar(bufferRX);
			writeString(" = ");
			writeNumber(bufferRX);
			outASCII(bufferRX);
			queLetra = 0;
		}
}

ISR(ADC_vect)
{
	valorADC = ADCH;
	ADCSRA	|= (1<<ADSC);
}