#include "adapter_camera_data_conf.h"
#include "stm32f1xx_flash_if.h"
#include <string.h>
#include "adapter_camera_config.h"

#define EP_ADDRESS  0xfc00  //0x10000 - 0x400 

APSDataConfiguration config_Buffer;
/**
  * @brief  APSDataconfig_Init
  * @param  None
  * @retval None
  */
void APSDataconfig_Init(APSDataConfigurationHandle_t *phandle)
{
	if(phandle == NULL)
		return;
	APSDataconfig_ReadFlash(phandle);
	if(phandle->config.reset_value == 5)
	{
			POWER_ON_PUCK(1);		
	}
	if(phandle->config.key !=ADAPTER_CAMERA_KEY)
	{
		APSDataconfig_DefaultFlash(phandle);
	}
}
/**
  * @brief  APSDataconfig_ReadFlash
  * @param  None
  * @retval None
  */
void APSDataconfig_ReadFlash(APSDataConfigurationHandle_t *phandle)
{
	if(phandle == NULL)
		return;

	Flash_If_Read((uint8_t*)EP_ADDRESS,(uint8_t*)&phandle->config, sizeof(APSDataConfiguration));
}
/**
  * @brief  APSDataconfig_WriteFlash
  * @param  None
  * @retval None
  */
void APSDataconfig_WriteFlash(APSDataConfigurationHandle_t * phandle)
{
	if(phandle == NULL)
		return;

	Flash_If_Read((uint8_t*)EP_ADDRESS,(uint8_t*)&config_Buffer, sizeof(APSDataConfiguration));
	if (memcmp (&config_Buffer, &phandle->config, sizeof(APSDataConfiguration)) != 0)
	{
		Flash_If_Init();
		Flash_If_EraseStart (EP_ADDRESS, 1);
		Flash_If_Write ((uint8_t*)&phandle->config, (uint8_t*) EP_ADDRESS, sizeof(APSDataConfiguration));
		Flash_If_DeInit();
	}	
}
/**
  * @brief  APSDataconfig_WriteFlash
  * @param  None
  * @retval None
  */
/*void APSUpdate_Data(APSDataConfigurationHandle_t *phandle)
{
	if(phandle == NULL)
		return;
	
	APSDataConfiguration * pconfig = &phandle->config;
	pconfig->tick_time_out_s = 
}*/
/**
  * @brief  APSDataconfig_DefaultFlash
  * @param  None
  * @retval None
  */
void APSDataconfig_DefaultFlash(APSDataConfigurationHandle_t *phandle)
{
	if(phandle == NULL)
		return;

	APSDataconfig_ReadFlash(phandle);
	APSDataConfiguration * pconfig = &phandle->config;

	pconfig->key = ADAPTER_CAMERA_KEY;
	pconfig->rfValue = ADAPTER_CAMERA_RF_VALUE;	
	pconfig->tick_time_out_s = ADAPTER_CAMERA_TIME_OUT_S;
	pconfig->tick_time_out_m = ADAPTER_CAMERA_TIME_OUT_M;
	APSDataconfig_WriteFlash(phandle);
}