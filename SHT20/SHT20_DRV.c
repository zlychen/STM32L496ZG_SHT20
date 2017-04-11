#include "stm32l4xx_hal.h"
#include "i2c.h"
#include "string.h"
#include "math.h"
#include "SHT20_DRV.h"

#define SHT20_ADDRESS  0X40
#define SHT20_Measurement_RH_HM  0XE5
#define SHT20_Measurement_T_HM  0XE3
#define SHT20_Measurement_RH_NHM  0XF5
#define SHT20_Measurement_T_NHM  0XF3
#define SHT20_READ_REG  0XE7
#define SHT20_WRITE_REG  0XE6
#define SHT20_SOFT_RESET  0XFE


const int16_t POLYNOMIAL = 0x131;
uint8_t SHT20_RxBuff[3];

float Get_SHT20_TEMP(void)
{
	unsigned short tmp = 0;
	float Temperature = 0;
	I2C_Write_Data(&hi2c2,SHT20_Measurement_T_HM);
	I2C_Read_Data(&hi2c2,SHT20_RxBuff,3);
	SHT2x_CheckCrc((char*)SHT20_RxBuff, 2,SHT20_RxBuff[2]);
	tmp = (SHT20_RxBuff[0] << 8) + SHT20_RxBuff[1];
	Temperature = SHT2x_CalcTemperatureC(tmp);
	memset(SHT20_RxBuff,0,3);
	return Temperature;
}
float Get_SHT20_HUM(void)
{
	unsigned short tmp = 0;
	float Humidity = 0;
	I2C_Write_Data(&hi2c2,SHT20_Measurement_RH_HM);
	I2C_Read_Data(&hi2c2,SHT20_RxBuff,3);
	SHT2x_CheckCrc((char*)SHT20_RxBuff, 2, SHT20_RxBuff[2]);
	tmp = (SHT20_RxBuff[0] << 8) + SHT20_RxBuff[1];
	Humidity = SHT2x_CalcRH(tmp);
	memset(SHT20_RxBuff,0,3);
	return Humidity;
}

void I2C_Write_Data(I2C_HandleTypeDef *hi2c,uint8_t data)
{
	HAL_I2C_Master_Transmit(hi2c,SHT20_ADDRESS<<1,&data,1,0xffff);
}

void I2C_Read_Data(I2C_HandleTypeDef *hi2c,uint8_t *Buf,uint8_t len)
{  
	HAL_I2C_Master_Receive(hi2c,SHT20_ADDRESS<<1,Buf,len,0xffff);
}

char SHT2x_CheckCrc(char data[], char nbrOfBytes, char checksum)
{	
    char crc = 0;
    char bit = 0;
    char byteCtr = 0;
    for(byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
    {
        crc ^= (data[byteCtr]);
        for ( bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
            else crc = (crc << 1);
        }
    }	
    if(crc != checksum)
		return 1;
    else
		return 0;	
}
float SHT2x_CalcTemperatureC(unsigned short u16sT)
{
    float temperatureC = 0;            
    u16sT &= ~0x0003;          
    temperatureC = -46.85 + 175.72 / 65536 * (float)u16sT; 
    return temperatureC;	
}
float SHT2x_CalcRH(unsigned short u16sRH)
{	
    float humidityRH = 0;            
    u16sRH &= ~0x0003;        
    humidityRH = ((float)u16sRH * 0.00190735) - 6;
    return humidityRH;	
}
