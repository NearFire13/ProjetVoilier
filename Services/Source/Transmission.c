#include "Transmission.h"

void Transmission_Init(USART_TypeDef * USART, MyTimer_Struct_TypeDef Timer, MyGPIO_Struct_TypeDef PortTX, MyGPIO_Struct_TypeDef PortRX){
	// Configuration du Timer 2 en Periode de 40ms soit une frequence de 25 KHz
	Timer.Timer = TIM4; 
	Timer.ARR = 288-1;
	Timer.PSC = 10-1;
	MyTimer_Base_Init(&Timer);
	MyTimer_Base_Start(Timer.Timer);
	
	// Configuration des PINS pour l'USART3
	
	MyGPIO_Init(&PortTX);
	MyGPIO_Init(&PortRX);
	
	MyUART_Init(USART);
	MyUART_Active_Receive_IT(USART, 1);
}

void Transmission_Send(USART_TypeDef * USART, char * message, int length){
	// Send Data via USART
	int i = 0;
	for(i = 0; i < length; i++){
		MyUART_Send(USART, *(message + i));
	}
}

signed char Transmission_Receive(){
	return MyUART_Receive();
}
