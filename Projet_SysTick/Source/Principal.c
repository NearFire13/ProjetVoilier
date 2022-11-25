#include "stm32f10x.h"
#include "MySysTick.h"

void Systick_Callback()
{
	
}

int main(void)
{
	Systick_Init(100);
	Systick_Prio_IT(0, Systick_Callback);

	while(1)
	{
	}
}
