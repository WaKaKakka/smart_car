#include "PWM.h"
#include "main.h"
#include "tim.h"

void TB6612_SetSpeed(uint8_t motor, int16_t speed)
{
    if (motor == 1)
    {
        if (speed >= 0)
        {
            HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint16_t)speed);
        }
        else
        {
            HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint16_t)(-speed));
        }
    }
    else if (motor == 2)
    {
        if (speed >= 0)
        {
            HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint16_t)speed);
        }
        else
        {
            HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint16_t)(-speed));
        }
    }
}

void TB6612_Stop(void)
{
    HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
}