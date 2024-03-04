/* ===========================================================================
	
	BIBLIOTECA USART AVR

    ARQUIVO adc.c       

    MCU:   Atmega328p
    Clock: 16MHz Cristal Externo

	Compilador: Microchip Studio 7.0.2542
	Autor: Vantuir Thume
	Data de criação: 22/02/2024
	Última atualização: 22/02/2024

============================================================================ */

#include "adc.h"

// ============================================================================
// --- Variáveis Globais ---
volatile uint8_t adc_done = 1;

// ============================================================================
// --- Interrupção ---
ISR(ADC_vect)
{
	adc_done = 1;
	
} //end ISR


// ============================================================================
// --- adc_init ---
void adc_init()
{
	ADMUX  |= (1<<REFS0);            //configura tensão de referência interna de 5V
	ADCSRA |= (1<<ADEN) |            //habilita conversão AD
	(1<<ADIE) |                      //habilita interrupção na conversão AD
	(0b111<<ADPS0);                  //fator de divisão 128 (para o Fclk de 16MHz)
	
} //end adc_init


// ============================================================================
// --- an_read ---
uint16_t an_read(uint16_t ch)
{
	
	adc_done = 0;	                 //limpa a flag de conversão AD pronta, para iniciar nova conversão
	
	DIDR0 |= (1<<ch);                //desligamos o buffer do canal utilizado, para poupar energia
	ADMUX &= 0xF0;                   //limpa o nibble menos significativo do ADMUX
	ADMUX |= ch;                     //atualiza o canal de conversão AD
	
	ADCSRA |= (1<<ADSC);	         //inicia nova conversão
	
	while(!adc_done);                //aguarda a interrupção do conversor AD
	
	//return ADC;
	return (ADCL | (ADCH << 8));
	
} //end an_read