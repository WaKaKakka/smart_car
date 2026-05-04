#ifndef __TRACE_TASK_H
#define __TRACE_TASK_H

#include <stdint.h>
#include <stdbool.h>
#include "line_sensor.h"

typedef struct {
    // 巡线PID参数
    float kp;
    float ki;
    float kd;

    float last_error;
    float integral;

    // 控制参数
    int16_t base_speed;     // 基础速度（mm/s）
    int16_t max_speed;      // 最大速度（mm/s）

    // 传感器权重（8个通道，左负右正）
    float sensor_weights[8];

    // 安全锁：上电时锁定电机，防止乱跑
    bool motor_locked;

} line_following_t;

extern line_following_t g_line_controller;

/**
 * @brief 初始化巡线控制器
 */
void line_following_init(line_following_t* controller);

/**
 * @brief 巡线主函数，需周期性调用
 *        读取灰度传感器→计算偏差→PID→差速控制电机
 */
void follow_line(line_following_t* controller);

#endif
