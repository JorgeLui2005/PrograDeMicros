/*
* Laboratorio3.asm
*
* Creado: 16/02/2026 16:42:39
* Autor : Jorge Dieguez
* Descripción: Contador binario en display con interrupciones
*/
/****************************************/
// Encabezado (Definición de Registros, Variables y Constantes)
.include "M328PDEF.inc"     // Include definitions specific to ATMega328P
.dseg
.org    SRAM_START
//variable_name:     .byte   1   // Memory alocation for variable_name:     .byte   (byte size)

.cseg
.org 0x0000
rjmp START

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
	//Desactivar TX y DX en D0 y D1
	CLR r16
	STS UCSR0B, r16

	//Configuracion de la interrupcion y Timer0
	LDI r16, 0x00
	OUT TCCR0A, r16 //Poner el timer en modo normal

	LDI r16, (1 << CS00)|(1 << CS01)
	OUT TCCR0B, r16 //Prescaler de 64

	LDI r16, 100
	OUT TCNT0, r16 // Valor inicial
	
	LDI r16, (1 << TOIE0)
	STS TIMSK0, r16 //Usar la mascara

	LDI r16, (1 << TOV0)
	STS TIFR0, r16 //Habilitar la bander de OVF

	//Conifguracion Timer a 1MHz
	LDI  R16, 0x80
	STS  CLKPR, R16      //Habilitar cambio
	LDI  R16, 0x04       //Prescaler = 16 (16MHz/16 = 1MHz)
	STS  CLKPR, R16

    // Salidas puerto D
	SBI DDRD, DDD1
	SBI DDRD, DDD2
	SBI DDRD, DDD3
	SBI DDRD, DDD4
	SBI DDRD, DDD5
	SBI DDRD, DDD6
	SBI DDRD, DDD7

	//Configuracion del vector 7 segmentos
	LDI ZH, HIGH(disp7seg<<1)
	LDI ZL, LOW(disp7seg<<1)

	LPM r16, Z
	LSL r16
	OUT PORTD, r16 //Para que muestre 0 inicialmente

	//Limpieza de registros
	CLR r16
	CLR r17
	CLR r18
	CLR r19
	CLR r20
	CLR r21
	CLR r1

	SEI
/****************************************/
// Loop Infinito
MAIN_LOOP:
	LDI ZH, HIGH(disp7seg<<1)
	LDI ZL, LOW(disp7seg<<1)
	ADD ZL, R18
	ADC ZH, R1
	LPM R19, Z
	LSL R19
	OUT		PORTD, R19
    RJMP    MAIN_LOOP

/****************************************/
// NON-Interrupt subroutines

/****************************************/
// Interrupt routines
ISR_TIMER0:
	LDI r21, 100
	STS TCNT0, r21
	INC R17
	CPI r17, 100
	BRNE SALIR
	CLR r17
	INC R18
	ANDI r18, 0x0F
SALIR:
	RETI
/****************************************/