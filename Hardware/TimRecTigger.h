#ifndef __TIMRECTIGGER_H
#define __TIMRECTIGGER_H

#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_can.h>
#include <stm32f10x_exti.h>
#include <misc.h>
#include <OLED.h>
#include <Delay.h>
#include <E63_USART1.h>
#include <stm32f10x_tim.h>
#include <string.h>

void TimRecTigger_Init(void);
void TimRecTimer3Delay_Config(int arr, int psc);
void TimRecTimer4Delay_Config(int arr, int psc);


#endif
