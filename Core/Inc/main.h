/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef struct _Devic_t
{
  uint8_t DO;
  uint8_t DI;
  struct _Diag
  {
    uint32_t LcdTimeout;
    uint32_t UartUnknwonCnt;
    uint32_t UartErrorCnt;
    uint32_t UpTimeSec;
    uint32_t TransactionCnt;
    uint32_t BootupCnt;
  }Diag;

}Device_t;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define DEVICE_DEBUG_LEVEL    3

#if (DEVICE_DEBUG_LEVEL > 0)
#define  DeviceUsrLog(...)  {printf(__VA_ARGS__);\
                             printf("\r\n");}
#else
#define DeviceUsrLog(...)
#endif

#if (DEVICE_DEBUG_LEVEL > 1)

#define  DeviceErrLog(...)  {printf("ERROR.DEVICE:") ;\
                             printf(__VA_ARGS__);\
                             printf("\r\n");}
#else
#define DeviceErrLog(...)
#endif

#if (DEVICE_DEBUG_LEVEL > 2)
#define  DeviceDbgLog(...)  {printf("DEBUG.DEVICE:") ;\
                             printf(__VA_ARGS__);\
                             printf("\r\n");}
#else
#define DeviceDbgLog(...)
#endif


#define DEVICE_NAME             "MRLY240314.FW"
#define DEVICE_NAME_SIZE        sizeof(DEVICE_NAME)
#define DEVICE_FW               "1.0.0.0"
#define DEVICE_FW_SIZE          sizeof(DEVICDSD_PCME_FW)
#define DEVICE_PCB              "00"
#define DEVICE_PCB_SIZE         sizeof(DEVICE_PCB)
#define DEVICE_MNF              "KONVOLUCIO"
#define DEVICE_MNF_SIZE         sizeof(DEVICE_MNF)

/*--- UART COM ---*/
#define UART_BUFFER_SIZE    64
#define UART_CMD_LENGTH     35
#define UART_ARG_LENGTH     35
#define UART_TERIMINATION_CHAR  '\r' //0x0D

/*--- MCP3421 ---*/
//MCP3421A0T-E/CH:0xD0
//MCP3421A1T-E/CH:0xD2
#define MCP3421_I2C_DEVICE_ADDRESS  0xD2

/*--- EEPROM MAP ---*/
#define EEPROM_ADDR_FIRST_START  0x0000
#define EEPROM_ADDR_BOOTUP_CNT   0x0004

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

// --- Relays ---
uint8_t Relays_Init(SPI_HandleTypeDef *spi);
void Relays_AllOn(void);
void Relays_AllOff(void);
void Relays_Update(void);

// --- UART COM ---
void UartCom_Init(UART_HandleTypeDef *uart, DMA_HandleTypeDef *dma);
void UartCom_Task(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BTN3_Pin GPIO_PIN_4
#define BTN3_GPIO_Port GPIOA
#define BTN2_Pin GPIO_PIN_5
#define BTN2_GPIO_Port GPIOA
#define BTN1_Pin GPIO_PIN_6
#define BTN1_GPIO_Port GPIOA
#define LIVE_LED_Pin GPIO_PIN_1
#define LIVE_LED_GPIO_Port GPIOB
#define LVL_CMP_Pin GPIO_PIN_2
#define LVL_CMP_GPIO_Port GPIOB
#define DIAG_CS_N_Pin GPIO_PIN_12
#define DIAG_CS_N_GPIO_Port GPIOB
#define FM25_CS_N_Pin GPIO_PIN_8
#define FM25_CS_N_GPIO_Port GPIOA
#define DAIG_BYPS_Pin GPIO_PIN_15
#define DAIG_BYPS_GPIO_Port GPIOA
#define DIAG_RCK_Pin GPIO_PIN_6
#define DIAG_RCK_GPIO_Port GPIOB
#define DIAG_G_N_Pin GPIO_PIN_7
#define DIAG_G_N_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
