/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "stm32f10x.h"
#include "common.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	//Petla zliczaj¹ca po koleii timery w dó³
	for(uint8_t i = 0; i < MAX_TIMERS; i++)
	{
		// jesli wiekszy od 0 to -1 i przypisz wartosc do timera
		if(sys_tims[i]) --sys_tims[i];
	}


}
// timer wysyla ipulsy do sterownikow silnika


// mapowanie pinow sterujacych na kolejne pozycje w tablicy
uint16_t engine_pins[] = {E0_PIN,
						  E1_PIN,
						  E2_PIN,
						  E3_PIN,
#ifdef ADDITIONAL_STEPER_MOTOR
						  E4_PIN
#endif
						  };
void TIM1_CC_IRQHandler(void)
{


	if(TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
	{
		uint32_t outputs = 0;
		uint16_t steps ;
		uint8_t continuous_work;
		static uint16_t speed[NR_OF_ENGINS];




		// wyczysc flage przerwania
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);

		//if(position) ENGINE_PORT |= 0x00000002;
		//else ENGINE_PORT |= 0x00020000;
		//GPIO_WriteBit(GPIOB, E0_PIN,GPIO_ReadInputDataBit(GPIOB,E0_PIN)^1 ); NR_OF_ENGINS
		for(uint8_t i = 0 ; i<NR_OF_ENGINS; i++)
		{
			//todo: change to pointer
			continuous_work = engines.engine_continuous_work[i];

			// if continuous_work is 1 then only  change the output to the opposite
			if(continuous_work){

				if((speed[i]--) == 0){
					// change output to the opposite
					if(ENGINE_PINS_V & engine_pins[i])
						{
						 outputs |= (engine_pins[i] << 16);

						}
					else
						{
						 outputs |= engine_pins[i];
						}
					// reload speed value
					speed[i] = engines.engine_speed[i] ;
				}
			}
			else{
				steps = engines.engine_counter[i];
				// if there are any steps left
				if(steps)
					{
						// count down until it reaches zero
					if((speed[i]--) == 0)//engine.engine_counter[i] % engine.engine_speed[i] == 0)
					{
						// decrease step
						steps--;
						// assign new value
						engines.engine_counter[i] = steps;

						// change state of output to the opposite
						if(ENGINE_PINS_V & engine_pins[i])
							{
							outputs |= (engine_pins[i] << 16);

							}
						else
							{
							outputs |= engine_pins[i];
							}

						// reload speed value
						speed[i] = engines.engine_speed[i] ;
						//GPIO_WriteBit(GPIOB, engine_pins[i], GPIO_ReadInputDataBit(GPIOB, engine_pins[i])^1);
					}
				}
			}
		}
		// assign output pins
		ENGINE_PORT = outputs;


		// ponowne zaladowanie licznika
		TIM_SetCompare1(TIM1, TIM_GetCapture1(TIM1)+TIMER1_PERIOD);

	}

}


void EXTI0_IRQHandler(void)
{
	// podtwierdz zrodlo przerwan
	//if(EXTI_GetITStatus(EXTI_Line0) != RESET)
		//{
	//	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)r1_servo1 = 100, GPIO_WriteBit(GPIOB, GPIO_Pin_7, SET);
	//	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)r1_servo1 = 1600,GPIO_WriteBit(GPIOB, GPIO_Pin_7, RESET);
	//	}
		// czyszczenie flagi przerwan

	EXTI_ClearITPendingBit(EXTI_Line0);

}

volatile uint8_t data_cnt = 0;
volatile uint8_t data_position = 0;
void USART1_IRQHandler()
{
	// odnotowanie iloœci przychod¹cych danych. nie mozna do dma bo tam przerwania po calkowitej transmisji
	append_rx_data_size(wsk_rx);
	// kasuje zeby przypadkowo flaga nie zosta³a .dma przy odczycie powino skasowac flage. usun¹c po testach !!!!!!!!!!!!!!!!!!
	USART_ClearFlag(USART1, USART_FLAG_RXNE);
#ifdef serial
	// sprawdzenie czy bufor ma dane
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		// tymczasowa zmiena przechowywujaca pobrany bajt
		// odczyt bajtu kasuje flage przerwañ
		uint8_t temp = (char)USART_ReceiveData(USART1);
		// zapisanie danych do bufora odbiorczego
		buf_readbyte(RX_buf, temp);
		// jesli dane == 0x0E to znak ze koniec polecenia;
		if(temp == START_OF_FRAME)
			command_recived = 1; //poinformuj o nowym poleceniu

	}

	// jesli bufor nadawczy  jest pusty to mozna wyslac
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{
		//pobranie wartosci z bufora
		char temp = buf_readbyte(TX_buf);
		if(temp != 255)
			// wysli wiadomosc
			USART_SendData(USART1, temp);

		// jesli bufor pusty wy³a nadawanie
		if( temp == 255)
		{	// wy³acz nadajnik
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		}


	}
#endif
	// sptawdzenie bufora czy ma dane
	//if(DMA_GetITStatus(DMA1_IT_GL4) != RESET)
	//{
	//	data_cnt++;
	//	data_position++;
	//	DMA_ClearFlag(DMA1_IT_GL4);
	//}
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//	{
//		data_cnt++;
//		data_position++;
//		//uint8_t temp = (char)USART_ReceiveData(USART1);
//		USART_ClearFlag(USART1, USART_FLAG_RXNE);
//	}




}
// kana³ 5 pod³aczony pod rx , wywolanie po zakonczeniu zapisu do bufora i rozpoczecia od poczatku (64bity)
void DMA1_Channel5_IRQHandler(void)
{
	//falga zakonczenia trasferu
	DMA_ClearITPendingBit(DMA1_IT_TC5);
	data_cnt++;

}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
