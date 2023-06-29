#ifndef _IST8310DRIVER_H_
#define _IST8310DRIVER_H_

#include "main.h"

#define MAG_RATE 0.3
typedef struct
{
    float x;
    float y;
    float z;
} IST8310_Magnetic_Data_t;
extern IST8310_Magnetic_Data_t magData;

HAL_StatusTypeDef IST8310_Init(void);
void get_ist8310_data(IST8310_Magnetic_Data_t *mag_data);

// ist8310 register address
#define IST8310_IIC_ADDRESS 0X0E

#define IST8310_ID_ADDR 0x00
#define IST8310_ID_VAL 0x10
// Status
#define IST8310_STATUS1_ADDR 0x02
#define IST8310_STATUS2_ADDR 0x09
// Data
#define IST8310_DATA_XL_ADDR 0x03
#define IST8310_DATA_XH_ADDR 0x04
#define IST8310_DATA_YL_ADDR 0x05
#define IST8310_DATA_YH_ADDR 0x06
#define IST8310_DATA_ZL_ADDR 0x07
#define IST8310_DATA_ZH_ADDR 0x08
#define IST8310_TEMPL_ADDR 0x1C
#define IST8310_TEMPH_ADDR 0x1D
// Control
#define IST8310_CNTL1_ADDR 0x0A     // 测量模式
#define IST8310_CNTL1_SLEEP 0x00    // 休眠
#define IST8310_CNTL1_SINGLE 0x01   // 单次测量
#define IST8310_CNTL1_CONTINUE 0x0B // 连续测量，输出频率200HZ

#define IST8310_CNTL2_ADDR 0x0B // 中断设置寄存器
#define IST8310_CNT2_NO_IT 0x00 // 不启用中断

#define IST8310_SELF_CHECK_ADDR 0x0C // 自检寄存器

#define IST8310_AVGCNTL_ADDR 0x41 // 平均采样寄存器
#define IST8310_AVGCNTL_TWICE 0x09
#define IST8310_AVGCNTL_FOURTH 0x12

#endif
