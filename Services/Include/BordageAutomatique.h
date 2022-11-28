#ifndef BAUTO_H

#define BAUTO_H
#include "stm32f10x.h"
#include "Driver_GPIO.h"
#include "MyTimer.h"

void BAUTO_Init(MyTimer_Struct_TypeDef Timer, MyGPIO_Struct_TypeDef GPIO);

int BAUTO_GetAlpha(TIM_TypeDef * Timer);

void BAUTO_WaitForIndex(TIM_TypeDef * Timer, MyGPIO_Struct_TypeDef GPIO);

#endif
