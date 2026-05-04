#include "trace_task.h"
#include "motion.h"
#include <math.h>

line_following_t g_line_controller;

// 传感器数据缓冲区
static uint16_t g_sensor_data[GRAYSCALE_SENSOR_CHANNELS];

static inline float clamp_float(float value, float min_val, float max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

static inline int16_t clamp_int16(int16_t value, int16_t min_val, int16_t max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

// 安全锁检查：传感器全亮或全灭时不启动
static bool check_sensors_safe(line_following_t* controller, uint16_t* sensor_values) {
    uint16_t first_value = sensor_values[0];
    for (int i = 1; i < GRAYSCALE_SENSOR_CHANNELS; i++) {
        if (sensor_values[i] != first_value) {
            return true;  // 传感器值不全相同，安全
        }
    }
    return false;  // 全亮或全灭，不安全
}

// 计算加权偏差
static float calculate_error(line_following_t* controller, uint16_t* sensor_values) {
    float weighted_sum = 0.0f;
    int active_sensors = 0;

    for (int i = 0; i < GRAYSCALE_SENSOR_CHANNELS; i++) {
        if (sensor_values[i] == 0) {  // 检测到黑线（低电平）
            weighted_sum += controller->sensor_weights[i];
            active_sensors++;
        }
    }

    if (active_sensors == 0) {
        return controller->last_error;  // 丢线，返回上次偏差
    }

    return weighted_sum / active_sensors;
}

// 巡线PID控制
static float pid_control(line_following_t* controller, float error) {
    // 死区
    if (fabsf(error) < 0.6f) {
        error = 0.0f;
    }

    // 过零清积分
    if ((controller->last_error > 0 && error < 0) ||
        (controller->last_error < 0 && error > 0)) {
        controller->integral = 0.0f;
    }

    // 动态积分限幅
    float integral_limit;
    if (fabsf(error) > 3.0f) {
        integral_limit = 80.0f;
    } else if (fabsf(error) > 1.5f) {
        integral_limit = 50.0f;
    } else {
        integral_limit = 20.0f;
    }

    controller->integral += error;
    controller->integral = clamp_float(controller->integral, -integral_limit, integral_limit);

    float derivative = error - controller->last_error;

    float output = (controller->kp * error +
                    controller->ki * controller->integral +
                    controller->kd * derivative);

    controller->last_error = error;
    return output;
}

void line_following_init(line_following_t* controller)
{
    controller->kp = 60.0f;   // 巡线PID参数，需根据实际调试
    controller->ki = 0.0f;
    controller->kd = 20.0f;

    controller->last_error = 0.0f;
    controller->integral = 0.0f;

    controller->base_speed = 200;   // 基础速度 mm/s
    controller->max_speed = 400;    // 最大速度 mm/s

    // 传感器权重：左负右正
    controller->sensor_weights[0] = -4.0f;
    controller->sensor_weights[1] = -3.0f;
    controller->sensor_weights[2] = -2.0f;
    controller->sensor_weights[3] = -1.0f;
    controller->sensor_weights[4] =  1.0f;
    controller->sensor_weights[5] =  2.0f;
    controller->sensor_weights[6] =  3.0f;
    controller->sensor_weights[7] =  4.0f;

    controller->motor_locked = true;  // 初始锁定
}

void follow_line(line_following_t* controller)
{
    // 读取灰度传感器
    Grayscale_Sensor_Read_All(g_sensor_data);

    // 安全锁检查
    if (controller->motor_locked) {
        if (check_sensors_safe(controller, g_sensor_data)) {
            controller->motor_locked = false;
        } else {
            Motion_Set_Speed(0, 0);
            return;
        }
    }

    // 计算偏差
    float error = calculate_error(controller, g_sensor_data);

    // PID计算
    float pid_output = pid_control(controller, error);

    // 差速控制
    int16_t left_speed  = controller->base_speed + (int16_t)pid_output;
    int16_t right_speed = controller->base_speed - (int16_t)pid_output;

    left_speed  = clamp_int16(left_speed,  -controller->max_speed, controller->max_speed);
    right_speed = clamp_int16(right_speed, -controller->max_speed, controller->max_speed);

    // 设置电机速度（速度闭环）
    Motion_Set_Speed(left_speed, right_speed);
}
