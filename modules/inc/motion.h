#ifndef __MOTION_H
#define __MOTION_H

#include <stdint.h>

// 编码器参数：每转一圈1040个脉冲（260线×4倍频）
#define ENCODER_CIRCLE_PULSES   (1040.0f)
// 轮子周长（mm）
#define WHEEL_CIRCUMFERENCE_MM  (204.203f)

typedef struct {
    float speed_mm_s[2];    // 当前速度（mm/s），[0]=左轮, [1]=右轮
    float speed_pwm[2];     // PID输出PWM值
    int16_t speed_set[2];   // 目标速度（mm/s）
} motor_data_t;

/**
 * @brief 初始化运动控制（PID参数等）
 */
void Motion_Init(void);

/**
 * @brief 设置左右轮目标速度
 * @param left  左轮速度（mm/s），范围约[-1000, 1000]
 * @param right 右轮速度（mm/s），范围约[-1000, 1000]
 */
void Motion_Set_Speed(int16_t left, int16_t right);

/**
 * @brief 运动控制主函数，需每10ms调用一次
 *        读取编码器→计算速度→PID→设置PWM
 */
void Motion_Handle(void);

/**
 * @brief 直接设置PWM值（无闭环控制）
 * @param left  左轮PWM [-1000, 1000]
 * @param right 右轮PWM [-1000, 1000]
 */
void Motion_Set_Pwm(int16_t left, int16_t right);

#endif
