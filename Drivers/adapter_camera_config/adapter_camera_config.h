#ifndef __H_APAPTER_CAMERA_CONFIG_H_
#define __H_APAPTER_CAMERA_CONFIG_H_

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "tim.h"
#include "button_driver.h"
#include "stm32f1xx_hal.h"
#include "adapter_camera_data_conf.h"
#include "di_debounce.h" 
	 
#define POWER_ON_IN(x) do{HAL_GPIO_WritePin(POWER_ON_IN_GPIO_Port, POWER_ON_IN_Pin, x == true ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define READ_BUTTON HAL_GPIO_ReadPin(BUTTON_GPIO_Port,BUTTON_Pin) == true ? GPIO_PIN_SET : GPIO_PIN_RESET
#define LED_GREEN(x) do{HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, x == true ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define TOGGLE_LED_GREEN  do{HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);}while(0)
#define LED_RED(x) do{HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, x == true ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define TOGGLE_LED_RED  do{HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);}while(0)
#define BUZZ(x) do{HAL_GPIO_WritePin(BUZZ_GPIO_Port, BUZZ_Pin, x == true ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define ON_OFF_CHARGER(x) do{HAL_GPIO_WritePin(ON_OFF_CHARGER_GPIO_Port, ON_OFF_CHARGER_Pin, x == true ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define POWER_ON_PUCK(x) do{HAL_GPIO_WritePin(POWER_ON_PUCK_GPIO_Port, POWER_ON_PUCK_Pin, x == true ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define ACTIVE_OUT(x) do{HAL_GPIO_WritePin(ACTIVE_OUT_GPIO_Port, ACTIVE_OUT_Pin, x == true ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define TEST_ACTIVE_OUT(x) do{HAL_GPIO_WritePin(TEST_ACTIVE_OUT_GPIO_Port, TEST_ACTIVE_OUT_Pin, x == true ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)

///#define ACTIVE_IN(x)do{HAL_GPIO_WritePin(ACTIVE_IN_GPIO_Port, ACTIVE_IN_Pin, x == true ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)

#define INTERVAL 4000 //ms
#define TIME_OUT_RELAY 20000  
#define TOGGLE_LED 100 //ms learning
/*milis()*/
#define milis() HAL_GetTick()
/* RF315 define */
#define HM_BSP_RF315_TIM_TICK_US_TIMER					( &htim4 )
#define HM_BSP_RF315_CPU_CLOCK       			SystemCoreClock
#define HM_BSP_RF315_DATA_PIN_HIGH() 			HAL_GPIO_WritePin( GPIOA, GPIO_PIN_11, GPIO_PIN_SET )
#define HM_BSP_RF315_DATA_PIN_LOW()  			HAL_GPIO_WritePin( GPIOA, GPIO_PIN_11, GPIO_PIN_RESET )

void delayMicroSeconds(uint32_t times);
uint32_t micros(void);
void tick_Micros(void);
enum state_relay{
	PROCESS_TASK_RELAY_IDLE = 0,
	PROCESS_TASK_RELAY_BUSY
};
enum{
	PROCESS_TASK_FLASH_IDLE = 0,
	PROCESS_TASK_FLASH_BUSY
};
enum{
	PROCESS_TASK_STATE_IDLE = 0,
	PROCESS_TASK_STATE_LEARNING,
	PROCESS_TASK_STATE_CONTROL
};
typedef struct _task{
	uint8_t flag;
	uint8_t state;
	uint8_t stateFlash;
	state_relay stateRelay;
	uint32_t button_value;
	uint32_t value_receive;
	uint32_t time_tick;
	uint32_t time_tick_relay;
	
	uint32_t reset_value;
	uint32_t counter_bounceAc;
	STBButtonHandle_t * bnt_Handle;

	STBButtonHandle_t * rf_io_Handle;


	APSDataConfigurationHandle_t *pDataFlash;
}APSCore_Process_t;

void Core_Init(APSCore_Process_t * phandle);
void Core_Start(APSCore_Process_t * phandle);

void Core_Process_Task(APSCore_Process_t * phandle,STBButtonHandle_t * button_Handle, STBButtonHandle_t * rf_io_Handle,Bounce_T *bounceAc, APSDataConfigurationHandle_t *pData);
void Core_Retart(APSCore_Process_t * phandle);
void Core_ConfData(APSCore_Process_t * phandle);
static void Core_Process_Control(APSCore_Process_t * phandle, void *pbounceAc);

#ifdef __cplusplus
 }
#endif
#endif	