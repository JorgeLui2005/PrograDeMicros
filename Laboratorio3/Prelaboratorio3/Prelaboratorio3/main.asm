/*
* Prelaboratorio3.asm
*
* Creado: 15/02/2026 21:53:18
* Autor : Jorge Diéguez
* Descripción: Contador binario de 4 bits con interrupciones
*/
/****************************************/
// Encabezado (Definición de Registros, Variables y Constantes)
.include "M328PDEF.inc"     // Include definitions specific to ATMega328P
.dseg
.org    SRAM_START
.cseg

.org 0x0000
	rjmp START

.org PCI1addr
	RJMP INT_BOTON

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
	//IN del puerto C para que el estado aterior se calibre correctamente
	IN		r20, PINC

	// Configuracion para la interrupcion
	LDI		R16, (1 << PCIE1) //Para habilitar interrupciones en el PORTC
	STS		PCICR, r16
	
	LDI		r16, (1 << PCINT11)|(1 << PCINT12) //Habilita unicamente los pines 3 y 4
	STS		PCMSK1, R16

	LDI		r16, (1 << PCIF1) //Habilitar la bandera de interrupcion en el PORTC
	STS		PCIFR, r16 

	SEI //Habilitar salidas globalmente

	// Salidas en puerto B
	SBI		DDRB, DDB0
    SBI		DDRB, DDB1
	SBI		DDRB, DDB2
	SBI		DDRB, DDB3

	//Botones en puerto con pull up interno
	CBI		DDRC, DDC3
	CBI		DDRC, DDC4
	SBI		PORTC, PORTC3
	SBI		PORTC, PORTC4

	//Limpieza de registros
	CLR		R16
	CLR		R17

	//Iniciar con todos los LEDs apagados
	OUT		PORTB, R16
/****************************************/
// Loop Infinito
MAIN_LOOP:
	ANDI	r17, 0x0F
	OUT		PORTB, r17
    RJMP    MAIN_LOOP

/****************************************/
// NON-Interrupt subroutines

/****************************************/
// Interrupt routines
INT_BOTON:
	IN		r18, PINC
	MOV		r19, r18
	EOR		r19, r20
	SBRS	r19, 3
	RJMP	RESTAR
	SBRC	r18, 3
	RJMP	RESTAR
	INC		r17

RESTAR:
	SBRS	r19, 4
	RJMP	NUEVO
	SBRC	r18, 4
	RJMP	NUEVO
	DEC		r17

NUEVO: 
	MOV r20, r18
	RETI
/****************************************/