#ifndef __PID_H__
#define __PID_H__

#include <stdint.h>

typedef struct {
    float Kp;           // 比例系数
    float Ki;           // 积分系数
    float Kd;           // 微分系数

    float target;       // 目标值
    float error;        // 当前误差
    float last_error;   // 上次误差
    float prev_error;   // 上上次误差

    float output;       // 输出值
    float max_output;   // 输出限幅
    float max_integral; // 积分限幅

    float integral;     // 积分累积（位置式PID用）
} PID_t;

/**
 * @brief 初始化PID控制器
 * @param pid PID结构体指针
 * @param Kp 比例系数
 * @param Ki 积分系数
 * @param Kd 微分系数
 * @param max_output 输出限幅
 * @param max_integral 积分限幅
 */
void PID_Init(PID_t *pid, float Kp, float Ki, float Kd, float max_output, float max_integral);

/**
 * @brief 设置PID目标值
 * @param pid PID结构体指针
 * @param target 目标值
 */
void PID_SetTarget(PID_t *pid, float target);

/**
 * @brief 位置式PID计算
 * @param pid PID结构体指针
 * @param current 当前值
 * @return PID输出值
 */
float PID_PositionCalc(PID_t *pid, float current);

/**
 * @brief 增量式PID计算
 * @param pid PID结构体指针
 * @param current 当前值
 * @return PID输出值（累积后，可直接用于控制）
 */
float PID_IncrementalCalc(PID_t *pid, float current);

/**
 * @brief 重置PID控制器（清空历史误差和积分）
 * @param pid PID结构体指针
 */
void PID_Reset(PID_t *pid);

#endif // __PID_H__
