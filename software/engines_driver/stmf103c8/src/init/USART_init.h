/*
 * USART.h
 *
 *  Created on: 14.03.2020
 *      Author: fet
 */

#ifndef INIT_USART_INIT_H_
#define INIT_USART_INIT_H_
#include "stm32f10x.h"
#include "common.h"
//------------------------------------------------ bufory komunikacji------------------------------
#define BUFOR_SIZE 30
#define true 1
#define false 0
#define DMABUFSIZE 64

typedef struct USART_buf {
	uint8_t positnio;
	uint8_t data_size;
	char data[BUFOR_SIZE];
} usart_buf;

typedef struct Data_buf {
	int position;
	int data_size;
	char data[DMABUFSIZE];
} data_buf;

typedef struct Command {
	char command[3];
	uint8_t value;

} recive_command;

extern char DMA_TX_buf[DMABUFSIZE];
extern char DMA_RX_buf[DMABUFSIZE];

//wskaznik na bufor dma
volatile data_buf *wsk_rx;

void USART1_init(uint32_t USART_Word_Len);

uint8_t buf_writebyte(volatile data_buf *buf, char data);
void parse_rx_buff(volatile data_buf *buf, char *command);
char buf_readbyte(volatile data_buf *buf);
uint8_t append_rx_data_size(volatile data_buf *buf);

// funkcja szukajaca pocz¹tku ramki i zwracajaca bufor z pocz¹tkiem ramki
data_buf *find_start_of_frame (volatile data_buf *rx_buf);
// funkcja ustawiajaca nowy bufor i d³ugosc danych
void  DMA_chan4_send_data(uint16_t size, uint32_t *address);

uint8_t buf_empty(volatile data_buf *buf);
uint8_t buf_full(volatile data_buf *buf);
//funkcja porównuj¹ca stringi
int mystrcmp(char *tab1, char *tab2);

#endif /* INIT_USART_INIT_H_ */
