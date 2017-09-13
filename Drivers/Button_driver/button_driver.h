/**
  ******************************************************************************
  * @file    STBButton.h
  * @author  luuvt
  * @version V1.0.0
  * @date    03-06-2016
  * @brief   Header file of STBButton module.
  ****************************************************************************** 
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STBButton_H_
#define __STBButton_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdint.h"

#define STB_BUTTON_MAX_DELAY 200 // ms
#define STB_TIME_TICK 1000 // ms
enum
{
	STB_STATUS_READY,
	STB_STATUS_BUSY
};

enum
{
	STB_BUTTON_VALUE_HIGH,
	STB_BUTTON_VALUE_LOW
};

enum
{
	STB_BUTTON_EVENT_ON = 0,
	STB_BUTTON_EVENT_OFF
};

enum
{
	STB_BUTTON_STATE_IDLE = 0,
	STB_BUTTON_STATE_START,
	STB_BUTTON_STATE_BUSY
};

typedef struct _STBButtonHandle_t STBButtonHandle_t;
typedef void (*STBButtonCB_t)(STBButtonHandle_t *phandle, uint8_t value);

struct _STBButtonHandle_t
{
	uint8_t state;
	uint8_t value;
	uint32_t isr_count;
	uint32_t time_tick;
	void *pdata;
	STBButtonCB_t cb;
	uint8_t time_value;
};

void STBButton_Init(STBButtonHandle_t *phandle);
void STBButton_DelInit(STBButtonHandle_t *phandle);
void STBButton_Start(STBButtonHandle_t *phandle);
void STBButton_Stop(STBButtonHandle_t *phandle);
uint8_t STBButton_IsBusy(STBButtonHandle_t *phandle);

void STBButton_ProcessISR(STBButtonHandle_t *phandle);
void STBButton_ProcessTask(STBButtonHandle_t *phandle);
uint8_t STBButton_GetValue(STBButtonHandle_t *phandle);

// rf io 
void STBRf_Io_Start(STBButtonHandle_t *phandle);
void STBRf_Io_ProcessTask(STBButtonHandle_t *phandle);

//Low Level
void STBButton_LL_Init(STBButtonHandle_t *phandle);
void STBButton_LL_Start(STBButtonHandle_t *phandle);
void STBButton_LL_Stop(STBButtonHandle_t *phandle);
uint8_t STBButton_LL_GetPin(STBButtonHandle_t *phandle);

#ifdef __cplusplus
 }
#endif
 
#endif


