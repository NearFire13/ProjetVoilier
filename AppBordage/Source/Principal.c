#include "stm32f10x.h"
#include "Driver_GPIO.h"
#include "MyTimer.h"
#include "BordageAutomatique.h"

MyTimer_Struct_TypeDef Timer2;
MyGPIO_Struct_TypeDef PA4;

int a = 0;

int main(void)
{
	
	Timer2.Timer = TIM2;
	Timer2.ARR = 1440-1;

	PA4.GPIO = GPIOA;
	PA4.GPIO_Pin = 4;
  PA4.GPIO_Conf = In_Floating;
	
	BAUTO_Init(Timer2, PA4);
	BAUTO_WaitForIndex(Timer2.Timer, PA4);
	
	while(1)
	{
		a = BAUTO_GetAlpha(Timer2.Timer);
	
	}
}
