/* ===========================================================================
	
	BIBLIOTECA USART AVR

    ARQUIVO adc.h       

    MCU:   Atmega328p
    Clock: 16MHz Cristal Externo

	Compilador: Microchip Studio 7.0.2542
	Autor: Vantuir Thume
	Data de cria��o: 22/02/2024
	�ltima atualiza��o: 22/02/2024

============================================================================ */

#ifndef ADC_H_
#define ADC_H_

// ============================================================================
// --- Bibliotecas ---
#include <avr/io.h>
#include <avr/interrupt.h>

// ============================================================================
// --- Prot�tipo das Fun��es ---
void adc_init();
uint16_t an_read(uint16_t ch);

#endif /* ADC_H_ */

