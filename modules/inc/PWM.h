#ifndef __PWM_H
#define __PWM_H
#include <stdint.h>

/**
 * @brief 设置电机速度（支持正反转）
 * @param motor 电机编号（1 或 2）
 * @param speed 速度值，正数正转，负数反转，范围 -1000 ~ 1000
 */
void TB6612_SetSpeed(uint8_t motor, int16_t speed);

/**
 * @brief 停止所有电机
 */
void TB6612_Stop(void);

#endif