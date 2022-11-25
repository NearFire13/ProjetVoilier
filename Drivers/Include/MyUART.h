#ifndef USART_H

#define USART_H
#include "stm32f10x.h"

void MyUART_Init(USART_TypeDef * USART);

void MyUART_Send(USART_TypeDef * USART, char data);

void MyUART_Active_Receive_IT(USART_TypeDef * USART, char Prio);

signed char MyUART_Receive(void);

#endif
