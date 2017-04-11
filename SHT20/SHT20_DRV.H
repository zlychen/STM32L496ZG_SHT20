

#ifndef _SHT20_DRV_H__
#define _SHT20_DRV_H__

#include "stm32l4xx_hal.h"


void I2C_Write_Data(I2C_HandleTypeDef *hi2c,uint8_t data);
void I2C_Read_Data(I2C_HandleTypeDef *hi2c,uint8_t *Buf,uint8_t len);
char SHT2x_CheckCrc(char data[], char nbrOfBytes, char checksum);
float SHT2x_CalcTemperatureC(unsigned short u16sT);
float SHT2x_CalcRH(unsigned short u16sRH);
float Get_SHT20_TEMP(void);
float Get_SHT20_HUM(void);


#endif
