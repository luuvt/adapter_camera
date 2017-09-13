#ifndef _ADAPTER_CAMERA_DATA_CONFIG__H_
#define _ADAPTER_CAMERA_DATA_CONFIG__H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

#define ADAPTER_CAMERA_ADDRESS		0x01
#define ADAPTER_CAMERA_KEY				0x122
#define ADAPTER_CAMERA_RF_VALUE		0x111
#define ADAPTER_CAMERA_TIME_OUT_S	20   // second
#define ADAPTER_CAMERA_TIME_OUT_M	10  //minute
typedef struct
{
	uint32_t rfValue;
	uint16_t key;
	uint32_t tick_time_out_s;
	uint32_t tick_time_out_m;
	uint32_t reset_value;
}APSDataConfiguration; 
typedef struct 
{
	APSDataConfiguration config;
	void * pData;
}APSDataConfigurationHandle_t;

void APSDataconfig_Init(APSDataConfigurationHandle_t * phandle);
void APSDataconfig_ReadFlash(APSDataConfigurationHandle_t * phandle);
void APSDataconfig_WriteFlash(APSDataConfigurationHandle_t * phandle);
void APSDataconfig_DefaultFlash(APSDataConfigurationHandle_t * phandle);
void APSUpdate_Data(APSDataConfigurationHandle_t *phandle);
#ifdef __cplusplus
}
#endif

#endif // end _ADAPTER_CAMERA_DATA_CONFIG__H_
