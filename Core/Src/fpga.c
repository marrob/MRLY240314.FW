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
}


uint8_t reverse_bits(uint8_t byte)
{
    byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4; // Fel- és alsó nibble megcserélése
    byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2; // Bitek 2-es csoportban megcserélése
    byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1; // Bitek egyesével megcserélése
    return byte;
}
/*
 * Bypass módban nem lehet a regisztereket olvasni!
 */
void FPGA_ReadReg(uint8_t *buffer, size_t size)
{
  HAL_GPIO_WritePin(DIAG_CS_N_GPIO_Port, DIAG_CS_N_Pin, GPIO_PIN_RESET);
  asm("nop");
  asm("nop");
  HAL_SPI_Receive(_hspi, buffer, size, 100);
  for(int i = 0; i < size; i++)
    buffer[i] = reverse_bits(buffer[i]);
  asm("nop");
  asm("nop");
  HAL_GPIO_WritePin(DIAG_CS_N_GPIO_Port, DIAG_CS_N_Pin, GPIO_PIN_SET);
}

/************************ (C) COPYRIGHT KonvolucioBt ***********END OF FILE****/

