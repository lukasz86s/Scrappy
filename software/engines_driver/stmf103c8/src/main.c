/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_conf.h"
#include "init/RCC_init.h"
#include "init/GPIO.h"
#include "init/TIMERS_init.h"
#include "init/stm32f10x_it.h"
#include "init/NVIC_init.h"
#include "init/common.h"
#include "init/USART_init.h"
#include <string.h>

#define ms_1 100
#define dir_pin GPIO_Pin_0
#define puls_pin GPIO_Pin_1


// funkcja przetwarzajaca ramke
void parse_comand(data_buf *rx_buf);

//funkcj przetwarzajaca z char hex do dec
uint8_t hex_char_to_dec(char hex);





int main(void)
{

	// inicjalizacja peryferiów
	RCC_init();
	tim1_init();
	nvic_init();
	gpio_init();
	USART1_init(115200);
	// wy³aczenie jtaga zeby muc uzywac pinB 3 i4
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR = AFIO_MAPR_SWJ_CFG_1;
	// ustawienia pinow portu b w stan niski
	GPIO_ResetBits(GPIOB, dir_pin|puls_pin);
	// inicjalizacja parametrów pracy silników
	init_engines(&engines);



	// inicjalizacja wstepna timerów wartoscia 0
	for(uint8_t i = 0; i < max_timers; i++)
	{
		sys_tims[i] = 0;
	}

	sys_tims[sys_tim1] = 20;

	command_recived = 0;

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	if(SysTick_Config(72000))
	{
		while(1);
	}
	//DMA_Cmd(DMA1_Channel4, ENABLE);
	while(1)
	{

		//pierwszy licznik (test sprawdzajacy przebieg na wyjsciu)
		if(!sys_tims[sys_tim1])
			{
		    static uint8_t position = 0;
			position ^=1;
			if(position){
				//GPIO_SetBits(GPIOB, GPIO_Pin_4);

			}
			else{
				//GPIO_ResetBits(GPIOB, GPIO_Pin_4);

			}

				sys_tims[sys_tim1] = 100;
				//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
				//USART_SendData(USART1, data_cnt);
				//USART_SendData(USART1, data_position);

				if(wsk_rx->data_size > 5)
				{
					parse_comand(find_start_of_frame(wsk_rx));
				}


			}
			//if(command_recived == 15)



	}
}



// funkcja przetwarzajaca ramke
// rx_buf - bufor który wskazuje na pocz¹tek kommendy

void parse_comand(data_buf *rx_buf)
{
	//if(!rx_buf)
		//return;

	// dlugosc samych danych
	int8_t len_of_data = (buf_readbyte(rx_buf)- '0');
	uint8_t function = buf_readbyte(rx_buf);
	//sprawdzanie komendy
	switch(function)
	{
	// ruch silnika
	case '1':
		// while get all data
		while(len_of_data>0)
			{
				len_of_data -= 6;
				// get engine number
				uint8_t nr_engine = (buf_readbyte(rx_buf) - '0');
				//get move direction
				uint8_t dir = (buf_readbyte(rx_buf) - '0');

				uint16_t steps = 0;
				// pobieranie 4 wartosci i konwersja z str(hex)
				for (uint8_t i = 0 ; i<4; i++)
					{
					steps |= ( hex_char_to_dec(buf_readbyte(rx_buf))<<(12-i*4) );
					}
				// set direction output
				GPIO_WriteBit(GPIOB, engines.engine_dir_pins[nr_engine],dir );
				// assign  steps to appropriate engine
				engines.engine_counter[nr_engine] = steps;
			}
			strcpy(DMA_TX_buf, "data ok");
			// wys³anie wiadomosci zwrotnej
			DMA_chan4_send_data(8, NULL);
			// dodac funkcje wykonujaca ruch move_eingin(uint8_t nr_eingine, uint16_t steps)

		break;
	case '2':
			{
				// get nr of engine
				uint8_t nr_engine = (buf_readbyte(rx_buf) - '0');
				// get engine direction
				uint8_t dir = (buf_readbyte(rx_buf) - '0');
				// get continuous vale
				uint8_t continous = (buf_readbyte(rx_buf) - '0');
				// set direction output
				GPIO_WriteBit(GPIOB, engines.engine_dir_pins[nr_engine],dir );
				// set continuous value 1 - work, 0 - stop
				engines.engine_continuous_work[nr_engine] = continous;


			}
		break;
	case '6':
		break;
	}
}

// konwersja jednego znaku char na postac szesnastkow¹
uint8_t hex_char_to_dec(char hex)
{
		if(hex>='0' && hex<='9')return hex-'0';
		if(hex>='a' && hex<='f')return hex-'a'+10;
		if(hex>='A' && hex<='A')return hex-'A'+10;
		return 0;

	// na koncu pozostaje wartosc 0E oznaczajaca koniec ramki ale odpadnie przy czytaniu nastepnej
	//konwersja z string do decimal
	//*value = (((uint8_t)temp[0] - 48)*100 ) + (((uint8_t)temp[1] - 48)*10 ) + ((uint8_t)temp[2] - 48);

}

