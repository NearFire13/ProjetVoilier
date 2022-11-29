#include "MySysTick.h"

void (*ptrSys)(void);

void SysTick_Handler(void)
{
	(*ptrSys)();
}

void Systick_Init(int PERIOD)
{
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE; // Sélectionne la source d'horloge
	SysTick->CTRL |= SysTick_CTRL_ENABLE; // Active le compteur
	SysTick->VAL = 0; // Initialise la valeur du compteur à 0
	SysTick->LOAD = PERIOD; // Fixe la période de reload
	
}

void Systick_Prio_IT(char Prio, void (*Systick_function)(void) )
{
	SysTick->CTRL |= SysTick_CTRL_TICKINT; // Activation de la demande d'interruption
	
	ptrSys = Systick_function;
	
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, Prio);
}
