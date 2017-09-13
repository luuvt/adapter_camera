#define __DEBUG__ 1
#ifndef __MODULE__ 
#define __MODULE__ "adapter_camera_middle_core.cpp"
#endif

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
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
/**
  * @brief  APSMiddleLink_Init
  * @param  None
  * @retval None
  */

void APSMiddleLink_Init(APSMiddleLinkHandle_t *phmidle)
{
	memset(phmidle, 0, sizeof(APSMiddleLinkHandle_t));
	phmidle->state = APS_MIDDLE_LINK_STATE_IDLE;
}
/**
  * @brief  APSMiddleLink_Registry
  * @param  None
  * @retval None
  */
void APSMiddleLink_Registry(APSMiddleLinkHandle_t *phmidle,
															APSCore_Process_t *pcore,
															STBButtonHandle_t *pbutton,
															STBButtonHandle_t *prfio,
															Bounce_T *pbounceAc,
															APSDataConfigurationHandle_t *pconfig)
{
	if (phmidle->state == APS_MIDDLE_LINK_STATE_IDLE)
	{
		if (phmidle != NULL&&
				pcore != NULL&&
				pbutton != NULL&&
				prfio != NULL&&
				pbounceAc != NULL&&
				pconfig != NULL)
		{
			/* code */
			phmidle->phadapter_core = pcore;
			phmidle->phadapter_button = pbutton;
			phmidle->phadapter_rfio = prfio;
			phmidle->phadapter_bounceAc = pbounceAc;
			phmidle->phadapter_config = pconfig;
		}
	}
}
/**
  * @brief  APSMiddleLink_Start
  * @param  None
  * @retval None
  */
void APSMiddleLink_Start(APSMiddleLinkHandle_t *phmidle)
{
	if (phmidle->state == APS_MIDDLE_LINK_STATE_IDLE)
	{
		/* code */
		debug("start middle link \r\n");
		
		// TODO
		Core_Start(phmidle->phadapter_core);
		STBButton_Start(phmidle->phadapter_button);
		STBRf_Io_Start(phmidle->phadapter_rfio);
		
		Bounce_Attach(phmidle->phadapter_bounceAc, ACTIVE_IN_GPIO_Port, ACTIVE_IN_Pin);
		//TEST ACTIVE IN 
		//Bounce_Attach(phmidle->phadapter_bounceAc, GPIOA, GPIO_PIN_14);
		Bounce_SetInterval(phmidle->phadapter_bounceAc, 1000);
		
		phmidle->state = APS_MIDDLE_LINK_STATE_BUSY;
	}
}
/**
  * @brief  APSMiddleLink_Process
  * @param  None
  * @retval None
  */
void APSMiddleLink_Process(APSMiddleLinkHandle_t *phmidle)
{
	STBButton_ProcessTask(phmidle->phadapter_button);
	STBRf_Io_ProcessTask(phmidle->phadapter_rfio);
	Bounce_Update(phmidle->phadapter_bounceAc);
	Core_Process_Task(phmidle->phadapter_core,phmidle->phadapter_button, phmidle->phadapter_rfio, phmidle->phadapter_bounceAc, phmidle->phadapter_config);
}

void APSMiddleLink_ResetCore(APSMiddleLinkHandle_t *phmidle)
{
	Core_Retart(phmidle->phadapter_core);
}
/**
  * @brief  APSMiddleLink_ConfigData
  * @param  None
  * @retval None
  */
void APSMiddleLink_ConfigData(APSMiddleLinkHandle_t *phmidle)
{
	Core_ConfData(phmidle->phadapter_core);
}