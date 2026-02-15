/*
* Poslaboratorio2.asm
*
* Creado: 14/02/2026 18:40:09
* Autor : Jorge Dieguez
* Descripción: contador hexadecimal
*/
/****************************************/
// Encabezado (Definición de Registros, Variables y Constantes)
.include "M328PDEF.inc"     // Include definitions specific to ATMega328P
//.dseg
//.org    SRAM_START
//variable_name:     .byte   1   // Memory alocation for variable_name:     .byte   (byte size)

.cseg
.org 0x00

rjmp START

disp7seg:
	.db 0x40, 0x79, 0x24, 0x30,	0x19, 0x12,	0x02, 0x78, 0x00, 0x10, 0x08, 0x03, 0x46, 0x21, 0x06, 0x0E
 /****************************************/
// Configuración de la pila
START:
LDI     R16, LOW(RAMEND)
OUT     SPL, R16
LDI     R16, HIGH(RAMEND)
OUT     SPH, R16
/****************************************/
// Configuracion MCU
SETUP:
	//Desactivar TX y RX en D0 y D1
	CLR r16
	STS UCSR0B, r16

	//Configuracion para el clock
	LDI  R16, 0x80
	STS  CLKPR, R16 //Se carga a CLKPR para habilitar el cambio
	LDI  R16, 0x04       
	STS  CLKPR, R16

	//Configuracion del timer0 
	LDI R16, 0b00000011 //dividir por 16
	OUT TCCR0B, R16
	LDI R16, 100 //Valor inicial para timer
	OUT TCNT0, R16
    
	//Configurar las salidas del puerto D
	SBI DDRD, DDD1
	SBI DDRD, DDD2
	SBI DDRD, DDD3
	SBI DDRD, DDD4
	SBI DDRD, DDD5
	SBI DDRD, DDD6
	SBI DDRD, DDD7

	//Configurar las salidas del puerto B
	SBI DDRB, DDB0
	SBI DDRB, DDB1
	SBI DDRB, DDB2
	SBI DDRB, DDB3

	SBI DDRC, DDC2
	CBI PORTC, PORTC2

	//Configurar entradas
	CBI DDRC, DDC0  
	CBI DDRC, DDC1 
	CBI PORTC, PORTC0
	CBI PORTC, PORTC1

	//Limpieza de registros
	CLR r17
	CLR r18  
	CLR r19
	CLR r20
	CLR r21
	CLR r22
	CLR r30

	//Hacer que la lista empiece en la primera posicion
	LDI ZH, HIGH(disp7seg<<1)
	LDI ZL, LOW(disp7seg<<1)

	//Empezar con el Display apagado
	LPM r17, Z
	LSL r17
	OUT PORTD, r17
	
	CLR r17

	//Empezar con las LEDs apgadas
	OUT PORTB, r17
/****************************************/
// Loop Infinito
MAIN_LOOP:
	IN   R20, PINC
	ANDI R20, 0b00000011
	CPI  R20, 0
	BREQ TIMER
	RJMP CONTADOR

TIMER:
	IN   r16, TIFR0
	SBRS r16, TOV0 //Para salto si hay overflow
	RJMP MAIN_LOOP
	SBI  TIFR0, TOV0
	LDI  r16, 100 //carga valor inicial
	OUT  TCNT0, R16 
	INC  r17 // Se incrementa el registro para contar las veces que se llega a Overflow
	CPI  r17, 100 //Comparacion para ver si ya realizo las 100
	BRNE TIMER 
	CLR  r17 
	INC  r18 
	ANDI r18, 0x0F
	CP   r22, r18 // Se comparan los contadores del los botones y LEDs para ver si hay coincidencia
	BREQ COMPARACION
	OUT  PORTB, r18
	RJMP MAIN_LOOP

CONTADOR:
	//Se usa para decidir si se ira a sumar o a restar
	CPI  R20, 0b00000001
	BREQ SUMAR_LOOP
	CPI  R20, 0b00000010
	BREQ RESTAR_LOOP
	RJMP MAIN_LOOP

SUMAR_LOOP:
	//Encargado de incrementar un registro para mostrar un numero mayor
	LDI ZH, HIGH(disp7seg<<1)
	LDI ZL, LOW(disp7seg<<1)
	INC R22
	ANDI R22, 0x0F
	ADD ZL, R22
	ADC ZH, R24
	LPM R23, Z
	LSL R23
	OUT PORTD, R23
	CALL VERIFICACION
	RJMP MAIN_LOOP
	
RESTAR_LOOP:
	// Resta, es en encargado de decrecer el registro para mostrar un numero menor
	LDI ZH, HIGH(disp7seg<<1)
	LDI ZL, LOW(disp7seg<<1)
	DEC R22
	ANDI R22, 0x0F
	ADD ZL, R22
	ADC ZH, R24
	LPM R23, Z
	LSL R23
	OUT PORTD, R23
	CALL VERIFICACION
	RJMP MAIN_LOOP

COMPARACION:
	// Se encarga de hacer el toggle de la LED de alarma y limpia el contador de LEDs
	OUT  PORTB, r18
	SBI  PINC, PINC2
	CLR  r18
	RJMP MAIN_LOOP
/****************************************/
// NON-Interrupt subroutines
VERIFICACION:
	//Verifica que no haya botones presionados
	IN   R19, PINC
	ANDI R19, 0b00000011 //Usamos una mascara para dejar solo los bits del 0 al 4
	CPI  R19, 0b00000000
	BRNE VERIFICACION
	RET
/****************************************/
// Interrupt routines

/****************************************/