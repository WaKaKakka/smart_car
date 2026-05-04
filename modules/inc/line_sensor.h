#ifndef LINE_SENSOR_H
#define LINE_SENSOR_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include "main.h"
#include "delay.h"

//=====================================================================================
//  引脚配置接口 (Pin Configuration)
//  地址线: PF13(AD2), PF14(AD1), PF15(AD0)
//  数据输出: PG0
//=====================================================================================
#define SENSOR_AD0_PORT         GPIOF
#define SENSOR_AD0_PIN          GPIO_PIN_15

#define SENSOR_AD1_PORT         GPIOF
#define SENSOR_AD1_PIN          GPIO_PIN_14

#define SENSOR_AD2_PORT         GPIOF
#define SENSOR_AD2_PIN          GPIO_PIN_13

#define GrayS_OUT_PORT          GPIOG
#define GrayS_OUT_PIN           GPIO_PIN_0

//=====================================================================================
//  GPIO操作抽象接口 (HAL Macro Wrappers)
//=====================================================================================
#define SENSOR_AD0_WRITE(state)  HAL_GPIO_WritePin(SENSOR_AD0_PORT, SENSOR_AD0_PIN, \
                                                   (state) ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define SENSOR_AD1_WRITE(state)  HAL_GPIO_WritePin(SENSOR_AD1_PORT, SENSOR_AD1_PIN, \
                                                   (state) ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define SENSOR_AD2_WRITE(state)  HAL_GPIO_WritePin(SENSOR_AD2_PORT, SENSOR_AD2_PIN, \
                                                   (state) ? GPIO_PIN_SET : GPIO_PIN_RESET)

#define SENSOR_OUT_READ()        ((uint16_t)HAL_GPIO_ReadPin(GrayS_OUT_PORT, GrayS_OUT_PIN))

//=====================================================================================
//  驱动函数接口 (Driver API)
//=====================================================================================
#define GRAYSCALE_SENSOR_CHANNELS   8   // 传感器通道总数

void Grayscale_Sensor_Init(void);
void Grayscale_Sensor_Read_All(uint16_t* sensor_values);
uint16_t Grayscale_Sensor_Read_Single(uint8_t channel);

#ifdef __cplusplus
}
#endif

#endif

