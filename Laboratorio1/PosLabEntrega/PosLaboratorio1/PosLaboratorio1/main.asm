/*
* PosLaboratorio1.asm
*
* Creado: 7/02/2026 21:31:29
* Autor : Jorge Dieguez
* Descripción: dos contadores en binario y un contadro que muestra la suma al presionar un boton
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
	//Configuracion del timer
	LDI R16, 0x80
	STS CLKPR, R16      //Se habilita el cambio del oscilador
	LDI R16, 0x04       //Se le divide por 16 a la frecuencia total
	STS CLKPR, R16

	//Configuracion de las salidas
		//Configurar los pines del puerto C del 0 al 3
		SBI DDRC, DDC0
		SBI DDRC, DDC1
		SBI DDRC, DDC2
		SBI DDRC, DDC3

		//Configurar los pines del puerto B del 0 al 3
		SBI DDRB, DDB0
		SBI DDRB, DDB1
		SBI DDRB, DDB2
		SBI DDRB, DDB3
		SBI DDRB, DDB4

		//Configurar los pines del puerto D del 2 al 5
		SBI DDRD, DDD2
		SBI DDRD, DDD3
		SBI DDRD, DDD4
		SBI DDRD, DDD5

	//Configuracion de las entradas
		//Botones de sumar y restar 1 (contador)
		CBI DDRC, DDC4
		CBI PORTC, PORTC4 //Desactivar Pull-up interno
		CBI DDRC, DDC5
		CBI PORTC, PORTC5

		//Botones de sumar y restar 2 (contador)
		CBI DDRD, DDD6
		CBI PORTD, PORTD6
		CBI DDRD, DDD7
		CBI PORTD, PORTD7

		//Boton para sumar ambos contadores
		CBI DDRD, DDD1
		CBI PORTD, PORTD1

	//Limpieza de registros
	CLR r16
	CLR r17
	CLR r18  
	CLR r19
	CLR r20
	CLR r21
	CLR r22
	CLR r23
	CLR r24
	CLR r25

	OUT PORTC, r16 //Inicio con LEDs apagados
	OUT PORTB, r16
	OUT PORTD, r16
	
/****************************************/
// Loop Infinito
MAIN_LOOP:
	IN   r16, PINC //Lectura del PINC donde estan las entradas para CONT1
	ANDI r16, 0b00110000 //Se deja solo los bits relevantes
	IN   r17, PIND // Lectura del PIND donde estan las entradas para CONT2 y el SUMADOR
	ANDI r17, 0b11000010
	ADD  r16, r17 //Se unen ambos vectores para detectar cualquier boton presionado
	CALL DELAY
	IN   r22, PINC //Lectura del PINC donde estan las entradas para CONT1
	ANDI r22, 0b00110000 //Se deja solo los bits relevantes
	IN   r23, PIND // Lectura del PIND donde estan las entradas para CONT2 y el SUMADOR
	ANDI r23, 0b11000010
	ADD  r22, r23 //Se unen ambos vectores para detectar cualquier boton presionado
	CP   r16, r22
	BRNE MAIN_LOOP
	CPI  r16, 0b00000000
	BREQ MAIN_LOOP
	CONT_SUMA:
		CPI  r16, 0b00010000
		BRNE CONT_REST
		INC  r20
		ANDI r20, 0b00001111
		OUT  PORTC, r20
		RJMP VERIFICACION //Verifica que el boton que estaba presionado ya no lo este para seguir
	CONT_REST:
		CPI  r16, 0b00100000
		BRNE CONT2_SUMA
		DEC  r20
		ANDI r20, 0b00001111
		OUT  PORTC, r20
		RJMP VERIFICACION
	CONT2_SUMA:
		CPI  r16, 0b01000000
		BRNE CONT2_REST
		INC  r21
		ANDI r21, 0b00001111
		OUT  PORTB, r21
		RJMP VERIFICACION
	CONT2_REST:
		CPI  r16, 0b10000000
		BRNE SUMADOR
		DEC  r21
		ANDI r21, 0b00001111
		OUT  PORTB, r21
		RJMP VERIFICACION
	SUMADOR:
		OUT  PORTD, r25
		CPI  r16, 0b00000010
		BRNE MAIN_LOOP
		ANDI r20, 0b00001111
		ANDI r21, 0b00001111
		MOV  r24, r20
		ADD  r24, r21
		MOV  r27, r24
		ANDI r27, 0b00010000
		CPI  r27, 0b00010000
		BRNE SALIDA_SUMAR
		LSL  r24
		LSL  r24
		ANDI r24, 0b00111100
		OUT  PORTD, r24
		LDI  r26, 0b00010000
		ADD  r26, r21
		OUT  PORTB, r26
		RJMP VERIFICACION
	SALIDA_SUMAR:
		LSL  r24
		LSL  r24
		ANDI r24, 0b00111100
		OUT  PORTD, r24
		RJMP VERIFICACION
/****************************************/
// NON-Interrupt subroutines
VERIFICACION:
	IN   r18, PINC
	ANDI r18, 0b00110000
	IN   r19, PIND
	ANDI r19, 0b11000010
	ADD  r18, r19
	CPI  r18, 0b00000000
	BRNE VERIFICACION
	RJMP MAIN_LOOP
DELAY:
	LDI     R30, 255 //Valor variable dependiendo del tiempo deseado de delay
LOOP_DELAY:
    DEC     R30
    BRNE    LOOP_DELAY
    RET
/****************************************/
// Interrupt routines

/****************************************/