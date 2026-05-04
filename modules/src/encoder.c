#include "encoder.h"

#include "stm32f1xx_hal.h"
#include "tim.h"
#include <stdint.h>

/**
  * @brief  读取TIM2编码器计数值（左轮，带方向转换与清零）
  * @retval int16_t 有符号计数值 (-32768 ~ 32767)
  */
int16_t Read_Encoder_TIM2(void)
{
    uint16_t raw_cnt;

    __disable_irq();
    raw_cnt = htim2.Instance->CNT;
    htim2.Instance->CNT = 0;
    __enable_irq();

    return (int16_t)raw_cnt;
}

/**
  * @brief  读取TIM8编码器计数值（右轮，带方向转换与清零）
  * @retval int16_t 有符号计数值 (-32768 ~ 32767)
  */
int16_t Read_Encoder_TIM8(void)
{
    uint16_t raw_cnt;

    __disable_irq();
    raw_cnt = htim8.Instance->CNT;
    htim8.Instance->CNT = 0;
    __enable_irq();

    return (int16_t)raw_cnt;
}