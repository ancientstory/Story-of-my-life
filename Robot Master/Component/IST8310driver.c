#include "IST8310driver.h"
#include <stdint.h>
#include <i2c.h>

IST8310_Magnetic_Data_t magData;

static uint8_t read_single_register(uint8_t reg);
static void write_single_register(uint8_t reg, uint8_t data);
static void read_multiply_register(uint8_t reg, uint8_t *buf, uint8_t len);
static void write_multiply_register(uint8_t reg, uint8_t *data, uint8_t len);
/***************************** main function ******************************/
HAL_StatusTypeDef IST8310_Init(void)
{
    // restart ist8310
    HAL_GPIO_WritePin(IST8310_RST_GPIO_Port, IST8310_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(IST8310_RST_GPIO_Port, IST8310_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(50);

    if (IST8310_ID_VAL != read_single_register(IST8310_ID_ADDR))
    {
        return HAL_ERROR;
    }
    // basic setting
    // 采样四次
    write_single_register(IST8310_CNTL2_ADDR, IST8310_CNT2_NO_IT);
    write_single_register(IST8310_AVGCNTL_ADDR, IST8310_AVGCNTL_FOURTH);
    write_single_register(IST8310_CNTL1_ADDR, IST8310_CNTL1_CONTINUE);
		
		return HAL_OK;
}
void get_ist8310_data(IST8310_Magnetic_Data_t *mag_data)
{
    uint8_t buf[6];
    int16_t temp_mag_data = 0;
    read_multiply_register(IST8310_DATA_XL_ADDR, buf, 6);
    // convert data
    temp_mag_data = (int16_t)((buf[1] << 8) | buf[0]);
    mag_data->x = (float)MAG_RATE * temp_mag_data;
    temp_mag_data = (int16_t)((buf[3] << 8) | buf[2]);
    mag_data->y = (float)MAG_RATE * temp_mag_data;
    temp_mag_data = (int16_t)((buf[5] << 8) | buf[4]);
    mag_data->z = (float)MAG_RATE * temp_mag_data;
}
// Basic read/write function
static uint8_t read_single_register(uint8_t reg)
{
    uint8_t buf = 0;
    HAL_I2C_Mem_Read(&hi2c3, IST8310_IIC_ADDRESS << 1, reg, I2C_MEMADD_SIZE_8BIT, &buf, 1, 10);
    return buf;
}
static void write_single_register(uint8_t reg, uint8_t data)
{
    HAL_I2C_Mem_Write(&hi2c3, IST8310_IIC_ADDRESS << 1, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
}
static void read_multiply_register(uint8_t reg, uint8_t *buf, uint8_t len)
{
    HAL_I2C_Mem_Read(&hi2c3, IST8310_IIC_ADDRESS << 1, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 10);
}
static void write_multiply_register(uint8_t reg, uint8_t *data, uint8_t len)
{
    HAL_I2C_Mem_Write(&hi2c3, IST8310_IIC_ADDRESS << 1, reg, I2C_MEMADD_SIZE_8BIT, data, len, 10);
}
