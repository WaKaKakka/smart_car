#include "encoder.h"

#include "stm32f1xx_hal.h" 
#include <stdint.h>

/**
  * @brief  读取TIM2编码器计数值（带方向转换与清零）
  * @retval int16_t 有符号计数值 (-32768 ~ 32767)
  */
int16_t Read_Encoder_TIM2(void)
{
    uint16_t raw_cnt;
    
    /* 临界区保护：防止读取与清零之间被中断打断导致少计/多计 */
    __disable_irq();
    raw_cnt = htim2.Instance->CNT;
    htim2.Instance->CNT = 0;
    __enable_irq();

    /* 利用C语言隐式类型转换，自动将 0x0000~0xFFFF 映射为 -32768~32767 */
    return (int16_t)raw_cnt;
}