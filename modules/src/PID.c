#include "PID.h"

void PID_Init(PID_t *pid, float Kp, float Ki, float Kd, float max_output, float max_integral)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;

    pid->target = 0;
    pid->error = 0;
    pid->last_error = 0;
    pid->prev_error = 0;

    pid->output = 0;
    pid->max_output = max_output;
    pid->max_integral = max_integral;

    pid->integral = 0;
}

void PID_SetTarget(PID_t *pid, float target)
{
    pid->target = target;
}

float PID_PositionCalc(PID_t *pid, float current)
{
    pid->error = pid->target - current;

    // 积分累加
    pid->integral += pid->error;

    // 积分限幅（Anti-Windup）
    if (pid->integral > pid->max_integral) {
        pid->integral = pid->max_integral;
    } else if (pid->integral < -pid->max_integral) {
        pid->integral = -pid->max_integral;
    }

    // 位置式PID公式: output = Kp*e + Ki*∫e + Kd*de
    pid->output = pid->Kp * pid->error
                + pid->Ki * pid->integral
                + pid->Kd * (pid->error - pid->last_error);

    // 输出限幅
    if (pid->output > pid->max_output) {
        pid->output = pid->max_output;
    } else if (pid->output < -pid->max_output) {
        pid->output = -pid->max_output;
    }

    pid->last_error = pid->error;

    return pid->output;
}

float PID_IncrementalCalc(PID_t *pid, float current)
{
    pid->error = pid->target - current;

    // 增量式PID公式: Δoutput = Kp*(e-e1) + Ki*e + Kd*(e-2*e1+e2)
    float increment = pid->Kp * (pid->error - pid->last_error)
                    + pid->Ki * pid->error
                    + pid->Kd * (pid->error - 2 * pid->last_error + pid->prev_error);

    pid->output += increment;

    // 输出限幅
    if (pid->output > pid->max_output) {
        pid->output = pid->max_output;
    } else if (pid->output < -pid->max_output) {
        pid->output = -pid->max_output;
    }

    // 更新误差历史
    pid->prev_error = pid->last_error;
    pid->last_error = pid->error;

    return pid->output;
}

void PID_Reset(PID_t *pid)
{
    pid->error = 0;
    pid->last_error = 0;
    pid->prev_error = 0;
    pid->integral = 0;
    pid->output = 0;
}
