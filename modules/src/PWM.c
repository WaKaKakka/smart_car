#include "PWM.h"
#include "main.h"
#include "tim.h"

void TB6612_SetSpeed(uint8_t motor, int16_t speed)
{
    if (motor == 1)
    {
        if (speed >= 0)
        {
            // 正转: AIN1=HIGH, AIN2=LOW
            HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint16_t)speed);
        }
        else
        {
            // 反转: AIN1=LOW, AIN2=HIGH
            HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint16_t)(-speed));
        }
    }
}

void TB6612_Stop(void)
{
    HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);
}