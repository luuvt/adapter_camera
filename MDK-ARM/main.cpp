/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#define __DEBUG__ 1
#ifndef __MODULE__ 
#define __MODULE__ "main.cpp"
#endif
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include "adapter_camera_middle_core.h"
#include "adapter_camera_config.h"
#include "adapter_camera_data_conf.h"
#include "stm32f1xx_flash_if.h"
#include "user_debug.h"
#include "debug_console.h"
#include "serial_debug_config.h"
#include "hm_serial_uart8.h"
#include "button_driver.h"
#include "RCSwitch.h"
#include "di_debounce.h" 
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
HmLogUart8ThreadHandle_t hm_log_thread_handle;
APSMiddleLinkHandle_t adapter_diddle_link_handle;
STBButtonHandle_t button_Handle;

STBButtonHandle_t rf_io_Handle;

APSCore_Process_t core_handle;
APSDataConfigurationHandle_t adapter_camera_data_conf_handle;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
RCSwitch mySwitch;
Bounce_T bounceAc;

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	//mySwitch.enabler
  /* USER CODE END 1 */
	mySwitch.enableReceive(RF_DATA_Pin);
  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_IWDG_Init();
  MX_TIM1_Init();

  MX_TIM3_Init();	
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_ADC1_Init();

  /* USER CODE BEGIN 2 */
	HmLogUart8Thread_Init(&hm_log_thread_handle);
	HmLogUart8Thread_RegistryUartHandle(&hm_log_thread_handle, &huart3);
	HmLogUart8Thread_Start(&hm_log_thread_handle);
	debug("start debug \r\n");
	
	/* start pw on in*/

  ON_OFF_CHARGER(1);
	POWER_ON_IN(1);
	POWER_ON_PUCK(0);
	ACTIVE_OUT(0);
	TEST_ACTIVE_OUT(0);
	BUZZ(0);
	HAL_Delay(100);
	BUZZ(1);
	HAL_Delay(100);	
	BUZZ(0);
	HAL_Delay(100);
	BUZZ(1);	
	
	STBButton_Init(&button_Handle);
	STBButton_Init(&rf_io_Handle);
	Bounce_Init(&bounceAc);

	Core_Init(&core_handle);
	APSDataconfig_Init(&adapter_camera_data_conf_handle);
	
	APSMiddleLink_Init(&adapter_diddle_link_handle);
	APSMiddleLink_Registry(&adapter_diddle_link_handle,
																						         &core_handle,
																										 &button_Handle,
																										 &rf_io_Handle,
                                                     &bounceAc,
																										 &adapter_camera_data_conf_handle);
	APSMiddleLink_Start(&adapter_diddle_link_handle);
	
	HAL_TIM_Base_Start_IT( HM_BSP_RF315_TIM_TICK_US_TIMER );

  /*##-3- Start the Input Capture in interrupt mode ##########################*/
	HAL_TIM_Base_Start( &htim3 );
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_3);
	HAL_IWDG_Start(&hiwdg);
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */
	/* USER CODE BEGIN 3 */
		APSMiddleLink_Process(&adapter_diddle_link_handle);
		APSMiddleLink_ConfigData(&adapter_diddle_link_handle);
		HAL_IWDG_Refresh(&hiwdg);
		
			/*ACTIVE_OUT(1);
			HAL_Delay(3000);
			ACTIVE_OUT(0);
			HAL_Delay(3000);*/
  }
  /* USER CODE END 3 */
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == HM_BSP_RF315_TIM_TICK_US_TIMER)
	{
		tick_Micros();
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t PIN)
{
	if(PIN == GPIO_PIN_0)
		mySwitch.handleInterrupt();
	if(PIN == BUTTON_Pin)
		STBButton_ProcessISR(&button_Handle);
}
/**
* @brief This function handles EXTI line0 interrupt.
*/
uint32_t counter = 0;
void RF_Io_ProcessISR(uint8_t counter)
{
	debug("start debug: %d", counter);
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
			//TODO
			counter++;
			RF_Io_ProcessISR(counter);
			STBButton_ProcessISR(&rf_io_Handle);
	}
}

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
