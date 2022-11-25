#include "MyTimer.h"

void (*ptr)(void);

void TIM1_UP_IRQHandler(void){
	TIM1->SR &= ~(0x1);
	(*ptr)();
}
void TIM2_IRQHandler(void) {
	TIM2->SR &= ~(0x1);
	(*ptr)();
}

void TIM3_IRQHandler(void) {
	TIM3->SR &= ~(0x1);
	(*ptr)();
}

void TIM4_IRQHandler(void) {
	TIM4->SR &= ~(0x1);
	(*ptr)();
}

void MyTimer_Base_Init (MyTimer_Struct_TypeDef * Timer ) {
	if(Timer->Timer == TIM1){
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	}
	if(Timer->Timer == TIM2){
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	}
	if(Timer->Timer == TIM3){
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	}
	if(Timer->Timer == TIM4){
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	}
	
	Timer->Timer->ARR = Timer->ARR;
	Timer->Timer->PSC = Timer->PSC;
}


void MyTimer_ActiveIT ( TIM_TypeDef * Timer , char Prio, void (* callback)(void) ) {
	Timer->DIER |= 0x1;
	Timer->SR &= ~(0x1);
	ptr = callback;
	if(Timer == TIM1){
		NVIC_EnableIRQ(TIM1_UP_IRQn);
		NVIC_SetPriority(TIM1_UP_IRQn, Prio);
	}
	if(Timer == TIM2){
		NVIC_EnableIRQ(TIM2_IRQn);
		NVIC_SetPriority(TIM2_IRQn, Prio);
	}
	if(Timer == TIM3){
		NVIC_EnableIRQ(TIM3_IRQn);
		NVIC_SetPriority(TIM3_IRQn, Prio);
	}
	if(Timer == TIM4){
		NVIC_EnableIRQ(TIM4_IRQn);
		NVIC_SetPriority(TIM4_IRQn, Prio);
	}
}

void MyTimer_PWM( TIM_TypeDef * Timer , char Channel ) {
	if(Timer == TIM1){
		if(Channel == 1){
			Timer->CCMR1 |= 0x1 << 5 | 0x1 <<  6;
			Timer->CCMR1 &= ~(0x1 << 4);
			Timer->CCMR1 |= 0x1 << 3;
			Timer->CCER |= 0x1;
			Timer->CCER |= 0x1 << 2;
		}
		if(Channel == 2){
			Timer->CCMR1 |= 0x1 << 14 | 0x1 <<  13;
			Timer->CCMR1 &= ~(0x1 << 12);
			Timer->CCMR1 |= 0x1 << 11 ;
			Timer->CCER |= 0x1 << 4 ;
			Timer->CCER |= 0x1 << 6;
		}
		if(Channel == 3){
			Timer->CCMR2 |= 0x1 << 5 | 0x1 <<  6;
			Timer->CCMR2 &= ~(0x1 << 4);		
			Timer->CCMR2 |= 0x1 << 3;
			Timer->CCER |= 0x1 << 8 ;
			Timer->CCER |= 0x1 << 10;
		}
		if(Channel == 4){
			Timer->CCMR2 |= 0x1 << 14 | 0x1 <<  13;
			Timer->CCMR2 &= ~(0x1 << 12);
			Timer->CCMR2 |= 0x1 << 11 ;
			Timer->CCER |= 0x1 << 12 ;
		}
		Timer->BDTR |= 0x1 << 15;
	}else{
		if(Channel == 1){
			Timer->CCMR1 |= 0x1 << 5 | 0x1 <<  6;
			Timer->CCMR1 &= ~(0x1 << 4);
			Timer->CCMR1 |= 0x1 << 3;
			Timer->CCER |= 0x1;
		}
		if(Channel == 2){
			Timer->CCMR1 |= 0x1 << 14 | 0x1 <<  13;
			Timer->CCMR1 &= ~(0x1 << 12);
			Timer->CCMR1 |= 0x1 << 11 ;
			Timer->CCER |= 0x1 << 4 ;
		}
		if(Channel == 3){
			Timer->CCMR2 |= 0x1 << 5 | 0x1 <<  6;
			Timer->CCMR2 &= ~(0x1 << 4);		
			Timer->CCMR2 |= 0x1 << 3;
			Timer->CCER |= 0x1 << 8 ;
		}
		if(Channel == 4){
			Timer->CCMR2 |= 0x1 << 14 | 0x1 <<  13;
			Timer->CCMR2 &= ~(0x1 << 12);
			Timer->CCMR2 |= 0x1 << 11 ;
			Timer->CCER |= 0x1 << 12 ;
		}
	}
	
	Timer->CR1 |= 0x1 << 7;
	Timer->EGR |= 0x1;
}

void MyTimer_Ratio ( TIM_TypeDef * Timer , char Channel, char ratio ) {
	if(Channel == 1){
			Timer->CCR1 = (ratio * Timer->ARR) / 1000;
		}
		if(Channel == 2){
			Timer->CCR2 = (ratio * Timer->ARR) / 1000;
		}
		if(Channel == 3){
			Timer->CCR3 = (ratio * Timer->ARR) / 1000;
		}
		if(Channel == 4){
			Timer->CCR4 = (ratio * Timer->ARR) / 1000;
		}
}
 
