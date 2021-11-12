/*
 * USART.c
 *
 *  Created on: 14.03.2020
 *      Author: fet
 */
#include "USART_init.h"
#include <string.h>
#define DMA_CONF

data_buf bufor_rx;
data_buf bufor_tx;
data_buf *RX_buf = &bufor_rx ;
data_buf *TX_buf = &bufor_tx;
// tworzenie struktury z buforem dma
volatile data_buf dma_bufor_rx;
volatile data_buf *wsk_rx = &dma_bufor_rx;

char DMA_TX_buf[DMABUFSIZE] = {"b"};
char DMA_RX_buf[DMABUFSIZE] = {""};


void USART1_init(uint32_t USART_Baund_Rate){
	// konfigurowanie uk³adu USAER
	USART_InitTypeDef USART_InitStructure;

#ifdef DMA_CONF
	DMA_InitTypeDef DMA_initStructure;
	// konfiguracja kanalu 4 dma dla  obslugi tx
	DMA_DeInit(DMA1_Channel4);
	// adres preryferiów usart1
	DMA_initStructure.DMA_PeripheralBaseAddr = USART1_BASE|(0x00000001<<2);
	// adres bufora
	DMA_initStructure.DMA_MemoryBaseAddr = (uint32_t)DMA_TX_buf;
	// ustawienie kierunku przesy³ania dst - destination , src -source
	DMA_initStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_initStructure.DMA_BufferSize = DMABUFSIZE;
	// inkremantacja dla peryferii wylaczona dla pamieci wlaczona
	DMA_initStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_initStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	// rozmair przesy³anych danych
	DMA_initStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_initStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_initStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_initStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_initStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4, &DMA_initStructure);

	// kanal 5 dla rx
	DMA_DeInit(DMA1_Channel5);
	DMA_initStructure.DMA_PeripheralBaseAddr = USART1_BASE|(0x00000001<<2);
	DMA_initStructure.DMA_MemoryBaseAddr = (uint32_t)dma_bufor_rx.data;
	DMA_initStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_initStructure.DMA_BufferSize = DMABUFSIZE;
	DMA_initStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_Init(DMA1_Channel5, &DMA_initStructure);
	//w³¹czenie kana³ów dma
	//DMA_Cmd(DMA1_Channel4, ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE);

#endif


	//predkosc tranismisji
	USART_InitStructure.USART_BaudRate = USART_Baund_Rate;

	//dlugosc slowa
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;

	// bit stopu
	USART_InitStructure.USART_StopBits = USART_StopBits_1;

	// kontrola parzystosci
	USART_InitStructure.USART_Parity = USART_Parity_No;

	// kontrola przep³ywu danych
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	// tryb pracy (Rx - odbior, Tx - nadawanie)
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;

	// inicjalizacja
	USART_Init(USART1, &USART_InitStructure);

	//w³aczenie przerwania od bufora odbiorczego
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	// w³¹czenie DMA
	USART_DMACmd(USART1, USART_DMAReq_Rx|USART_DMAReq_Tx, ENABLE);
	// w³acz przerwania od zakoñczenia transmisji
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE);



	// w³aczenie USART
	USART_Cmd(USART1, ENABLE);

}



// zapis do bufora
uint8_t  write_buf(data_buf *buf, char data)
{
	if(buf_full(buf))return false;
	uint8_t temp = (buf->position + buf->data_size) % BUFOR_SIZE;
	buf->data[temp] = data;
	buf->data_size++;
	return true;
}




//sprawdza czy bufor  jest pe³ny
uint8_t buf_full(volatile data_buf *buf)
{
	if(buf->data_size == DMABUFSIZE)
		return true;
	return false;
}
//sprawdza czy  buforjest pusty
uint8_t buf_empty(volatile data_buf *buf)
{
	if(buf->data_size == 0)
		return true;
	return false;
}

// kontrola iloœci danych w buforze , jesli sie przepe³ni wy³¹cz dma
uint8_t append_rx_data_size(volatile data_buf *buf)
{
	//jesli bufor pe³ny nie dodawaj wiêcej danych
	if (buf_full(buf)){
		// wy³¹cz dma
		DMA_Cmd(DMA1_Channel5, DISABLE);
		return 0;
	}
	if(DMA1_Channel5->CCR == DISABLE)
		DMA1_Channel5->CCR = ENABLE;
	buf->data_size++;
	return 1;
}
//funkcja pobierajac jeden bajt z bufora
char buf_readbyte(volatile data_buf *buf)
{
	if(buf_empty(buf))
		return 255;
	char temp = buf->data[buf->position];
	buf->position++;
	buf->position %= DMABUFSIZE;
	buf->data_size--;
	return temp;
}

// funkcj przetwarzj¹ca dane z bufora
// buf - wsk na bufor z danymi
// command - wskaznik na tablice do której zostanie przypisana komenda
void parse_rx_buff(volatile data_buf *buf, char *command)
{
	uint8_t dlen = 0;
	// dopóki  w buforze sa nie przeanalizowane dane, szukaj pocz¹tku ramki ':'
	while(!buf_empty(buf))
	{
		// finding start of frame
		if (buf_readbyte(buf) == START_OF_FRAME)
			break;
	}
	// drugi bit iloœc danych do pobrania
	dlen = buf_readbyte(buf);
	// kopiowanie danych z buforu
	for(uint8_t i = 0; i < dlen; ++i)
	{
		command[i] = buf_readbyte(buf);
	}
}
//zwraca bufor z ustawiona pozycja na poczatek ramki
data_buf *find_start_of_frame (volatile data_buf *rx_buf){
	while(!buf_empty(rx_buf)){
		if(buf_readbyte(rx_buf) == START_OF_FRAME)
			return rx_buf;
	}
	return NULL;
}

// funkcja wysy³acaca dane przez dma
// size -> rozmiar nowych danych
// address -> adres do nowego bufora wpisac NULL jesli ma zostac domyslny
void DMA_chan4_send_data(uint16_t size, uint32_t *address){
	// wylaczenie DMA na czas zmiany ustawien
	DMA_Cmd(DMA1_Channel4, DISABLE);

	// czyszczenie flagi zakoñczenia pracy dma
	DMA_ClearFlag(DMA_ISR_TCIF4);

	// jesli podany addres nowego bufora to podmien aktualny
	if(address!=NULL)
	{
		DMA1_Channel4->CPAR = (uint32_t)address;
	}

	//ustawienie d³ugoœci danych
	DMA_SetCurrDataCounter(DMA1_Channel4, size);
	// za³aczenie ponownie dma kana³ czwarty
	DMA_Cmd(DMA1_Channel4, ENABLE);
	//czekaj az zakonczy transfer
	//while(DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET);

}

int mystrcmp(char *tab1, char *tab2)
{
	// najpierw sprawdzam czy d³ugosci ³añcuchów sa takie same
	if (strlen(tab1) != strlen(tab2))
		return false;
	// sprawdzam czy ³añcuch doszed³ do koñca (null)
	while( *tab1)
	{
		// porównuje ³añcuchy
		if(!(*(tab1++) == *(tab2++)))
			return false;
	}
	return true;
}


