/* ===========================================================================
	
	BIBLIOTECA USART AVR

    ARQUIVO main.c    
    
    MCU:   Atmega328p
    Clock: 16MHz Cristal Externo

	Compilador: Microchip Studio 7.0.2542
	Autor: Vantuir Thume
	Data de criação: 22/02/2024
	Última atualização: 22/02/2024

============================================================================ */


// ============================================================================
// --- Biblioteca ---
#include <stdio.h>
#include "usart.h"
#include "adc.h"


// ============================================================================
// --- Mapeamento de Hardware ---
#define   LED1   (1<<PB5)
#define   LED2   (1<<PB3)

// ============================================================================
// --- Variáveis Globais ---
volatile unsigned long counter = 0;
uint16_t leitura = 0;

#define buffer 10

unsigned int filtro[buffer], media;
unsigned char k = 0;

int original, filtrado;
int numbers[buffer];


uint16_t MovingAverageFilter(uint16_t newValue);
long moving_average(uint16_t value);


// ============================================================================
// --- Função Principal ---
int main()
{  
  DDRB  |=  LED1 |  LED2;
  PORTB &= ~LED1 & ~LED2;
  
  cli();
  adc_init();
  usart_init(9600,0);
  char hex_read[3];
  char print[3];
  
  float RO = 10000.0;		// Resistance at reference temperature (10k)
  float TO = 25.0;			// Reference temperature in Celsius
  float beta = 3950.0;		// Beta value of the thermistor
  
  while(1)
  {
    if (counter <= 160000)
    {
		counter += 1;
    }else
	{
		counter = 0;
		leitura = an_read(2);
		
		//sprintf(hex_read, "%u", leitura);
		//usart_println(hex_read);
		
		//uint16_t filteredValue = MovingAverageFilter(leitura);
		uint16_t filteredValue = moving_average(leitura);
		sprintf(hex_read, "%d", filteredValue);
		usart_print("Analog Read: ");
		usart_println(hex_read);
		
		float R = RO * (1023.0 / filteredValue - 1.0); // Resistance of the thermistor calc
		sprintf(print, "%d", (int)R);
		usart_print("Resistance:  ");
		usart_print(print);
		usart_println(" Ohm");
		
		float tensao = (float)filteredValue * (5.0 / 1023.0);
		
		double temp = 1.0 / ((1.0 / (TO + 273.15)) + (1.0 / beta) * log(R / RO));
		temp -= 273.15;
		
		double inteiro = (int)temp;
		double fracao = modf(temp, &inteiro) * 100;
		
		if (fracao < 0)
		{
			fracao *= -0.1;
		}
		
		sprintf(print, "%d,", (int)inteiro);
		usart_print("Temperature: ");
		usart_print(print);
		sprintf(print, "%d ", (int)fracao);		
		usart_print(print);
		usart_println("*C");
		usart_println("=======================");
		
		inteiro = (int)tensao;
		fracao  = modf(tensao, &inteiro) * 100;
		sprintf(print, "%d,", (int)inteiro);
		usart_print(print);
		sprintf(print, "%d ", (int)fracao);
		usart_print(print);
		usart_println("Volts");
		usart_println("=======================");

		
	    //sprintf(hex_read, "%u", filteredValue);		
		//usart_println(hex_read);
		
		//sprintf(hex_read, "%02X", leitura);
		//usart_println(hex_read);
		
		//PrintBinary(leitura);
	}
	   	  
	  
	char received = 0;
	if(usart_cnt()>0)
	{	
				
		received = usart_read();
	}
	
	if (received == 'L')
	{
		PORTB |= LED1;
		usart_println("Ligado");
		received = 0;
	}
	
	if (received == 'D')
	{
		PORTB &= ~LED1;
		usart_println("Desligado");
		received = 0;
	}
	
	usart_clear_buffer();
	
	  
  } //end while
 
} //end while


// ============================================================================
// Função para calcular a média móvel
uint16_t MovingAverageFilter(uint16_t newValue) 
{
	unsigned char j;
	
	filtro[k] = newValue;
	k++;
	if(k==buffer) k = 0;
	
	media = 0;
	
	for(j=0; j<buffer; j++)
	{
		media += filtro[j];
	}
	
	media /= buffer;
	
	return media;
}

long moving_average(uint16_t value)
{
	for (int i = buffer - 1; i > 0; i--)
	{
		numbers[i] = numbers[i-1];
	}
	
	numbers[0] = value;
	
	media = 0;
	
	for (int i = 0; i < buffer; i++)
	{
		media += numbers[i];
	}
	
	return media/buffer;
}
