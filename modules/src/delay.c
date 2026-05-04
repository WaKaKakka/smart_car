#include "delay.h"

/* 
 * 注意：原代码使用 uint8_t/uint16_t 存储延时系数，在 72MHz 以上主频时极易溢出。
 * 已改为 uint32_t 保证通用性和安全性。
 */
static uint32_t fac_us = 0;  // us延时系数 (每微秒的时钟周期数)
static uint32_t fac_ms = 0;  // ms延时系数 (每毫秒的时钟周期数)

void delay_init(void)
{
    /* 
     * HAL库工程中通常直接使用系统主频(HCLK)。
     * 原SPL代码中的 HCLK/8 属于旧版习惯，现代HAL/CMSIS推荐直接使用 HCLK 计算，
     * 精度更高且无需操作控制寄存器。
     */
    fac_us = SystemCoreClock / 1000000;
    fac_ms = fac_us * 1000;
}

/**
  * @brief  精确阻塞延时 n 毫秒
  * @param  nms: 延时时间，单位 ms
  */
void delay_ms(uint16_t nms)
{
    uint32_t temp;
    
    SysTick->LOAD  = (uint32_t)nms * fac_ms; // 装载重载值
    SysTick->VAL   = 0x00;                  // 清空当前值
    SysTick->CTRL  = 0x01;                  // 使能定时器，关闭中断
    
    /* 等待计数到0，COUNTFLAG位置1表示计时结束 */
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));
    
    SysTick->CTRL  = 0x00;                  // 关闭定时器
    SysTick->VAL   = 0x00;                  // 清空当前值
}

/**
  * @brief  精确阻塞延时 n 微秒
  * @param  nus: 延时时间，单位 us
  */
void delay_us(uint32_t nus)
{
    uint32_t temp;
    
    SysTick->LOAD  = nus * fac_us;
    SysTick->VAL   = 0x00;
    SysTick->CTRL  = 0x01;
    
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));
    
    SysTick->CTRL  = 0x00;
    SysTick->VAL   = 0x00;
}