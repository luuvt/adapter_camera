/**
  ******************************************************************************
  * @file    USB_Device/DFU_Standalone/Inc/usbd_dfu_flash.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    25-June-2015
  * @brief   Header for usbd_dfu_flash.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_IF_H_
#define __FLASH_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define FLASH_IF_START_ADD 0x800fc00
#define FLASH_IF_END_ADD	0x8010000

/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */
uint16_t Flash_If_Init(void);
uint16_t Flash_If_Erase(uint32_t Add);
uint16_t Flash_If_Write(uint8_t *src, uint8_t *dest, uint32_t Len);
uint16_t Flash_If_EraseStart( uint32_t Addr, uint32_t nums );
uint8_t *Flash_If_Read(uint8_t *src, uint8_t *dest, uint32_t Len);
uint16_t Flash_If_DeInit(void);

#ifdef __cplusplus
 }
#endif
 
#endif /* __USBD_DFU_FLASH_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
