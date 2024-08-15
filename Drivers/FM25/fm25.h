/*
 * FM25.h
 *
 *  Created on: Jun 13, 2024
 *      Author: marrob
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef FM25_FM25_H_
#define FM25_FM25_H_
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void FM25_Init(SPI_HandleTypeDef *spi);
void FM25_Read(uint16_t address, void *pData, size_t size);
void FM25_Write(uint16_t address, void *pData, size_t size);
void FM25_UnitTest(void);
#endif /* FM25_FM25_H_ */

/************************ (C) COPYRIGHT KonvolucioBt ***********END OF FILE****/
