#include "led.h"

void LED_Init(void){
	PORTB->PCR[L1] |= PORT_PCR_MUX(1); 
	PTB->PDDR |= (L_1); /* Set pin 13 óf GPIO B as óutput */
	PORTA->PCR[L2] |= PORT_PCR_MUX(1); 
	PTA->PDDR |= (L_2); /* Set pin 9 óf GPIO A as óutput */	
	PORTB->PCR[L3] |= PORT_PCR_MUX(1); 
	PTB->PDDR |= (L_3); /* Set pin 8 óf GPIO B as óutput */	
	
	PTB->PDOR|=(L_1); /* wlaczenie 1 led */
	PTA->PDOR|=(L_2); /* wlaczenie 2 led */
	PTB->PDOR|=(L_3); /* wlaczenie 3 led */
	
	
}
