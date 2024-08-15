/*
 * fm25_uint_test.c
 *
 *  Created on: Jun 13, 2024
 *      Author: marrob
 */

/* Includes ------------------------------------------------------------------*/
#include "fm25.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/

void FM25_UnitTest(void)
{
  /*--- Flash ---*/
  uint8_t read_buffer[32] = { 0 };
  FM25_Read(0x0000, read_buffer, sizeof(read_buffer));

  /*--- Erase with 0xCC ---*/
  uint8_t erase_buffer[256];
  memset(erase_buffer, 0xCC, sizeof(erase_buffer));
  FM25_Write(0x0000, erase_buffer, sizeof(erase_buffer));

  /*--- Empty Check ---*/
  char empty[] = { 0xCC, 0xCC ,0xCC,  0xCC, 0xCC ,0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
  FM25_Read(0x000, read_buffer, sizeof(read_buffer));
  if(memcmp(read_buffer, empty, sizeof(empty)) == 0)
    printf("Successful, FM25 is empty.\n");
  else
    printf("Error, FM25 is not empty.\n");


  /*--- Write Data ---*/
  char sample_data[] = "Hello, World!";
  for(int i = 0; i < strlen(sample_data);i++)
  {
    FM25_Write(i, (uint8_t*)sample_data + i , 1);
  }

  /*--- Data Read ---*/
  FM25_Read(0x0000, read_buffer, sizeof(read_buffer));

  /*--- Data Check ---*/
  if(memcmp(read_buffer, sample_data, strlen(sample_data)) == 0)
    printf("Successful, Content of FM25 is correct.\n");
  else
    printf("Error, Content of FM25 is not correct.\n");



  /*--- uint32_t read/write ---*/

  uint32_t excepted_value = 0x55AA;
  FM25_Write(0, &excepted_value, sizeof(uint32_t));

  uint32_t read_value = 0;
  FM25_Read(0, &read_value, sizeof(uint32_t));

  if(excepted_value == read_value)
    printf("Successful, value check ok.\n");
  else
    printf("Error, value check failed.\n");


  HAL_Delay(100);

  while(1)
  {
  }
}


/************************ (C) COPYRIGHT KonvolucioBt ***********END OF FILE****/
