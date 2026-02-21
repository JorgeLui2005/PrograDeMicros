/*
* Poslaboratorio3.asm
*
* Creado: 19/02/2026 22:27:53
* Autor : Jorge Diéguez
* Descripción: contador a 60 con display 
*/
/****************************************/
// Encabezado (Definición de Registros, Variables y Constantes)
.include "M328PDEF.inc"     // Include definitions specific to ATMega328P
.dseg
.org    SRAM_START
//variable_name:     .byte   1   // Memory alocation for variable_name:     .byte   (byte size)

.cseg
.org 0x0000
.equ TIMER0_VALOR = 100
RJMP START

.org OVF0addr
	RJMP ISR_TIMER0

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
	CLI

	//Conifguracion Timer a 1MHz
	LDI  R16, 0x80
	STS  CLKPR, R16      //Habilitar cambio
	LDI  R16, 0x04       //Prescaler = 16 (16MHz/16 = 1MHz)
	STS  CLKPR, R16

    //Desactivar TX y RX en D0 y D1
	CLR r16
	STS UCSR0B, r16

	//Configuracion de la interrupcion y Timer0
	LDI r16, 0x00
	OUT TCCR0A, r16 //Poner el timer en modo normal

	LDI r16, (1 << CS00)|(1 << CS01)
	OUT TCCR0B, r16 //Prescaler de 64

	LDI r16, TIMER0_VALOR
	OUT TCNT0, r16 // Valor inicial
	
	LDI r16, (1 << TOIE0)
	STS TIMSK0, r16 //Usar la mascara

	LDI r16, (1 << TOV0)
	STS TIFR0, r16 //Habilitar la bander de OVF

    // Salidas puerto D
	SBI DDRD, DDD1
	SBI DDRD, DDD2
	SBI DDRD, DDD3
	SBI DDRD, DDD4
	SBI DDRD, DDD5
	SBI DDRD, DDD6
	SBI DDRD, DDD7

	// Salidas del Puerto B
	SBI DDRB, DDB0
	SBI DDRB, DDB1

	//Configuracion del vector 7 segmentos
	LDI ZH, HIGH(disp7seg<<1)
	LDI ZL, LOW(disp7seg<<1)

	LPM r16, Z
	LSL r16
	OUT PORTD, r16 //Para que muestre 0 inicialmente

	//Limpieza de registros
	CLR		r16
	CLR		r17
	CLR		r18
	CLR		r19
	CLR		r20
	CLR		r21
	CLR		r1

	SEI
/****************************************/
// Loop Infinito
MAIN_LOOP:
	CALL	MOSTRAR_UNI
	SBI		PORTB, PORTB0
	OUT		PORTD, r20
	CALL	DELAY
	CBI		PORTB, PORTB0

	CALL	MOSTRAR_DEC
	SBI		PORTB, PORTB1
	OUT		PORTD, R20
	CALL	DELAY
	CBI		PORTB, PORTB1
    RJMP    MAIN_LOOP

/****************************************/
// NON-Interrupt subroutines
MOSTRAR_UNI:
	LDI		ZH, HIGH(disp7seg<<1)
	LDI		ZL, LOW(disp7seg<<1)
	ADD		ZL, R18
	ADC		ZH, R1
	LPM		R20, Z
	LSL		r20
	RET

MOSTRAR_DEC:
	LDI		ZH, HIGH(disp7seg<<1)
	LDI		ZL, LOW(disp7seg<<1)
	ADD		ZL, R19
	ADC		ZH, R1
	LPM		R20, Z
	LSL		r20
	RET

DELAY:
    LDI R22, 200
	LOOP_DELAY:
		DEC R22
		BRNE LOOP_DELAY
		RET
/****************************************/
// Interrupt routines
ISR_TIMER0:
	PUSH	R16
	IN		R16, SREG
	PUSH	R16

	LDI		r21, TIMER0_VALOR
	STS		TCNT0, r21
	INC		R17
	CPI		r17, 100
	BRNE	SALIR
	CLR		r17
	INC		R18
	CPI		r18, 10
	BRNE	SALIR
	CLR		R18
	INC		R19
	CPI		R19, 6
	BRNE	SALIR
	CLR		R19

SALIR:
	POP		R16
	OUT		SREG, R16
	POP		R16
	RETI
/****************************************/