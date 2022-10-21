#include "Driver_GPIO.h"

void MyGPIO_Init ( MyGPIO_Struct_TypeDef * GPIOStructPtr ){
	if(GPIOStructPtr->GPIO == GPIOA){
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	}
	if(GPIOStructPtr->GPIO == GPIOB){
		RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	}
	if(GPIOStructPtr->GPIO == GPIOC){
		RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	}
	if(GPIOStructPtr->GPIO == GPIOD){
		RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
	}
	if(GPIOStructPtr->GPIO == GPIOE){
		RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
	}
	
	if(GPIOStructPtr->GPIO_Conf == 0x9){
		GPIOStructPtr->GPIO->CRL &= ~(0xF << GPIOStructPtr->GPIO_Pin*4);
		GPIOStructPtr->GPIO->CRL |= 0x8 << GPIOStructPtr->GPIO_Pin*4;
		GPIOStructPtr->GPIO->ODR |= 0x1 << 0x8;
	}
	else if(GPIOStructPtr->GPIO_Pin < 8){
		GPIOStructPtr->GPIO->CRL &= ~(0xF << GPIOStructPtr->GPIO_Pin*4);
		GPIOStructPtr->GPIO->CRL |= GPIOStructPtr->GPIO_Conf << GPIOStructPtr->GPIO_Pin*4;
	}
	else{
		GPIOStructPtr->GPIO->CRH &= ~(0xF << (GPIOStructPtr->GPIO_Pin%8)*4);
		GPIOStructPtr->GPIO->CRH |= GPIOStructPtr->GPIO_Conf << (GPIOStructPtr->GPIO_Pin%8)*4;
	}
}

int MyGPIO_Read ( GPIO_TypeDef * GPIO , char GPIO_Pin ){
	if((GPIO->IDR & (0x01<<GPIO_Pin)) == 0){
		return 0;
	}
	else{
		return 1;
	}
} // renvoie 0 ou autre chose different de 0


void MyGPIO_Set ( GPIO_TypeDef * GPIO , char GPIO_Pin ){
	GPIO->ODR |= 0x01<<(GPIO_Pin);
}
void MyGPIO_Reset ( GPIO_TypeDef * GPIO , char GPIO_Pin ){
	GPIO->ODR &= ~(0x01<<(GPIO_Pin));
}
void MyGPIO_Toggle ( GPIO_TypeDef * GPIO , char GPIO_Pin ){
		GPIO->ODR ^= 0x01<<GPIO_Pin;
}
