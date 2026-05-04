#ifndef __DELAY_H
#define __DELAY_H

/* 根据实际使用的芯片系列替换头文件，例如：stm32f4xx_hal.h、stm32h7xx_hal.h 等 */
#include "stm32f1xx_hal.h"  
#include <stdint.h>

void delay_init(void);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);

#endif /* __DELAY_H */