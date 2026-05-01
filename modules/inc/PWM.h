#ifndef __PWM_C
#define __PWM_C
#include <stdint.h>
void TB6612_SetSpeed(uint8_t motor, uint16_t speed);
void TB6612_Stop(void);

#endif