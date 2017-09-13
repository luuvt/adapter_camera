#ifndef _ADAPTER_CAMERA_MIDDLE_CORE_H_
#define _ADAPTER_CAMERA_MIDDLE_CORE_H_

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

enum{
	APS_MIDDLE_LINK_STATE_IDLE,
	APS_MIDDLE_LINK_STATE_BUSY
};

typedef struct
{
	STBButtonHandle_t *phadapter_button;
	STBButtonHandle_t *phadapter_rfio;
	APSCore_Process_t *phadapter_core;
	Bounce_T *phadapter_bounceAc;
	APSDataConfigurationHandle_t *phadapter_config;

	uint32_t state;
}APSMiddleLinkHandle_t;

void APSMiddleLink_Init(APSMiddleLinkHandle_t *phmidle);
void APSMiddleLink_Registry(APSMiddleLinkHandle_t *phmidle,
															APSCore_Process_t *pcore,
															STBButtonHandle_t *pbutton,
															STBButtonHandle_t *prfio,
															Bounce_T *pbounceAc,
															APSDataConfigurationHandle_t *pconfig);
void APSMiddleLink_Start(APSMiddleLinkHandle_t *phmidle);
void APSMiddleLink_Process(APSMiddleLinkHandle_t *phmidle);
void APSMiddleLink_ResetCore(APSMiddleLinkHandle_t *phmidle);
void APSMiddleLink_ConfigData(APSMiddleLinkHandle_t *phmidle);
#endif