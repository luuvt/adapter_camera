/**
  ******************************************************************************
  * @file    STBButton.h
  * @author  Luuvt
  * @version V1.0.0
  * @date    03-06-2016
  * @brief   Header file of STBButton module.
  ****************************************************************************** 
  */

#include "button_driver.h"
#include "stdlib.h"
#include "gpio.h"
#include "string.h"

#ifdef __STBButton_H_

__weak uint32_t STB_LL_GetTick()
{
	return HAL_GetTick();
}

__weak uint32_t STB_LL_GetDeltaTick(uint32_t tick)
{
	return HAL_GetTick() - tick;
}

__weak uint8_t STBButton_LL_GetPin(STBButtonHandle_t *phandle)
{
	return GPIO_PIN_RESET;
}

__weak void STBButton_LL_Init(STBButtonHandle_t *phandle)
{

}

__weak void STBButton_LL_Start(STBButtonHandle_t *phandle)
{
	
}

__weak void STBButton_LL_Stop(STBButtonHandle_t *phandle)
{
	
}

/**
  * @brief  STBButton_Init
  * @param  None
  * @retval None
  */
void STBButton_Init(STBButtonHandle_t *phandle)
{
	if(phandle != NULL)
	{
		memset(phandle, 0, sizeof(STBButtonHandle_t));
		phandle->state = STB_BUTTON_STATE_IDLE;
		phandle->time_value = 0;
		STBButton_LL_Init(phandle);
	}
}

/**
  * @brief  STBButton_Start
  * @param  None
  * @retval None
  */
void STBButton_Start(STBButtonHandle_t *phandle)
{
	if(phandle != NULL)
	{
		STBButton_LL_Start(phandle);
		phandle->state = STB_BUTTON_STATE_START;
		phandle->value = STBButton_LL_GetPin(phandle);
	}
}

/**
  * @brief  STBRF_IO_Start
  * @param  None
  * @retval None
  */
void STBRf_Io_Start(STBButtonHandle_t *phandle)
{
	if(phandle !=  NULL)
	{
		phandle->state = STB_BUTTON_STATE_START;
		phandle->value = 0;			
	}
}
/**
  * @brief  STBButton_Stop
  * @param  None
  * @retval None
  */
void STBButton_Stop(STBButtonHandle_t *phandle)
{
	if(phandle != NULL)
	{
		phandle->state = STB_BUTTON_STATE_IDLE;
		STBButton_LL_Stop(phandle);
	}
}

/**
  * @brief  STBButton_IsBusy
  * @param  None
  * @retval None
  */
uint8_t STBButton_IsBusy(STBButtonHandle_t *phandle)
{
	if((phandle->state == STB_BUTTON_STATE_IDLE) || 
		((phandle->state == STB_BUTTON_STATE_START) && (phandle->isr_count == 0)))
	{
		return STB_STATUS_READY;
	}
	else
	{
		return STB_STATUS_BUSY;
	}
}

/**
  * @brief  STBButton_ProcessISR
  * @param  None
  * @retval None
  */
void STBButton_ProcessISR(STBButtonHandle_t *phandle)
{
	if(phandle->state != STB_BUTTON_STATE_IDLE)
	{
		phandle->isr_count++;
	}
}

/**
  * @brief  STBButton_GetValue
  * @param  None
  * @retval None
  */
uint8_t STBButton_GetValue(STBButtonHandle_t *phandle)
{
	return phandle->value;
}

/**
  * @brief  STBButton_ProcessTask
  * @param  None
  * @retval None
  */
void STBButton_ProcessTask(STBButtonHandle_t *phandle)
{			
	switch(phandle->state)
	{
		case STB_BUTTON_STATE_START:
		{
			if(phandle->isr_count != 0)
			{
				phandle->time_tick = STB_LL_GetTick();
				phandle->state = STB_BUTTON_STATE_BUSY;
			}
		}
		break;
		
		case STB_BUTTON_STATE_BUSY:
		{
			uint32_t count = phandle->isr_count;
			phandle->isr_count = 0;
			
			if(count == 0)
			{
				uint32_t tmout = STB_LL_GetDeltaTick(phandle->time_tick);
				if(tmout > STB_BUTTON_MAX_DELAY)
				{
					phandle->time_tick = STB_LL_GetTick();
					phandle->value = STBButton_LL_GetPin(phandle);
					phandle->state = STB_BUTTON_STATE_START;

					// cb
					if(phandle->cb != NULL)
					{
						phandle->cb(phandle, phandle->value);
					}
				}
			}
			else
			{
				phandle->value = STBButton_LL_GetPin(phandle);
				phandle->time_tick = STB_LL_GetTick();
			}
		}
		break;
	}
}

/**
  * @brief  STBRf_Io_ProcessTask
  * @param  None
  * @retval None
  */
void STBRf_Io_ProcessTask(STBButtonHandle_t *phandle)
{
	if(phandle->isr_count != 0)
	{
		phandle->value = phandle->isr_count;
		phandle->isr_count = 0;
	}		
}

#endif


