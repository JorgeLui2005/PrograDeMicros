/*
* AssemblerApplication1.asm
*
* Creado: 1/02/2026 19:45:41
* Autor : Jorge Luis Dieguez Hegel
* Descripción: prelaboratorio 1, contador en assembler de 4 bits 
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
//Configurando los bits 1 y 0 del PIND para que sean entradas
	CBI DDRD, DDD2 //entrada para restar
	CBI DDRD, DDD1 //entrada para sumar
//Desactivar el pullup interno de ambos bits del puerto
	CBI PORTD, PORTD2
    CBI PORTD, PORTD1
//Configuracion de salidas
	SBI DDRC, DDC0
	CBI PORTC, PORTC0
	SBI DDRC, DDC1
	CBI PORTC, PORTC1
	SBI DDRC, DDC2
	CBI PORTC, PORTC2
	SBI DDRC, DDC3
	CBI PORTC, PORTC3
//Limpieza de los registros a utilizar
	CLR r17
	CLR r18  
	CLR r19
	OUT PORTC, r17 //Inicio con LEDs apagados
/****************************************/
// Loop Infinito
MAIN_LOOP:
	IN r18, PIND //Se lee el PIND
	ANDI r18, 0b00000110 //Se deja solo a los bits 0 y 1
	CPI r18, 0b00000000
	BREQ MAIN_LOOP
	CALL DELAY // mecanismo anti rebote con un delay y una segunda lectura
	CALL DELAY
	CALL DELAY
	IN r19, PIND
	ANDI r19, 0b00000110 //Se vuelve a dejar solo los bits relevantes
	CP r19, r18
	BRNE MAIN_LOOP
	MOV r18, r19
SUMAR_LOOP:
	CPI r19, 0b00000010 //Verificar si el boton presionado es el sumador
	BRNE RESTAR_LOOP
	INC r17 //Se incrementa en 1 al contador
	ANDI r17, 0b00001111 //Se limpian los bits sin utilizar para dejar solo los cuatro que saldran
	OUT PORTC, r17 //Se manda la señal al puerto
    RJMP VERIFICACION
RESTAR_LOOP:
	CPI r19, 0b00000100 //Verificar si el boton presionado es el de resta
	BRNE MAIN_LOOP
	DEC r17 //Se le resta 1 al contador
	ANDI r17, 0b00001111 //Se limpian los bits sin utilizar para dejar solo los cuatro que saldran
	OUT PORTC, r17 //Se manda la señal al puerto
    RJMP VERIFICACION
/****************************************/
// NON-Interrupt subroutines
DELAY:
    LDI     R20, 255 //Valor variable dependiendo del tiempo deseado de delay
LOOP_DELAY:
    DEC     R20
    BRNE    LOOP_DELAY
    RET
VERIFICACION:
	IN R18, PIND
	ANDI R18, 0b00000110 //Usamos una mascara para dejar solo los bits 4 y 5
	CALL DELAY
	CPI R18, 0b00000010
	BREQ VERIFICACION
	CPI R18, 0b00000100
	BREQ VERIFICACION
	RJMP MAIN_LOOP
/****************************************/