/*
 * fpga.c
 *
 *  Created on: Aug 21, 2024
 *      Author: marrob
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static SPI_HandleTypeDef *_hspi;

/* Private function prototypes -----------------------------------------------*/
void xxx(void);
uint8_t Read(void);

/* Private user code ---------------------------------------------------------*/

void FPGA_Init(SPI_HandleTypeDef *hspi)
{
  _hspi = hspi;

  HAL_GPIO_WritePin(DIAG_CS_N_GPIO_Port, DIAG_CS_N_Pin, GPIO_PIN_RESET);
  for(int addr = 0; addr < 59; addr++)
  {
    Read();
  }
   HAL_GPIO_WritePin(DIAG_CS_N_GPIO_Port, DIAG_CS_N_Pin, GPIO_PIN_SET);

  xxx();
}



uint8_t Read(void)
{
  uint8_t response[] = {0};
  asm("nop");
  HAL_SPI_Receive(_hspi, response, sizeof(response), 100);
  asm("nop");
  printf("%02X\n",response[0]);
}

void xxx(void)
{
  uint8_t resp = 0x00;

/*
  for(int i = 0 ; i < 5; i++)
  {
    uint8_t addr = 4;
    resp = FPGA_ReadU8(addr);
    printf("addr: 0x%02X, resp: 0x%02X\n", addr, resp);
    printf("----\n");
  }
  */
/*
  FPGA_ReadU8(0x00, 0x00);
  FPGA_ReadU8(0x01, 0x00);
  FPGA_ReadU8(0x3, 0x00);
  FPGA_ReadU8(0x06, 0x00);
  FPGA_ReadU8(0x0C, 0x00);
  FPGA_ReadU8(0x00, 0x00);
*/

  /*
  for(int addr = 0; addr < 7; addr++)
  {
    resp = FPGA_ReadU8(0x03, addr);
    printf("addr: 0x%02X, resp: 0x%02X\n", addr, resp);
    printf("----\n");
  }
*/

}



uint8_t FPGA_ReadU8(uint8_t inst, uint16_t address)
{
  uint8_t header[5] = {0};
  uint8_t response[5] = {0};

  header[0]  = inst; //0x03
  header[1]  = (uint8_t)(address >> 8);
  header[2]  = (uint8_t)(address);
  header[3]  = 0x00;

  HAL_GPIO_WritePin(DIAG_CS_N_GPIO_Port, DIAG_CS_N_Pin, GPIO_PIN_RESET);

  asm("nop");
  HAL_SPI_TransmitReceive(_hspi, header, response, sizeof(header), 100);
  asm("nop");


  HAL_GPIO_WritePin(DIAG_CS_N_GPIO_Port, DIAG_CS_N_Pin, GPIO_PIN_SET);


  for(int i = 0; i < sizeof(header); i++)
    printf("%02X", header[i]);
  printf("\n");

  for(int i = 0; i < sizeof(response); i++)
    printf("%02X", response[i]);
  printf("\n");



  return response[3];
}


/************************ (C) COPYRIGHT KonvolucioBt ***********END OF FILE****/
