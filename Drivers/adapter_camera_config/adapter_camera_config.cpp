/**resouce file adapter camera config 
		Tran Van Luu
		8/8/2016
*/
#define __DEBUG__ 1
#ifndef __MODULE__ 
#define __MODULE__ "adapter_camera_config.h"
#endif

#include "adapter_camera_config.h"
#include "button_driver.h"
#include "gpio.h"
#include "RCSwitch.h"
#include <string.h>
#include "di_debounce.h" 

#include "user_debug.h"
#include "debug_console.h"
#include "serial_debug_config.h"
#include "hm_serial_uart8.h"

static 	APSDataConfigurationHandle_t *pData;
uint8_t temp = 0;
uint8_t counter_t = 0;
uint8_t flag_ = 0;
uint8_t temp_reset = 0;
/* micros */
static __IO uint32_t us_Tick = 0;
static uint32_t timeLockTick = 0;
static uint32_t Flag = 0;
 void tick_Micros(void)
{
	us_Tick++;
}
 uint32_t micros(void){
	
	return us_Tick*10 + TIM4->CNT;
}

/*delaymicros()*/

void delayMicroSeconds(__IO uint32_t times){
	/*Every Timer Ick 10us*/
	times = times/10 + 1;
	timeLockTick = micros();
	while(micros() - timeLockTick < times)
		{
		}
}
/* low lever button*/
void STBButton_LL_Init(STBButtonHandle_t *phandle)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	if(phandle != NULL)
	{
		//GPIO_InitStruct.Pin = BUTTON_Pin;
		//GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
		//GPIO_InitStruct.Pull = GPIO_NOPULL;
		//HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}

void STBButton_LL_Start(STBButtonHandle_t *phandle)
{
	
}

void STBButton_LL_Stop(STBButtonHandle_t *phandle)
{
	
}

uint8_t STBButton_LL_GetPin(STBButtonHandle_t *phandle)
{
	
	return HAL_GPIO_ReadPin(BUTTON_GPIO_Port,BUTTON_Pin);
}


void Core_Init(APSCore_Process_t * phandle)
{
	phandle->state = PROCESS_TASK_STATE_IDLE;
	phandle->stateFlash = PROCESS_TASK_FLASH_IDLE;
	phandle->stateRelay = PROCESS_TASK_RELAY_IDLE;
	phandle->bnt_Handle = NULL;

	phandle->rf_io_Handle = NULL;


	phandle->flag = 0;
	phandle->time_tick = milis();
	phandle->time_tick_relay = milis();
}
void Core_Start(APSCore_Process_t * phandle)
{
	if(phandle->state == PROCESS_TASK_STATE_IDLE )
	{
		phandle->state = PROCESS_TASK_STATE_CONTROL;
	}
}


static void Core_Process_Control(APSCore_Process_t * const phandle, void *pbounceAc)
{	
	pData = phandle->pDataFlash;
	Bounce_T *bounceAc = (Bounce_T*)pbounceAc;
	//TODO
	uint8_t state = Bounce_Read(bounceAc); // check xem state cua bounceAc co o muc 1 ko
	
	if((phandle->flag != 0)||(phandle->rf_io_Handle->value != 0)||(state == 1))
	{
		switch(phandle->stateRelay)
		{
			case PROCESS_TASK_RELAY_IDLE:
			{

				if((pData->config.rfValue == phandle->value_receive)||(phandle->rf_io_Handle->value != 0)||(state == 1))
					//if(pData->config.rfValue == phandle->value_receive)
					//{
						debug("recevice true:: \r\n");
						ACTIVE_OUT(1);
						TEST_ACTIVE_OUT(1);
						LED_GREEN(1);
						BUZZ(0);
						phandle->stateRelay = PROCESS_TASK_RELAY_BUSY;
						phandle->time_tick_relay = milis();
					//}
			}
				break;
			case PROCESS_TASK_RELAY_BUSY:
			{
				if(milis() - phandle->time_tick_relay > (phandle->pDataFlash->config.tick_time_out_s*1000))
				{
					ACTIVE_OUT(0);
					TEST_ACTIVE_OUT(0);
					BUZZ(1);
					++Flag;
					if(Flag >= ((phandle->pDataFlash->config.tick_time_out_m*60)/phandle->pDataFlash->config.tick_time_out_s))  //check minute
					{
						LED_GREEN(0);
						phandle->stateRelay = PROCESS_TASK_RELAY_IDLE;
						phandle->flag = Flag = 0;

						phandle->rf_io_Handle->value = 0;						
					}
					phandle->time_tick_relay = milis();
				}
			}
				break;
		}
	}
	
/*	if(pData->config.reset_value == 5)
	{
		debug("POWER ON PUCK\n");
			POWER_ON_PUCK(1);
	}*/
}


void Core_Process_Task(APSCore_Process_t * phandle,STBButtonHandle_t * button_Handle, STBButtonHandle_t * rf_io_Handle, Bounce_T *bounceAc, APSDataConfigurationHandle_t *pData)
{
	phandle->bnt_Handle = button_Handle;
	phandle->rf_io_Handle = rf_io_Handle;
	phandle->pDataFlash = pData;
	

	switch(phandle->state)
		{
			case PROCESS_TASK_STATE_CONTROL:
			{
				LED_RED(1);
				if(phandle->bnt_Handle->value == GPIO_PIN_RESET)
				{
					flag_ = 1;
					if(milis() - phandle->time_tick > INTERVAL)
					{
						temp_reset++;
						counter_t = 0;
						LED_RED(0);
						BUZZ(0);
						phandle->state = PROCESS_TASK_STATE_LEARNING;
						debug("PROCESS_TASK_STATE_LEARNING: %d \r\n", phandle->bnt_Handle->value);
						phandle->time_tick = milis();
					}
					if(temp_reset == 4) // check if equal 12(s)
					{
						phandle->reset_value  = 4;
						//TODO 
					}
				}
				else
				{
					// TODO					
					if(flag_ == 1)
					{
						flag_ = 0;
						counter_t++;
						debug("counter %d\n", counter_t);
						if(counter_t >= 5) // neu nhan 5 lan thi cho power on puck se tat  
						{
							phandle->counter_bounceAc = 5;
							POWER_ON_PUCK(1);
							counter_t = 0;
							debug("POWER ON PUCK  ON\n");
						}
					}
					if(phandle->stateFlash == PROCESS_TASK_FLASH_IDLE)
					{
						/*read to EEPROM*/
						APSDataconfig_ReadFlash(phandle->pDataFlash);
						
						phandle->stateFlash =PROCESS_TASK_FLASH_BUSY;
						
					}
					else if(phandle->stateFlash == PROCESS_TASK_FLASH_BUSY)
					{
						if(mySwitch.available())
						{
							debug("ReceivedValue: %x ReceivedBitlength: %x \r\n",mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength());
							phandle->flag++;
							phandle->value_receive = mySwitch.getReceivedValue();
							mySwitch.resetAvailable();
						}
						Core_Process_Control(phandle,bounceAc);
					}
					phandle->time_tick = milis();
				}
			}
				break;
			case PROCESS_TASK_STATE_LEARNING:
			{
				if(phandle->bnt_Handle->value == GPIO_PIN_RESET)
				{
					if(milis() - phandle->time_tick > INTERVAL)
					{
						temp_reset++;
						LED_GREEN(0);
						BUZZ(0);
						phandle->state = PROCESS_TASK_STATE_CONTROL;
						phandle->stateFlash = PROCESS_TASK_FLASH_IDLE;
						debug("PROCESS_TASK_STATE_CONTROL: %d \r\n", phandle->bnt_Handle->value);
						
						phandle->time_tick = milis();
					}
				}
				else
				{
					// TODO
						uint32_t temp = 0;
						if(mySwitch.available())
							{
								debug("ReceivedValue: %x ReceivedBitlength: %x \r\n",mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength());
								phandle->pDataFlash->config.rfValue = temp = mySwitch.getReceivedValue();
								/*write to EEPROM*/
								APSDataconfig_WriteFlash(phandle->pDataFlash);								
								mySwitch.resetAvailable();
							}
							static uint32_t time_tick = 0;
							if(milis() - time_tick > TOGGLE_LED)
							{
								TOGGLE_LED_GREEN;
								time_tick = milis();
							}
							APSDataconfig_ReadFlash(phandle->pDataFlash);
							if(phandle->pDataFlash->config.rfValue  == temp)
							{
								BUZZ(1);
							}
							phandle->time_tick = milis();
				}
			}
				break;
		}
}

void Core_ConfData(APSCore_Process_t * phandle)
{
	char *result;
	if(phandle == NULL)
		return;
	if (phandle->reset_value == 4)
	{
		/* code */
		BUZZ(0);
		HAL_Delay(100);
		BUZZ(1);
		HAL_Delay(100);	
		BUZZ(0);
		HAL_Delay(100);
		BUZZ(1);		
		phandle->pDataFlash->config.reset_value = 0;
		phandle->reset_value = 0;
		debug("factory reset power on puck : %d write to EEPROM OK \r\n",phandle->reset_value);
		APSDataconfig_WriteFlash(phandle->pDataFlash);
		//TODO
		NVIC_SystemReset();	
	}
	if (phandle->counter_bounceAc == 5)
	{
		/* code */
		phandle->pDataFlash->config.reset_value = phandle->counter_bounceAc;
		debug("value power on puck : %d write to EEPROM OK \r\n",phandle->reset_value);
		APSDataconfig_WriteFlash(phandle->pDataFlash);		
		phandle->counter_bounceAc = 0;
	}

	if(console_read(&result) > 0)
	{
		debug("You input: %s\n", result);
		if(strstr(result,"key") != NULL)
		{
			char* pchar = strchr( (char*)result, ' ');
			if( pchar != NULL )
				{
					char* pchar0 = strchr( pchar + 1, '\0');
					if( pchar0 != NULL )
					{
						*pchar0 = 0;
						uint16_t key = atoi(pchar + 1);
						phandle->pDataFlash->config.key = key;
						debug("factory reset: %d write to EEPROM OK \r\n",key);
						APSDataconfig_WriteFlash(phandle->pDataFlash);
						APSDataconfig_Init(phandle->pDataFlash);
					}
				}			
		}
		else if((strstr(result,"TimeOnRelay") != NULL)||(strstr(result,"timeonrelay") != NULL))
		{
			char* pchar = strchr( (char*)result, ' ');
			if( pchar != NULL )
				{
					char* pchar0 = strchr( pchar + 1, '\0');
					if( pchar0 != NULL )
					{
						*pchar0 = 0;
						uint16_t timeOff = atoi(pchar + 1);
						phandle->pDataFlash->config.tick_time_out_s = timeOff;
						/*write to EEPROM*/
						APSDataconfig_WriteFlash(phandle->pDataFlash);

						// update to data
						


						debug("timeOnRelay: %d write to EEPROM OK \r\n",phandle->pDataFlash->config.tick_time_out_s);
					}
				}			
		}
		else if((strstr(result,"TimeDisableRF") != NULL)||((strstr(result,"timedisablerf") != NULL)))
		{
			char* pchar = strchr( (char*)result, ' ');
			if( pchar != NULL )
				{
					char* pchar0 = strchr( pchar + 1, '\0');
					if( pchar0 != NULL )
					{
						*pchar0 = 0;
						uint16_t timeOn = atoi(pchar + 1);
						phandle->pDataFlash->config.tick_time_out_m = timeOn;
						/*write to EEPROM*/
						APSDataconfig_WriteFlash(phandle->pDataFlash);
						debug("timeDisableRF: %d write to EEPROM OK\r\n",phandle->pDataFlash->config.tick_time_out_m);						
					}
				}			
		}
	}
}
void Core_Retart(APSCore_Process_t * phandle)
{
	//TODO
}

