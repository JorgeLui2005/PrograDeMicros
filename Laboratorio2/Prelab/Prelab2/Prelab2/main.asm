/*
* Prelab2.asm
*
* Creado: 9/02/2026 16:03:59
* Autor : Jorge Dieguez
* Descripción: contador
*/
/****************************************/
// Encabezado (Definición de Registros, Variables y Constantes)
.include "M328PDEF.inc"     // Include definitions specific to ATMega328P
.dseg
.org    SRAM_START
//variable_name:     .byte   1   // Memory alocation for variable_name:     .byte   (byte size)

.cseg
.org 0x0000
 /****************************************/
// Configuración de la pila
LDI     R16, LOW(RAMEND)
OUT     SPL, R16
LDI     R16, HIGH(RAMEND)
OUT     SPH, R16
/****************************************/
// Configuracion MCU
SETUP:
     //Configuracion para el clock
	LDI  R16, 0x80
	STS  CLKPR, R16      //Se carga a CLKPR para habilitar el cambio
	LDI  R16, 0x04       
	STS  CLKPR, R16
	//Configuracion del timer0 
	LDI R16, 0b00000011 //dividir por 64
	OUT TCCR0B, R16
	LDI R16, 100 //Valor inicial para timer
	OUT TCNT0, R16 

	//SALIDAS
	SBI DDRB, DDB0
	CBI PORTB, PORTB0
	SBI DDRB, DDB1
	CBI PORTB, PORTB1
	SBI DDRB, DDB2
	CBI PORTB, PORTB2
	SBI DDRB, DDB3
	CBI PORTB, PORTB3
	SBI DDRB, DDB4
	CBI PORTB, PORTB4

	//Limpieza de registros
	LDI R17, 0
	LDI R18, 0
	LDI R19, 0

/****************************************/
// Loop Infinito
MAIN_LOOP:
	IN r16, TIFR0
	SBRS r16, TOV0 //Para salto si hay overflow
	RJMP MAIN_LOOP
	SBI TIFR0, TOV0
	LDI r16, 100 //carga valor inicial
	OUT TCNT0, R16 
	INC r17 
	CPI r17, 10 //comparacion para ver si ya realizo las 10
	BRNE MAIN_LOOP 
	CLR r17 
	INC r18 
	ANDI r18, 0b00001111
	OUT PORTB, R18
    RJMP MAIN_LOOP

/****************************************/
// NON-Interrupt subroutines

/****************************************/
// Interrupt routines

/****************************************/