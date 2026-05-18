/*
 * ADCsetup.c
 *
 * Created: 26/04/2026 17:45:58
 *  Author: capid
 */ 
#include "ADCsetup.h"
void initADC(uint8_t justif, uint8_t chanel, uint8_t autoTrig, uint16_t presc)
{
	ADMUX = 0;
	//Modo
	ADMUX |= (1<<REFS0);
	
	//Seleccion de justificacion
	if (justif){
		ADMUX |= (1<<ADLAR);
	} else {
		ADMUX &= ~(1<<ADLAR);
	}
	
	//Seleccion de canal
	switch (chanel){
		case 0:
			ADMUX |= 0x00;
			break;
		case 1:
			ADMUX |= 0x01;
			break;
		case 2:
			ADMUX |= 0x02;
			break;
		case 3:
			ADMUX |= 0x03;
			break;
		case 4:
			ADMUX |= 0x04;
			break;
		case 5:
			ADMUX |= 0x05;
			break;
		case 6:
			ADMUX |= 0x06;
			break;
		case 7:
			ADMUX |= 0x07;
			break;	
		default:
			ADMUX |= 0x00;
			break;
	}

	ADCSRA = 0;//Limpieza
	//Habilitar el ADC
	ADCSRA |= (1<<ADEN) | (1<<ADIE);
	
	if (autoTrig){
		ADCSRA |= (1<<ADATE);
	} else {
		ADCSRA &= ~(1<<ADATE);
	}
	
	//Seleccion de prescaler
	switch (presc){
		case 2:
			ADCSRA |= (1<<ADPS0);
			break;
		case 4:
			ADCSRA |= (1<<ADPS1);
			break;
		case 8:
			ADCSRA |= (1<<ADPS0) | (1<<ADPS1);
			break;
		case 16:
			ADCSRA |= (1<<ADPS2);
			break;
		case 32:
			ADCSRA |= (1<<ADPS0) | (1<<ADPS2);
			break;
		case 64:
			ADCSRA |= (1<<ADPS1) | (1<<ADPS2);
			break;
		case 128:
			ADCSRA |= (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);
			break;
		default :
			ADCSRA &= ~((1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2));
			break;		
	}
}