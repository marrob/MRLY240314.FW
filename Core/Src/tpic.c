/*
 * tpic.h
 *
 *  Created on: Apr 13, 2024
 *      Author: Margit Robert
 */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <string.h>

static SPI_HandleTypeDef *_hspi;

/*
 *
 * MINDEN TPICs_ művelet előtt az TPICs_FpgaBypassOn
 * majd miután végzett a mikró visszadni a vezérlést az FPGA-nak
 * a TPICs_FpgaBypassOff()-al.
 *
 */

static void ArrayToolsU8ClrBit(const uint16_t index, void * array);
static void ArrayToolsU8SetBit(const uint16_t index, void * array);
static void Update(void);


uint8_t _tpic_array [TPIC_COUNT] = {0};

void TPICs_Init(SPI_HandleTypeDef *hspi)
{
  _hspi = hspi;

/*
  TPICs_FpgaBypassOn();
 // _tpic_array[TPIC_COUNT - 1] = 0xFF; //demo panaelen az elso bit, mivel a bit sorrend jó, de küldést a kisebb cimeü memória területtel kezdi.
  _tpic_array[53] = 0x03;
  Update();
  TPICs_FpgaBypassOff();
*/
}

bool TPICs_ChainCheckIsPassed(void)
{

  uint8_t testvector[TPIC_COUNT *2];
  uint8_t result[TPIC_COUNT * 2];
  memset(testvector, 0x00, sizeof(testvector));
  memset(testvector, 0x55, sizeof(testvector)/2);
  memset(result, 0x00, sizeof(testvector));

  HAL_GPIO_WritePin(DAIG_BYPS_GPIO_Port, DAIG_BYPS_Pin, GPIO_PIN_SET);

  //do{
  /*
   * A szkop
   * 200ns/div
   * 2V/div-es állásban kell hogy legyen
   */
    HAL_SPI_TransmitReceive(_hspi, testvector, result, TPIC_COUNT * 2, 100);
    HAL_Delay(100);
  //}while(1);

  if(memcmp(testvector, result + TPIC_COUNT, TPIC_COUNT) == 0)
  {
    printf("tpic.c Loop test PASSED\r\n");
    return true;
  }
  else
  {
    printf("tpic.c Loop test FAILED\r\n");
    return false;
  }
}

//0..431
void TPICs_Set(uint16_t index)
{
  ArrayToolsU8SetBit(index, _tpic_array);
  Update();
}

void TPICs_Clr(uint16_t index)
{
  ArrayToolsU8ClrBit(index, _tpic_array);
  Update();
}

/*
 * 0..431
 *                                 //first sent byte ... last stent byte
 * 0.   -> TPIC 1.  DRAIN0, array[54] = { 0x00, 0x00, 0x00... 0x01}
 * 431. -> TPIC 54. DRAIN7, array[54] = { 0x80, 0x00, 0x00... 0x00}
 */
void ArrayToolsU8SetBit(const uint16_t index, void * array)
{
  uint8_t *ptr = array;
  uint8_t  mask;

  //bájt sorrend forditasa
  uint8_t byteIndex = (TPIC_COUNT - 1) - (index / 8) ;
  uint8_t bitIndex =  index % 8;

  if(index % 8)
  {
     mask = 1 << bitIndex;
  }
  else
  {
     mask = 0x01;
  }
  ptr[byteIndex]|= mask;
}


void ArrayToolsU8ClrBit(const uint16_t index, void * array)
{
  uint8_t *ptr = array;
  uint8_t  mask;

  //bájt sorrend forditasa
  uint8_t byteIndex = (TPIC_COUNT - 1) - (index / 8) ;
  uint8_t bitIndex = index - (byteIndex*8);

  if(index % 8)
  {
     mask = 0xFE << bitIndex;
  }
  else
  {
     mask = 0xFE;
  }

  ptr[byteIndex]&= mask;
}

void TPICs_FpgaBypassOn(void)
{
  //FPGA SPI bypass ON
  HAL_GPIO_WritePin(DAIG_BYPS_GPIO_Port, DAIG_BYPS_Pin, GPIO_PIN_SET);

  //TPIC kimenetek enegedélyezése
  HAL_GPIO_WritePin(DIAG_G_N_GPIO_Port, DIAG_G_N_Pin, GPIO_PIN_RESET);
}

void TPICs_FpgaBypassOff(void)
{
  //FPGA SPI bypass OFF
  HAL_GPIO_WritePin(DAIG_BYPS_GPIO_Port, DAIG_BYPS_Pin, GPIO_PIN_RESET);

  //TPIC kimenetek tiltása
  HAL_GPIO_WritePin(DIAG_G_N_GPIO_Port, DIAG_G_N_Pin, GPIO_PIN_SET);
}


void TPICs_TestPattern_1(void)
{
  for(uint8_t i = 0;  i < TPIC_COUNT; i++)
      _tpic_array[i] = 0x55;
  Update();
}

void TPICs_ChainWrite(uint8_t *buffer, size_t size)
{
  memcpy(_tpic_array , buffer, size);
  Update();
}

void Update(void)
{
  HAL_SPI_Transmit(_hspi, _tpic_array, TPIC_COUNT, 100);
  HAL_GPIO_WritePin(DIAG_RCK_GPIO_Port, DIAG_RCK_Pin, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(DIAG_RCK_GPIO_Port, DIAG_RCK_Pin, GPIO_PIN_RESET);
}
/************************ (C) COPYRIGHT KonvolucioBt ***********END OF FILE****/


