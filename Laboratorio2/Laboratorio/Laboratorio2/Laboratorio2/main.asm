/*
* Laboratorio2.asm
*
* Creado: 9/02/2026 17:03:16
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
	//Desactivar TX y DX en D0 y D1
	CLR r16
	STS UCSR0B, r16

	//Conifguracion Timer a 1MHz
	LDI  R16, 0x80
	STS  CLKPR, R16      //Habilitar cambio
	LDI  R16, 0x04       //Prescaler = 16 (16MHz/16 = 1MHz)
	STS  CLKPR, R16

	//Configuramos el timer0 
	LDI R16, 0b00000011 //Prescaler de I/0 64
	OUT TCCR0B, R16
	LDI R16, 100 
    
	//Configurar las salidas
	SBI DDRD, DDD1
	SBI DDRD, DDD2
	SBI DDRD, DDD3
	SBI DDRD, DDD4
	SBI DDRD, DDD5
	SBI DDRD, DDD6
	SBI DDRD, DDD7

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

	LDI ZH, HIGH(disp7seg<<1)
	LDI ZL, LOW(disp7seg<<1)

	LPM r17, Z
	LSL r17
	OUT PORTD, r17

	CLR r17
/****************************************/
// Loop Infinito
MAIN_LOOP:
   IN R17, PINC
   ANDI R17, 0b00000011
   CPI R17, 0
   BREQ MAIN_LOOP
   CALL DELAY
   IN R18, PINC
   ANDI R18, 0b00000011
   CP R18, R17
   BRNE MAIN_LOOP
   MOV R17, R18

CONTADOR:
	CPI R18, 0b00000001
	BREQ SUMAR_LOOP
	CPI R18, 0b00000010
	BREQ RESTAR_LOOP
	RJMP MAIN_LOOP

SUMAR_LOOP:
	LDI ZH, HIGH(disp7seg<<1)
	LDI ZL, LOW(disp7seg<<1)
	INC R20
	ANDI R20, 0x0F
	ADD ZL, R20
	ADC ZH, R21
	LPM R22, Z
	LSL R22
	OUT PORTD, R22
	CALL VERIFICACION
	
RESTAR_LOOP:
	LDI ZH, HIGH(disp7seg<<1)
	LDI ZL, LOW(disp7seg<<1)
	DEC R20
	ANDI R20, 0x0F
	ADD ZL, R20
	ADC ZH, R21
	LPM R22, Z
	LSL R22
	OUT PORTD, R22
	CALL VERIFICACION
	
/****************************************/
// NON-Interrupt subroutines
DELAY:
    LDI     R30, 255 //Valor variable dependiendo del tiempo deseado de delay
LOOP_DELAY:
    DEC     R30
    BRNE    LOOP_DELAY
    RET
VERIFICACION:
	IN   R19, PINC
	ANDI R19, 0b00000011 //Usamos una mascara para dejar solo los bits del 0 al 4
	CPI  R19, 0b00000000
	BRNE VERIFICACION
	RJMP MAIN_LOOP

/****************************************/
// Interrupt routines

/****************************************/