#include "Transmission.h"
#include "BordageAutomatique.h"
#include "MySysTick.h"

MyGPIO_Struct_TypeDef PB6; // Sortie PMW pour le Plateau
MyGPIO_Struct_TypeDef PB10; // USART TX
MyGPIO_Struct_TypeDef PB11; // USART RX
MyGPIO_Struct_TypeDef PC10; // Plateau Direction

MyTimer_Struct_TypeDef Timer4; // Pour la PMW du Plateau

MyTimer_Struct_TypeDef Timer2; // Pour le Codeur Incremental
MyTimer_Struct_TypeDef Timer3; // Pour la PMW du ServoMoteur
MyGPIO_Struct_TypeDef PA4; // Index du Codeur Incremental
MyGPIO_Struct_TypeDef PA6; // Sortie PMW pour le Servo Moteur

int a = 0; // Alpha : angle de la girouette
int teta = 0;
int ratio = 0;

int compteur = 0; // Choisir les fonctions a appeler par l'interruption du Systik

// Definition de quelque messages à envoyer à la commande en fonction de la position du voilier
char * message_debout       = "Le voilier navigue en vent debout       \r\n";
char * message_pres         = "Le voilier navigue au pres              \r\n";
char * message_plein        = "Le voilier navigue en bon plein         \r\n";
char * message_travers      = "Le voilier navigue en vent de travers   \r\n";
char * message_grand_largue = "Le voilier navigue en grand largue      \r\n";
char * message_arriere      = "Le voilier navigue en vent arriere      \r\n";

signed char usart_reveived_value; // Valeur recu depuis la commande

void interactionGirouetteServoMoteur(void){
	// Recuperation de la valeur de l'angle alpha
	a = BAUTO_GetAlpha(Timer2.Timer);
	if((a >= 0 && a <= 45) || (a >= 315 && a < 360)) // Calcul en vent avant
	{
		teta = 0;
	}
	else if((a > 45 && a < 180)) // Calcul en Babord
	{
		teta = (2*a/3)-30;
	}
	else // Calcul en Tribord
	{
		teta = -(2*a/3)+210;
	}
	
	// Calcul et Mouvement du Servo Moteur
	ratio = -(50*teta)/90+100;
	MyTimer_Ratio(Timer3.Timer, 1, ratio);
}

void interactionUsartPlateau(void){
	// Prendre la valeur de la dernière donnée recu
	usart_reveived_value = Transmission_Receive();
	
	// Changer le ratio PMW en fonction de la valeur recu de l'USART
	if(usart_reveived_value < 0){
		MyGPIO_Set(PC10.GPIO, PC10.GPIO_Pin);
		MyTimer_Ratio(Timer4.Timer , 1, -usart_reveived_value*10);
	}
	else{
		MyGPIO_Reset(PC10.GPIO, PC10.GPIO_Pin);
		MyTimer_Ratio(Timer4.Timer , 1, usart_reveived_value*10);
	}
}

void interactionPositionVoilierUsart(void){
	// Envoie des messages sur le positionnement du bateau
	if((a > 45 && a <= 50) || (a >= 310 && a < 315)){
		Transmission_Send(USART3, message_pres, 45);
	}else if((a > 50 && a <= 60) || (a >= 300 && a < 310)){
		Transmission_Send(USART3, message_plein, 45);
	}else if((a > 60 && a <= 100) || (a >= 260 && a < 300)){
		Transmission_Send(USART3, message_travers, 45);
	}else if((a > 100 && a <= 170) || (a >= 190 && a < 260)){
		Transmission_Send(USART3, message_grand_largue, 45);
	}else if((a > 170 && a < 190)){
		Transmission_Send(USART3, message_arriere, 45);
	}else{
		Transmission_Send(USART3, message_debout, 45);
	}
}

void Systick_Callback()
{
	compteur++;
	if(compteur == 1){
		interactionGirouetteServoMoteur();
		interactionUsartPlateau();
	}else if(compteur == 2){
		interactionPositionVoilierUsart();
	}else{
		compteur = 0;
	}
}

int main(void){
	// Configuration du Timer 2 pour le Codeur Incremental
	Timer2.Timer = TIM2;
	Timer2.ARR = 1440-1;
	
	// Configuratin de la PIN pour la PMW Servo Moteur
	PA6.GPIO = GPIOA;
	PA6.GPIO_Pin = 6;
	PA6.GPIO_Conf = AltOut_Ppull;
	
	MyGPIO_Init(&PA6);
	
	// Configuration du Timer 3 en Periode de 20ms
	Timer3.Timer = TIM3;
	Timer3.ARR = 14400-1;
	Timer3.PSC = 100-1;
	MyTimer_Base_Init(&Timer3);
	MyTimer_Base_Start(Timer3.Timer);

	// COnfiguratin de la PIN pour l'index du Codeur Incremental
	PA4.GPIO = GPIOA;
	PA4.GPIO_Pin = 4;
	PA4.GPIO_Conf = In_Floating;
	
	
	// Configuration de la PIN Pour configurer la direction du plateau
	PC10.GPIO = GPIOC;
	PC10.GPIO_Pin = 10;
  PC10.GPIO_Conf = Out_Ppull;
	
	MyGPIO_Init(&PC10);
	
	// COnfiguratin de la PIN pour la PMW du Plateau
	
	PB6.GPIO = GPIOB;
	PB6.GPIO_Pin = 6;
  PB6.GPIO_Conf = AltOut_Ppull;
	
	MyGPIO_Init(&PB6);
	
	// Configuration du Timer 4 en Periode de 40ms soit une frequence de 25 KHz
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
	
	// Activation de la PMW du Plateau
	
	MyTimer_PWM(Timer4.Timer , 1 );
	MyTimer_Ratio(Timer4.Timer , 1, 0);
	
	// Configuration du Bordage Automatique et Attente de l'index
	BAUTO_Init(Timer2, PA4);
	BAUTO_WaitForIndex(Timer2.Timer, PA4);
	
	// Activation de la PMW par defaut en Vent Arriere
	MyTimer_PWM(Timer3.Timer, 1);
	MyTimer_Ratio(Timer3.Timer, 1, 50); // (50/90)*têta+50
	
	// Activer le Systik et mettre la periode de comptage sur 100 ms
	Systick_Init(7200000);
	Systick_Prio_IT(0, Systick_Callback);
	
	while(1)
	{
	}
}
