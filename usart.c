/* ===========================================================================
	
	BIBLIOTECA USART AVR

    ARQUIVO usart0.c 
    
    MCU:   Atmega328p
    Clock: 16MHz Cristal Externo

	Compilador: Microchip Studio 7.0.2542
    Autor: Vantuir Thume
    Data de cria��o: 22/02/2024
    �ltima atualiza��o: 22/02/2024

============================================================================ */


// ============================================================================
// --- Biblioteca ---
#include "usart.h"


// ============================================================================
// --- Vari�veis Globais ---
volatile uint8_t usart_run = 1;                //1 para usart rodando, 0 para usart ocupada
volatile char rx_buffer[SIZEBUFF] = {0x00};    //buffer para recep��o serial
volatile uint16_t rx_buff = 0;                 //contador de bytes


// ============================================================================
// --- Interrup��es USART ---
ISR(USART_RX_vect)
{
  static uint16_t cnt_buff = 0;                //contador local de endere�o para o buffer rx
  
  rx_buffer[cnt_buff] = UDR0;                  //armazena o byte recebido no endere�o atual do buffer de rx
  rx_buff++;                                   //incrementa o contador global de bytes recebidos
  cnt_buff++;                                  //incrementa o contador local de endere�os para o buffer de rx
  
  if(cnt_buff>=SIZEBUFF)
    cnt_buff = 0;
    	
}

ISR(USART_TX_vect)
{
  usart_run = 1;                               //ocorr�ncia da transmiss�o	
}


// ============================================================================
// --- Desenvolvimento das Fun��es USART ---


// ============================================================================
// --- usart0_init ---
// Inicializa��o da USART
void usart_init(uint16_t bps, uint8_t fast)
{
  
  if(fast)                                     //se fast verdadeiro, double speed
  {
	UCSR0A |= (1<<U2X0);                       //double speed 
	UBRR0 = (F_CPU/(8UL*bps))-1;               //c�lculo do baud rate  
	  
  }
  
  else                                         //sen�o, opera��o normal da USART
  {
	UCSR0A &= ~(1<<U2X0);                      //modo normal
	UBRR0 = (F_CPU/(16UL*bps))-1;              //c�lculo do baud rate para o modo normal  
	  
  }
  
  UCSR0B |= (1<<RXCIE0) |                      //habilita interrup��o para RX
            (1<<TXCIE0) |                      //habilita interrup��o para TX
			(1<<RXEN0)  |                      //habilita recep��o USART
			(1<<TXEN0);                        //habilita transmiss�o da USART
			
			
  UCSR0C = 0x06;                               //USART ass�ncrona, sem paridade, 8 bits de dados, 1 stop bit
  
  sei();                                       //habilita interrup��o global
  
	
} //usart0_init


// ============================================================================
// --- usart0_write ---
// Fun��o para enviar 1 byte por USART
void usart_write(char byte_tx)
{
  while(!usart_run);                            //aguarda usart liberar
  while(!( UCSR0A & (1<<UDRE0)));				//Espera a limpeza do registr. de transmiss�o
  
  usart_run = 0;                                //ocupa usart
  
  UDR0 = byte_tx;
	
} //usart0_write


// ============================================================================
// --- usart0_print ---
// Fun��o para enviar 1 string por USART
void usart_print(char *str)
{
   uint16_t i;
   
   for(i=0;str[i]!='\0';i++)
     usart_write(str[i]);
	 
}

void usart_byte(uint16_t byte)
{
	while(!usart_run);                            //aguarda usart liberar
	while(!( UCSR0A & (1<<UDRE0)));				  //Espera a limpeza do registr. de transmiss�o
	
	usart_run = 0;                                //ocupa usart
	
	UDR0 = byte;
}

void PrintBinary(uint16_t value) 
{
	int i;
	for (i = 15; i >= 0; i--) {
		usart_write((value & (1 << i)) ? '1' : '0');
	}
	usart_write('\n');
}


// ============================================================================
// --- usart0_println ---
// Fun��o para enviar 1 string por USART com o "enter"
void usart_println(char *str)
{
  uint16_t i;
	
  for(i=0;str[i]!='\0';i++)
	usart_write(str[i]);
	
  usart_write(0x0A);
  usart_write(0x0D);	
	
}


// ============================================================================
// --- usart0_read ---
// L� os bytes recebidos via USART
char usart_read()
{
  static uint16_t buff_lo = 0;
  char byte_rx = 0;
  
  byte_rx = rx_buffer[buff_lo];
  buff_lo++;
  rx_buff--;
  
  if(buff_lo>=SIZEBUFF)
    buff_lo = 0;
	
  return byte_rx;
  	
}


// ============================================================================
// --- usart0_cnt ---
// Retorna o n�mero de bytes recebidos at� o momento
uint16_t usart_cnt()
{
  return rx_buff;	
}

// ============================================================================
// --- usart0_clear_biffer ---
// Limpa Buffer de Trasmiss�o
void usart_clear_buffer()
{
	while (UCSR0A & (1 << RXC0))
	{
		char dummy = UDR0;
		(void)dummy;
	}
}
