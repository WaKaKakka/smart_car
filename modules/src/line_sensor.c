#include "line_sensor.h"


static void _delay_us(volatile uint32_t us)
{
    delay_us(us); // 依赖外部实现的微秒延时函数
}

//选择传感器通道 select sensor channel
static void _select_channel(uint8_t channel)
{
    SENSOR_AD0_WRITE((channel >> 0) & 0x01);  
    SENSOR_AD1_WRITE((channel >> 1) & 0x01);  
    SENSOR_AD2_WRITE((channel >> 2) & 0x01);  
}

//读取OUT引脚的值 read the value of OUT pin
static uint16_t Read_OUT_value(void)
{
    return SENSOR_OUT_READ();
}

//初始化灰度传感器所需的GPIO / init GPIO for grayscale sensor
void Grayscale_Sensor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 使能 GPIOF 和 GPIOG 时钟
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    // 配置 PF13/PF14/PF15 为推挽输出 (地址线 AD2/AD1/AD0)
    GPIO_InitStruct.Pin   = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    // 配置 PG0 为浮空输入 (数据输出)
    GPIO_InitStruct.Pin   = GPIO_PIN_0;
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

//读取所有8个通道的灰度值 read all 8 channels grayscale values
void Grayscale_Sensor_Read_All(uint16_t* sensor_values)
{
    uint8_t i;
    for (i = 0; i < GRAYSCALE_SENSOR_CHANNELS; i++)
    {
        _select_channel(i);
        _delay_us(50);
        sensor_values[i] = Read_OUT_value();
    }
}

//读取单个指定通道的灰度值 read single specified channel grayscale value
uint16_t Grayscale_Sensor_Read_Single(uint8_t channel)
{
    if (channel >= GRAYSCALE_SENSOR_CHANNELS)
    {
        return 0; // 无效通道 // Invalid channel
    }
    _select_channel(channel);
    _delay_us(50);
    return Read_OUT_value();
}