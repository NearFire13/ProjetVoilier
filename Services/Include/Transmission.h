#include "stm32f10x.h"
#include "MyTimer.h"
#include "Driver_GPIO.h"
#include "MyUART.h"

void Transmission_Init(USART_TypeDef * USART, MyTimer_Struct_TypeDef Timer, MyGPIO_Struct_TypeDef PortTX, MyGPIO_Struct_TypeDef PortRX);
void Transmission_Send(USART_TypeDef * USART, char * message, int length);
signed char Transmission_Receive(void);
