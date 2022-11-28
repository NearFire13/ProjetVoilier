#include "stm32f10x.h"

// Configure le timer Systick avec une périodicité donnée
void Systick_Init(int PERIOD);

// Associe une fonction d'interruption (callback) lors du débordement du Systick
void Systick_Prio_IT(char Prio, void (*Systick_function)(void));
