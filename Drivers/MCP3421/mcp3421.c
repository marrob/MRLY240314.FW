/******************** (C) COPYRIGHT 2011 marrob Design *************************
* File Name          : mcp3421.c
* Author             : Margit R�bert
* Date First Issued  : 26/10/2013
* Description        : 18-Bit Analog-to-Digital Converter
********************************************************************************/
#include <limits.h>
#include "mcp3421.h"


/* Private functions ---------------------------------------------------------*/
static uint8_t WriteConfig(uint8_t config);
static I2C_HandleTypeDef *_i2c;
static uint8_t _devAddress = 0;

uint8_t MCP3421_Init(I2C_HandleTypeDef *i2c, uint8_t address)
{ 
  _i2c = i2c;
  _devAddress = address;
  return(MCP3421_OK);
}

static uint8_t WriteConfig(uint8_t config)
{
  uint8_t data[] = { config };
  if(HAL_I2C_Master_Transmit(_i2c, _devAddress, data, sizeof(data), 100) == HAL_OK)
    return MCP3421_OK;
  else
    return MCP3421_FAIL;
}


int32_t MCP3421_GetValue_Blocking(uint8_t config)
{
  uint32_t poll_cnt = 0;
  uint8_t data[4]= { 0 };
  //S|Control|W|SlaveACK|BYTE|SlaveACK|BYTE|SlaveACK|....|ST|
  uint32_t result = 0;
  //Initate New Conversion
  //Beallitom 1-be RDY-t
  WriteConfig(MCP3421_MODE_ONESHOT | config);
  do
  {

    if(poll_cnt > 1024)
    { //ADC does not response
      result = 0;
      break;
    }
    //18-bit resolution
    //1SPS
    if((config & MCP3421_RES_MASK) == MCP3421_RES_18)
    {
      HAL_I2C_Master_Receive(_i2c, _devAddress, data, 4, 100);
      //Tx:  S|Address|
      //Rx:  UpperData|MidleData|LowerData|ConfigByte|NAK
      //Tx:  S
      if((data[3] & MCP3421_DRY) == 0x00)
      {
        //HAL_GPIO_TogglePin(ADC_SYNC_GPIO_Port, ADC_SYNC_Pin);

        result = ((data[0] & 0x03) <<16 ); //MSB
        result |= (data[1] << 8);
        result |= (data[2] << 0);

        //18.bit sign bit check
        if(result & 0x20000)
        {//Negative
          result |= 0xFFFE0000;
          result = ~result;
          result = -1 * result;
        }
        break;
      }
    }
    //16-bit resolution
    //1.6SPS
    else if((config & MCP3421_RES_MASK) == MCP3421_RES_16)
    {
      HAL_I2C_Master_Receive(_i2c, _devAddress, data, 3, 100);
      //Tx:  S|Address|
      //Rx:  UpperData|LowerData|ConfigByte|NAK
      //Tx:  S
      if((data[2] & 0x80) == 0x00)
      {
        //HAL_GPIO_TogglePin(ADC_SYNC_GPIO_Port, ADC_SYNC_Pin);

        result = (data[0]<<8); //MSB
        result |= (data[1]<<0);

        if(result & 0x8000)
        {
          result |= 0xFFFF8000;
          result = ~result;
          result = -1 * result;
        }
        break;
      }
    }
    //14-bit resolution
    //1.88SPS
    else if((config & MCP3421_RES_MASK) == MCP3421_RES_14)
    {
      HAL_I2C_Master_Receive(_i2c, _devAddress, data, 3, 100);
      //Tx:  S|Address|
      //Rx:  UpperData|LowerData|ConfigByte|NAK
      //Tx:  S
      if((data[2] & 0x80) == 0x00)
      {
        //HAL_GPIO_TogglePin(ADC_SYNC_GPIO_Port, ADC_SYNC_Pin);

        result = (data[0]<<8); //MSB
        result |= (data[1]<<0);

        if(result & 0x4000)
        {
          result |= 0xFFFFC000;
          result = ~result;
          result = -1 * result;
        }
        break;
      }
    }

    //12-bit resolution
    //2SPS
    else if((config & MCP3421_RES_MASK) == MCP3421_RES_12)
    {
      HAL_I2C_Master_Receive(_i2c, _devAddress, data, 3, 100);
      //Tx:  S|Address|
      //Rx:  UpperData|LowerData|ConfigByte|NAK
      //Tx:  S
      if((data[2] & 0x80) == 0x00)
      {
        //HAL_GPIO_TogglePin(ADC_SYNC_GPIO_Port, ADC_SYNC_Pin);

        result = (data[0]<<8); //MSB
        result |= (data[1]<<0);

        if(result & 0x1000)
        {
          result |= 0xFFFFF000;
          result = ~result;
          result = -1 * result;
        }
        break;
      }
    }


    poll_cnt++;

  }while(1);


  return(result);
}

/******************* (C) COPYRIGHT 2011 marrob Design *****END OF FILE******/


