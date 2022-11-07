#include "BordageAutomatique.h"

void BAUTO_Init(MyTimer_Struct_TypeDef Timer, MyGPIO_Struct_TypeDef GPIO)
{
	MyTimer_Base_Init(&Timer);
	MyGPIO_Init(&GPIO);
	MyTimer_Base_Start(Timer.Timer);
	MyTimer_CodeurIncremental(Timer.Timer);
	MyTimer_CodeurIncremental(Timer.Timer);
}

int BAUTO_GetAlpha(TIM_TypeDef * Timer)
{
	return Timer->CNT / 4;
}

void BAUTO_WaitForIndex(TIM_TypeDef * Timer, MyGPIO_Struct_TypeDef GPIO)
{
	while(MyGPIO_Read(GPIO.GPIO, GPIO.GPIO_Pin) == 0)
	{
	}
	Timer->CNT = 0;
}
