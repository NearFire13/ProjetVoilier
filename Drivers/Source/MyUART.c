#include "MyUART.h"
signed char lastValueReceived = 0;

void USART1_IRQHandler(void){
	signed char temp;
	if(USART1->SR & USART_SR_RXNE){
		temp = USART1->DR;
		if(temp != '\n' && temp != '\r'){
			lastValueReceived = temp;
		}
	}
}

void USART2_IRQHandler(void){
	signed char temp;
	if(USART2->SR & USART_SR_RXNE){
		temp = USART2->DR;
		if(temp != '\n' && temp != '\r'){
			lastValueReceived = temp;
		}
	}
}

void USART3_IRQHandler(void){
	signed char temp;
	if(USART3->SR & USART_SR_RXNE){
		temp = USART3->DR;
		if(temp != '\n' && temp != '\r'){
			lastValueReceived = temp;
		}
	}
}

void MyUART_Init(USART_TypeDef * USART){
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	USART->CR1 = 0x00;
	USART->CR1 |= USART_CR1_UE; // Activer l'USART
	USART->CR1 |= USART_CR1_TE; // Autoriser la Transmission
	USART->CR1 |= USART_CR1_RE; // Autoriser la Reception
	USART->CR1 |= USART_CR1_RXNEIE; // Activer l'interruption en cas de reception d'une donnée
	USART->BRR = (234 << 4) | (6 << 0);  // Determiner le baud rate en donnant la mantisse et la fraction BR = CLK/(16*USARTDIV)
}

void MyUART_Send(USART_TypeDef * USART, char data){
	while(!(USART->SR & USART_SR_TXE));
	USART->DR = data; // Put the data into the DR Register
	while(!(USART->SR & USART_SR_TC)); // Wait for the transmission to be complete
}

void MyUART_Active_Receive_IT(USART_TypeDef * USART, char Prio){
	if(USART == USART1){
		NVIC_EnableIRQ(USART1_IRQn);
		NVIC_SetPriority(USART1_IRQn, Prio);
	}
	if(USART == USART2){
		NVIC_EnableIRQ(USART2_IRQn);
		NVIC_SetPriority(USART2_IRQn, Prio);
	}
	if(USART == USART3){
		NVIC_EnableIRQ(USART3_IRQn);
		NVIC_SetPriority(USART3_IRQn, Prio);
	}
}

signed char MyUART_Receive(void){
	return lastValueReceived;
}
