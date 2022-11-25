#include "stm32f10x.h"
#include "MyTimer.h"
#include "Driver_GPIO.h"
#include "MyUART.h"
#include "Transmission.h"


MyGPIO_Struct_TypeDef PB6; // Sortie PMW
MyGPIO_Struct_TypeDef PB10; // USART TX
MyGPIO_Struct_TypeDef PB11; // USART RX
MyGPIO_Struct_TypeDef PC10; // Plateau Direction

MyTimer_Struct_TypeDef Timer4;

signed char usart_reveived_value;

void callback(void){
	//MyGPIO_Toggle(PA0.GPIO, PA0.GPIO_Pin);
}


int main(void){
	
	// Configuration de la PIN Pour configurer la direction
	PC10.GPIO = GPIOC;
	PC10.GPIO_Pin = 10;
  PC10.GPIO_Conf = Out_Ppull;
	
	MyGPIO_Init(&PC10);
	
	// COnfiguratin des PINS pour la PMW
	
	PB6.GPIO = GPIOB;
	PB6.GPIO_Pin = 6;
  PB6.GPIO_Conf = AltOut_Ppull;
	
	MyGPIO_Init(&PB6);
	
	// Activation de la PMW
	
	MyTimer_PWM( Timer4.Timer , 1 );
	MyTimer_Ratio (Timer4.Timer , 1, 0);
	
	// Configuration du Timer 2 en Periode de 40ms soit une frequence de 25 KHz
	Timer4.Timer = TIM4; 
	Timer4.ARR = 288-1;
	Timer4.PSC = 10-1;
	
	// Configuration des PINS pour l'USART3
	
	PB10.GPIO = GPIOB;
	PB10.GPIO_Pin = 10;
  PB10.GPIO_Conf = AltOut_Ppull;
		
	PB11.GPIO = GPIOB;
	PB11.GPIO_Pin = 11;
  PB11.GPIO_Conf = In_PullUp;
		
			
	// Initialisation de l'USART, du l'Interruption du Receiver
	Transmission_Init(USART3, Timer4, PB10, PB11);	
	
	
	while(1)
	{

		// Get the value of the last data receive
		usart_reveived_value = Transmission_Receive();
		
		// Change the PMW Ratio according to the USART Value received
		if(usart_reveived_value < 0){
			MyGPIO_Set(PC10.GPIO, PC10.GPIO_Pin);
			MyTimer_Ratio (Timer4.Timer , 1, -usart_reveived_value*10);
		}
		else{
			MyGPIO_Reset(PC10.GPIO, PC10.GPIO_Pin);
			MyTimer_Ratio (Timer4.Timer , 1, usart_reveived_value*10);
		}
	}
}
