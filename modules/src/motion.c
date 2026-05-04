#include "motion.h"
#include "PID.h"
#include "PWM.h"
#include "encoder.h"

// 左轮PID（motor1），右轮PID（motor2）
static PID_t pid_left;
static PID_t pid_right;

static motor_data_t motor_data;
static uint8_t g_start_ctrl = 0;

// PID参数
#define MOTOR_KP  (1.5f)
#define MOTOR_KI  (0.08f)
#define MOTOR_KD  (0.5f)
#define MOTOR_MAX_OUTPUT  (800.0f)
#define MOTOR_MAX_INTEGRAL (300.0f)

void Motion_Init(void)
{
    PID_Init(&pid_left,  MOTOR_KP, MOTOR_KI, MOTOR_KD, MOTOR_MAX_OUTPUT, MOTOR_MAX_INTEGRAL);
    PID_Init(&pid_right, MOTOR_KP, MOTOR_KI, MOTOR_KD, MOTOR_MAX_OUTPUT, MOTOR_MAX_INTEGRAL);

    motor_data.speed_set[0] = 0;
    motor_data.speed_set[1] = 0;
    motor_data.speed_pwm[0] = 0;
    motor_data.speed_pwm[1] = 0;
    motor_data.speed_mm_s[0] = 0;
    motor_data.speed_mm_s[1] = 0;
}

void Motion_Set_Speed(int16_t left, int16_t right)
{
    g_start_ctrl = 1;
    motor_data.speed_set[0] = left;
    motor_data.speed_set[1] = right;
    PID_SetTarget(&pid_left,  (float)left);
    PID_SetTarget(&pid_right, (float)right);
}

void Motion_Set_Pwm(int16_t left, int16_t right)
{
    TB6612_SetSpeed(1, left);
    TB6612_SetSpeed(2, right);
}

// 读取编码器并计算速度（mm/s）
static void Motion_Get_Speed(void)
{
    // 读取编码器脉冲数（10ms内的脉冲）
    int16_t pulse_left  = Read_Encoder_TIM2();   // 左轮 TIM2 (PA0/PA1)
    int16_t pulse_right = Read_Encoder_TIM8();   // 右轮 TIM8 (PC6/PC7)

    // 脉冲转换为速度: speed(mm/s) = pulse * (1000/10ms) * (周长/每圈脉冲)
    // 即 speed = pulse * 100 * WHEEL_CIRCUMFERENCE / ENCODER_CIRCLE_PULSES
    motor_data.speed_mm_s[0] = (float)pulse_left  * 100.0f * WHEEL_CIRCUMFERENCE_MM / ENCODER_CIRCLE_PULSES;
    motor_data.speed_mm_s[1] = (float)pulse_right * 100.0f * WHEEL_CIRCUMFERENCE_MM / ENCODER_CIRCLE_PULSES;
}

void Motion_Handle(void)
{
    Motion_Get_Speed();

    if (g_start_ctrl)
    {
        // PID计算
        motor_data.speed_pwm[0] = PID_IncrementalCalc(&pid_left,  motor_data.speed_mm_s[0]);
        motor_data.speed_pwm[1] = PID_IncrementalCalc(&pid_right, motor_data.speed_mm_s[1]);

        // 设置PWM
        Motion_Set_Pwm((int16_t)motor_data.speed_pwm[0], (int16_t)motor_data.speed_pwm[1]);
    }
}
