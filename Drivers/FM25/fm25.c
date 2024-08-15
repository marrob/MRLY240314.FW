/*
 * FM25.c
 *
 *  Created on: Jun 13, 2024
 *      Author: marrob
 */

/* Includes ------------------------------------------------------------------*/
#include "fm25.h"


/* Private define ------------------------------------------------------------*/
#define FM25_CMD_READ       0x03
#define FM25_CMD_WREN       0x06  //Set write enable latch
#define FM25_CMD_WRDI       0x04  //Write disable
#define FM25_CMD_BP         0x02  //Byte Program
#define FM25_CMD_RDSR       0x05  //Read Status Register
#define FM25_CMD_WRSR       0x01  //Write Status Register
#define FM25_SPI_TIMEOUT_MS 100

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef *_spi;
/* Private function prototypes -----------------------------------------------*/
static inline void ChipSelectHigh(void);
static inline void ChipSelectLow(void);
static inline void WriteDisable(void);
static inline void WriteEnable(void);
static inline uint8_t ReadStatus(void);
static inline void WriteStatus(uint8_t status);

/* Private user code ---------------------------------------------------------*/
void FM25_Init(SPI_HandleTypeDef *spi)
{
  _spi = spi;

  /*--- All Protection OFF ---*/
  WriteStatus(0x00);
  WriteDisable();
}

void FM25_Read(uint16_t address, void *pData, size_t size)
{
  uint8_t cmd[] =
  {
     FM25_CMD_READ,
     address >> 8,
     address
  };

  ChipSelectLow();
  HAL_SPI_Transmit(_spi, cmd, sizeof(cmd), FM25_SPI_TIMEOUT_MS);
  HAL_SPI_Receive(_spi, pData, size, FM25_SPI_TIMEOUT_MS);
  ChipSelectHigh();
}


void FM25_Write(uint16_t address, void *pData, size_t size)
{
  uint8_t cmd[] =
  {
     FM25_CMD_BP,
     address >> 8,
     address,
  };

  WriteEnable();

  ChipSelectLow();
  HAL_SPI_Transmit(_spi, cmd, sizeof(cmd), FM25_SPI_TIMEOUT_MS);
  HAL_SPI_Transmit(_spi, pData, size, FM25_SPI_TIMEOUT_MS);
  ChipSelectHigh();
}


static inline uint8_t ReadStatus(void)
{
  uint8_t cmd[] =
  {
     FM25_CMD_RDSR,
  };
  uint8_t status;

  ChipSelectLow();
  HAL_SPI_Transmit(_spi, cmd, sizeof(cmd), FM25_SPI_TIMEOUT_MS);
  HAL_SPI_Receive(_spi, &status, 1, FM25_SPI_TIMEOUT_MS);
  ChipSelectHigh();

  return status;
}

static inline void WriteStatus(uint8_t status)
{
  uint8_t cmd[] =
  {
      FM25_CMD_WREN,
      status
  };

  WriteEnable();

  ChipSelectLow();
  HAL_SPI_Transmit(_spi, cmd, sizeof(cmd), FM25_SPI_TIMEOUT_MS);
  ChipSelectHigh();
}


static inline void WriteEnable(void)
{
  uint8_t cmd[] =
  {
      FM25_CMD_WREN
  };
  ChipSelectLow();
  HAL_SPI_Transmit(_spi, cmd, sizeof(cmd), FM25_SPI_TIMEOUT_MS);
  ChipSelectHigh();
}


static inline void WriteDisable(void)
{
  uint8_t cmd[] =
  {
      FM25_CMD_WRDI
  };
  ChipSelectLow();
  HAL_SPI_Transmit(_spi, cmd, sizeof(cmd), FM25_SPI_TIMEOUT_MS);
  ChipSelectHigh();
}


static inline void ChipSelectHigh(void)
{
  HAL_GPIO_WritePin(FM25_CS_N_GPIO_Port, FM25_CS_N_Pin, GPIO_PIN_SET);
}

static inline void ChipSelectLow(void)
{
  HAL_GPIO_WritePin(FM25_CS_N_GPIO_Port, FM25_CS_N_Pin, GPIO_PIN_RESET);
}

/************************ (C) COPYRIGHT KonvolucioBt ***********END OF FILE****/
