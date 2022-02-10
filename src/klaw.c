#include "klaw.h"

void Klaw_Init(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;		// Wlaczenie portu A
	PORTA->PCR[S2] |= PORT_PCR_MUX(1);		//ustawienie pinów jako GPIO
	PORTA->PCR[S3] |= PORT_PCR_MUX(1);
	PORTA->PCR[S4] |= PORT_PCR_MUX(1);
	PORTA->PCR[S2] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;//rezystory podciagajace
	PORTA->PCR[S3] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTA->PCR[S4] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
}
void Klaw_S2_4_Int(void)
{
	PORTA -> PCR[S2] |= PORT_PCR_IRQC(0xa);		// 0xa - zbocze opadajace
	PORTA -> PCR[S3] |= PORT_PCR_IRQC(0xa);		
	PORTA -> PCR[S4] |= PORT_PCR_IRQC(0xa);
	
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	NVIC_EnableIRQ(PORTA_IRQn);
}
