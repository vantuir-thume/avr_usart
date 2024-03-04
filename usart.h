/* ===========================================================================
	
	BIBLIOTECA USART AVR

    ARQUIVO usart0.h       

    MCU:   Atmega328p
    Clock: 16MHz Cristal Externo

	Compilador: Microchip Studio 7.0.2542
	Autor: Vantuir Thume
	Data de criação: 22/02/2024
	Última atualização: 22/02/2024

============================================================================ */

#ifndef USART_H_
#define USART_H_

// ============================================================================
// --- Frequência do MCU ---
#define   F_CPU   16000000UL


// ============================================================================
// --- Bibliotecas ---
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


// ============================================================================
// --- Protótipo das Funções USART ---
void usart_init(uint16_t bps, uint8_t fast);
void usart_write(char byte_tx);
void usart_clear_buffer();
void usart_print(char *str);
void usart_println(char *str);
char usart_read();
void usart_byte(uint16_t byte);
void PrintBinary(uint16_t value);
uint16_t usart_cnt();


// ============================================================================
// --- Constantes Auxiliares ---
#define  SIZEBUFF  64

#endif /* ADC_H_ */


