/*
# ##############################################################################
# File: main.c                                                                 #
# Project: src                                                                 #
# Created Date: Sunday, June 4th 2023, 7:25:17 pm                              #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Saturday, July 29th 2023, 8:02:33 pm                          #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
# Date      	By          	Comments                                       #
# ##########	########    	################################################
 */

#include "main.h"

void GPIO_Init(void);
void Error_Handler(void);
void UART3_Init(void);
void SystemClockConfig_HSE(uint8_t clock_freq);
void SystemClockConfig_HSI(uint8_t clock_freq);
void CAN1_Init(void);
void CAN1_Tx(uint16_t GPIO_Pin);
void CAN1_Rx(void);
void CAN_Filter_Config(void);

UART_HandleTypeDef huart3;
CAN_HandleTypeDef hcan1;


int main(void)
{	
	HAL_Init();
	
	SystemClockConfig_HSE(SYS_CLOCK_FREQ_50_MHZ);
	
	GPIO_Init();

	UART3_Init();

	CAN1_Init();
	
	CAN_Filter_Config();

	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_BUSOFF) != HAL_OK)
	{
		Error_Handler();
	}
	
	if (HAL_CAN_Start(&hcan1) != HAL_OK)
	{
		Error_Handler();
	}
	
	while(1);
	
    return 0;
}


void SystemClockConfig_HSE(uint8_t clock_freq)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	uint32_t FLatency =0;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE ;
	osc_init.HSEState = RCC_HSE_BYPASS;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	switch(clock_freq)
	{
		case SYS_CLOCK_FREQ_50_MHZ:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 100;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ  = 2;
			osc_init.PLL.PLLR=2;

		    clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
		    					RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		    clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		    clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		    clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		    clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

		    FLatency = FLASH_ACR_LATENCY_1WS;
			break;
		}
		case SYS_CLOCK_FREQ_84_MHZ:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 168;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ  = 2;
			osc_init.PLL.PLLR=2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			 FLatency = FLASH_ACR_LATENCY_2WS;

			break;
		}
		case SYS_CLOCK_FREQ_120_MHZ:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 240;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ  = 2;
			osc_init.PLL.PLLR=2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			FLatency = FLASH_ACR_LATENCY_3WS;
			break;

		}


		case SYS_CLOCK_FREQ_180_MHZ:
		{
			//Enable the clock for the power controller
			__HAL_RCC_PWR_CLK_ENABLE();

			//set regulator voltage scale as 1
			__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

			//turn on the over drive mode of the voltage regulator
			__HAL_PWR_OVERDRIVE_ENABLE();

			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 360;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ  = 2;
			osc_init.PLL.PLLR=2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			FLatency = FLASH_ACR_LATENCY_5WS;
			break;

		}

		default:
			return;

	}

	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_Handler();
	}


	if(HAL_RCC_ClockConfig(&clk_init,FLatency) != HAL_OK)
	{
		Error_Handler();
	}


	//Systick configuration
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);



	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

}

void SystemClockConfig_HSI(uint8_t clock_freq)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	uint32_t FLatency =0;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	osc_init.HSIState = RCC_HSI_ON;
	osc_init.HSICalibrationValue = 16;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	switch(clock_freq)
	{
		case SYS_CLOCK_FREQ_50_MHZ:
		{
			osc_init.PLL.PLLM = 16;
			osc_init.PLL.PLLN = 100;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ  = 2;
			osc_init.PLL.PLLR=2;

		    clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
		    					RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		    clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		    clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		    clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		    clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

		    FLatency = FLASH_ACR_LATENCY_1WS;
			break;
		}
		case SYS_CLOCK_FREQ_84_MHZ:
		{
			osc_init.PLL.PLLM = 16;
			osc_init.PLL.PLLN = 168;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ  = 2;
			osc_init.PLL.PLLR=2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			 FLatency = FLASH_ACR_LATENCY_2WS;

			break;
		}
		case SYS_CLOCK_FREQ_120_MHZ:
		{
			osc_init.PLL.PLLM = 16;
			osc_init.PLL.PLLN = 240;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ  = 2;
			osc_init.PLL.PLLR=2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			FLatency = FLASH_ACR_LATENCY_3WS;
			break;

		}

		default:
			return;

	}

	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_Handler();
	}


	if(HAL_RCC_ClockConfig(&clk_init,FLatency) != HAL_OK)
	{
		Error_Handler();
	}


	//Systick configuration
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void GPIO_Init(void){
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitTypeDef ledgpio;
	ledgpio.Pin = GPIO_PIN_0;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &ledgpio);

	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef button;
	button.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	button.Mode = GPIO_MODE_IT_RISING;
	button.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &button);\
	
	/*Interrupt Priorities for GPIO*/
	HAL_NVIC_SetPriority(EXTI4_IRQn, 15, 0);
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

void UART3_Init(void)
{
	
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	//We actually dont need to configure things that we dont need, code runs
	//normal without configuring the above parameters.
	huart3.Init.Mode = UART_MODE_TX_RX;
	
	if (HAL_UART_Init(&huart3) != HAL_OK)
	{
		Error_Handler();
	}
	
}

void CAN1_Init(void)
{
	hcan1.Instance = CAN1;
	hcan1.Init.Mode = CAN_MODE_NORMAL;
	hcan1.Init.AutoBusOff = ENABLE;
	hcan1.Init.AutoRetransmission = ENABLE;
	hcan1.Init.AutoWakeUp = DISABLE;
	hcan1.Init.ReceiveFifoLocked = DISABLE;
	hcan1.Init.TimeTriggeredMode = DISABLE;
	hcan1.Init.TransmitFifoPriority = DISABLE;

	/*Settings related to CAN bit timings*/
	hcan1.Init.Prescaler = 5;
	hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
	hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;

	/*Initialize the CAN Init*/
	/*This puts the CAN periph in initialization, but not in start mode
	requires the start can function for that*/
	if(HAL_CAN_Init(&hcan1) != HAL_OK)
	{
		Error_Handler();
	}
}

void CAN1_Tx(uint16_t GPIO_Pin)
{
	
	CAN_TxHeaderTypeDef TxHeader;
	uint32_t TxMailBox;
	char our_message[5] = {0};
	
	TxHeader.DLC = 5;
	TxHeader.StdId = 0x65D;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	
	if(GPIO_Pin == GPIO_PIN_4)
  	{
		strcpy(our_message, "LED1");
		if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, (uint8_t *)our_message, &TxMailBox) != HAL_OK)
		{
			Error_Handler();
		}
  	}
  	else if(GPIO_Pin == GPIO_PIN_5)
  	{
		strcpy(our_message, "LED2");
		if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, (uint8_t *)our_message, &TxMailBox) != HAL_OK)
		{
			Error_Handler();
		}
  	}
  	else if(GPIO_Pin == GPIO_PIN_6)
  	{
		strcpy(our_message, "LED3");
		if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, (uint8_t *)our_message, &TxMailBox) != HAL_OK)
		{
			Error_Handler();
		}
  	}
  	else if(GPIO_Pin == GPIO_PIN_7)
  	{
		strcpy(our_message, "LED4");
		if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, (uint8_t *)our_message, &TxMailBox) != HAL_OK)
		{
			Error_Handler();
		}
  	}
}

void CAN1_Rx(void)
{
	CAN_RxHeaderTypeDef RxHeader;
	uint8_t rcvd_msg[5];
	char msg[50];

	/*Normally we would  first monitor the reception of a message using HAL_CAN_GetRxFifoFillLevel() until at least on e message is received
	but in this case we know that we are going to send a message that will for sure be received by our MCU since we are going to send a message
	which passes our receive filter and it will then be in our receive FIFO , and since we have our filters set on FIFO 0 then if the can message 
	passes the filter on FIFO 0 then it will go there, if it passes filters on FIFO 1 then it will go there. In our case we only have fitlers on
	FIFO 0 and nothing on FIFO1, so it will only go to FIFO 0. If a message matches more than one filter*/
	
	/*Wait until theres a message to read*/
	while(!HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0));

	/*At this point, message is in FIFO*/
	/*Get message from FIFO 0 since we are using filter for FIFO 0*/
	/*If you have only configured filter banks for FIFO 1 in the bxCAN peripheral on an STM32 microcontroller, all messages that pass through these 
	filters will indeed be stored in FIFO 1.
	However, any message that does not match the filters configured for FIFO 1 will not automatically be stored in FIFO 0. Instead, such messages will 
	be discarded because no filter bank has been configured to accept them.
	In essence, if a message doesn't match any filter, it is assumed to be irrelevant for the application and is thus discarded, to avoid flooding the 
	FIFOs with unneeded messages. So, you'll need to set up filters for FIFO 0 if there are messages that you want to be accepted into FIFO 0.*/
	
	if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader,rcvd_msg) != HAL_OK)
	{
		Error_Handler();
	}

	sprintf(msg, "Message Received: %s \r\n", rcvd_msg);
	HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void CAN_Filter_Config(void)
{
	CAN_FilterTypeDef can1_filter_init;

	can1_filter_init.FilterActivation = CAN_FILTER_ENABLE;
	can1_filter_init.FilterBank = 0;
	can1_filter_init.FilterFIFOAssignment = CAN_RX_FIFO0;
	can1_filter_init.FilterIdHigh = 0x0000;
	can1_filter_init.FilterIdLow = 0x0000;
	can1_filter_init.FilterMaskIdHigh = 0x0000;
	can1_filter_init.FilterMaskIdLow = 0x0000;
	can1_filter_init.FilterMode = CAN_FILTERMODE_IDMASK;
	can1_filter_init.FilterScale = CAN_FILTERSCALE_32BIT;

	if(HAL_CAN_ConfigFilter(&hcan1, &can1_filter_init) != HAL_OK)
	{
		Error_Handler();
	}
}

/*Callback Functions*/

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	/*Message transmission request has completed*/
	char msg[50];
	sprintf(msg, "Message has been sent: Mailbox 0\r\n");
	HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
	/*Message transmission request has completed*/
	char msg[50];
	sprintf(msg, "Message has been sent: Mailbox 1\r\n");
	HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
	/*Message transmission request has completed*/
	char msg[50];
	sprintf(msg, "Message has been sent: Mailbox 2\r\n");
	HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef RxHeader;
	uint8_t rcvd_msg[5];
	char msg[50];
	
	/*We now have a message in the FIFO, which is ready to be read*/
	/*Get the message from FIFO 0*/

	if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, rcvd_msg) != HAL_OK)
	{
		Error_Handler();
	}

	/*We have received the message*/
	sprintf(msg, "Message Received: %s\r\n", rcvd_msg);
	HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	char msg[50];
	sprintf(msg, "CAN Error Detected\r\n");
	HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_4)
  	{
		HAL_Delay(20);
		if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4) == GPIO_PIN_SET)
		{
			CAN1_Tx(GPIO_Pin);
		}
  	}
  	else if(GPIO_Pin == GPIO_PIN_5)
  	{
		HAL_Delay(20);
		if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5) == GPIO_PIN_SET)
		{
			CAN1_Tx(GPIO_Pin);
		}
  	}
  	else if(GPIO_Pin == GPIO_PIN_6)
  	{
		HAL_Delay(20);
		if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6) == GPIO_PIN_SET)
		{
			CAN1_Tx(GPIO_Pin);
		}
  	}
  	else if(GPIO_Pin == GPIO_PIN_7)
  	{
		HAL_Delay(20);
		if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7) == GPIO_PIN_SET)
		{
			CAN1_Tx(GPIO_Pin);
		}
  	}
}

void Error_Handler(void)
{
    while(1);
}