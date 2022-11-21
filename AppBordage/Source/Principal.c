#include "stm32f10x.h"
#include "Driver_GPIO.h"
#include "MyTimer.h"
#include "BordageAutomatique.h"

MyTimer_Struct_TypeDef Timer2, Timer3;
MyGPIO_Struct_TypeDef PA4, PA6;

int a = 0;
int teta = 0;
int ratio = 0;

int main(void)
{
	
	Timer2.Timer = TIM2;
	Timer2.ARR = 1440-1;
	
	Timer3.Timer = TIM3;
	Timer3.ARR = 14400-1;
	Timer3.PSC = 10-1;
	MyTimer_Base_Init(&Timer3);
	MyTimer_Base_Start(Timer3.Timer);

	PA4.GPIO = GPIOA;
	PA4.GPIO_Pin = 4;
  PA4.GPIO_Conf = In_Floating;
	
	PA6.GPIO = GPIOA;
	PA6.GPIO_Pin = 6;
  PA6.GPIO_Conf = AltOut_Ppull;
	
	MyGPIO_Init(&PA6);
	
	MyTimer_PWM(Timer3.Timer, 1);
	MyTimer_Ratio(Timer3.Timer, 1, 50); // (50/180)*têta+50
	
	BAUTO_Init(Timer2, PA4);
	BAUTO_WaitForIndex(Timer2.Timer, PA4);

	while(1)
	{
		a = BAUTO_GetAlpha(Timer2.Timer); // Si alpha > 45 => têta = (2/3)*a - 30 sinon têta = 0
		if((a > 45 && a < 180))
		{
			teta = (2*a/3)-30;
		}
		else if((a > 0 && a <= 45) || (a >= 315 && a < 360))
		{
			teta = 0;
		}
		else
		{
			teta = -(2*a/3)+210;
		}
		ratio = (50*teta/180)+50;
		MyTimer_Ratio(Timer3.Timer, 1, ratio);
	}
}
