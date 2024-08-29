/*
 * uart_com.c
 *
 *  Created on: Feb 29, 2024
 *      Author: marrob
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static UART_HandleTypeDef *_uart;
extern Device_t Device;

char  UartRxBuffer[UART_BUFFER_SIZE];
char  UartTxBuffer[UART_BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void Parser(char *request, char *response);
static void TxTask(void);
static void RxTask(void);
/* Private user code ---------------------------------------------------------*/
void UartCom_Init(UART_HandleTypeDef *uart, DMA_HandleTypeDef *dma)
{
  _uart = uart;

  if(HAL_UART_Receive_DMA(_uart, (uint8_t*)UartRxBuffer, UART_BUFFER_SIZE)!= HAL_OK)
    Device.Diag.UartErrorCnt++;
  __HAL_DMA_DISABLE_IT(dma, DMA_IT_HT);
}

void UartCom_Task(void)
{
  TxTask();
  RxTask();
}

static void Parser(char *request, char *response)
{
  char cmd[UART_CMD_LENGTH];
  char arg1[UART_ARG_LENGTH];
  //char arg2[UART_ARG_LENGTH];

  sscanf(request, "%s", cmd);

  /*--------------------------------------------------------------------------*/
  /*--------------------- Generic --------------------------------------------*/
  /*--------------------------------------------------------------------------*/
  if(!strcmp(cmd, "*IDN?")){
    sprintf(response, "%s", DEVICE_NAME);
  }
  else if(!strcmp(cmd, "*OPC?")){
    strcpy(response, "*OPC");
  }
  else if(!strcmp(cmd, "FW?")){
    sprintf(response, "FW? %s", DEVICE_FW);
  }
  else if(!strcmp(cmd, "VER?")){
    sprintf(response, "%s", DEVICE_FW);
  }
  else if(!strcmp(cmd, "UID?")){
    sprintf(response, "%4lX%4lX%4lX",HAL_GetUIDw0(), HAL_GetUIDw1(), HAL_GetUIDw2());
  }
  else if(!strcmp(cmd, "PCB?")){
    sprintf(response, "%s", DEVICE_PCB);
  }
  else if(!strcmp(cmd,"UPTIME?")){
     sprintf(response, "%08lX", Device.Diag.UpTimeSec);
  }
  else if(!strcmp(cmd,"DI?")){
     sprintf(response, "%08hX", Device.DI);
  }
  else if(!strcmp(cmd,"DO?")){
     sprintf(response, "%08hX", Device.DO);
  }
  else if(!strcmp(cmd,"UE?")){
    sprintf(response, "%08lX", Device.Diag.UartErrorCnt);
  }
  else if(!strcmp(cmd,"DO")){
    sscanf(request, "%s %s",cmd, arg1);
    Device.DO = strtol(arg1, NULL, 16);
    strcpy(response, "OK");
  }
  /*--------------------------------------------------------------------------*/
  /*--------------------- MRLY240314.FW --------------------------------------*/
  /*--------------------------------------------------------------------------*/
  else if(!strcmp(cmd,"RLY:SET")){
    sscanf(request, "%s %s",cmd, arg1);
    uint16_t index = strtol(arg1, NULL, 16);
    TPICs_Set(index);
    strcpy(response, "OK");
  }
  else if(!strcmp(cmd,"RLY:CLR")){
    sscanf(request, "%s %s",cmd, arg1);
    uint16_t index = strtol(arg1, NULL, 16);
    TPICs_Clr(index);
    strcpy(response, "OK");
  }

  else{
    Device.Diag.UartUnknwonCnt++;
  }
}

static void TxTask(void)
{
  uint8_t txLen = strlen(UartTxBuffer);
  if(txLen != 0)
  {
    UartTxBuffer[txLen] = UART_TERIMINATION_CHAR;
    UartTxBuffer[txLen + 1] = '\0';

    HAL_UART_Transmit(_uart, (uint8_t*) UartTxBuffer, txLen + 1, 100);
    UartTxBuffer[0] = 0;
  }
}

static void RxTask(void)
{
  for(uint8_t i=0; i < UART_BUFFER_SIZE; i++)
  {
    if(UartRxBuffer[i] == UART_TERIMINATION_CHAR)
    {
      HAL_UART_DMAStop(_uart);
      Parser(UartRxBuffer, UartTxBuffer);
      memset(UartRxBuffer, 0x00, UART_BUFFER_SIZE);
      if(HAL_UART_Receive_DMA(_uart, (uint8_t*)UartRxBuffer, UART_BUFFER_SIZE)!= HAL_OK)
        Device.Diag.UartErrorCnt++;
      Device.Diag.TransactionCnt++;
    }
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  Device.Diag.UartErrorCnt++;
  __HAL_UART_CLEAR_PEFLAG(huart);
  __HAL_UART_CLEAR_FEFLAG(huart);
  __HAL_UART_CLEAR_NEFLAG(huart);
  __HAL_UART_CLEAR_OREFLAG(huart);
}

void UART_DMAError(UART_HandleTypeDef *huart)
{
  Device.Diag.UartErrorCnt++;
  __HAL_UART_CLEAR_PEFLAG(huart);
  __HAL_UART_CLEAR_FEFLAG(huart);
  __HAL_UART_CLEAR_NEFLAG(huart);
  __HAL_UART_CLEAR_OREFLAG(huart);
}



/************************ (C) COPYRIGHT KonvolucioBt ***********END OF FILE****/


