#ifndef __ENCODER_H__
#define __ENCODER_H__

#include <stdint.h>

// 左轮编码器 (TIM2, PA0/PA1)
int16_t Read_Encoder_TIM2(void);

// 右轮编码器 (TIM8, PC6/PC7)
int16_t Read_Encoder_TIM8(void);

#endif