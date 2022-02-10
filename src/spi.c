#include "spi.h"

#define one    0xF0
#define zero   0xC0
#define reset  0x00

void SPI_init(void){
  __disable_irq();
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK|SIM_SCGC5_PORTB_MASK;
	SIM->SCGC4 |= SIM_SCGC4_SPI0_MASK;
  PORTA->PCR[7] = PORT_PCR_MUX(3);  // MOSI	
	SPI0->C1 = 0x54;///SPE|MSTR|CPHA SPI system enable\Master/slave mode select/Clock phase
	//1 First edge on SPSCK occurs at the start of the first cycle of a data transfer
	SPI0->BR = 0x10;//dzielimy przez 4
	PORTA->PCR[7] =0;
	__enable_irq();
}
/*----------------------------------------------------------------------------*/

uint8_t SPI_status(void) {
	return SPI0->S;		//sprawzamy czy dane zostaly wyslane i czy bufor jest pusty 
}
/*----------------------------------------------------------------------------*/
void res(int a){
__disable_irq();
	SPI0->D<<=0x01;	
	for(int i=0;i<a;i++){		
		SPI0->D=reset;///wysylamy zera 
		while ((SPI_status() & 0x20) != 0x20){}		
	}
	SPI0->D<<=0x01;	
__enable_irq();
}

/*---------------------------------------------------------------------------*/
void ramka(unsigned char red, unsigned char green, unsigned char blue){
  __disable_irq();
	SPI0->D=0x00;//zerowanie rejestru
	PORTA->PCR[7] = PORT_PCR_MUX(3);//MOSI
	while ((SPI_status() & 0x20) != 0x20){}//oczekiwanie na wyslanie zer 
		/*green*/
	for (unsigned char bit = 0; bit < 8; bit++){		
      if (green & 0x80){		//sprawdzamy najstarszy bit		
        SPI0->D=one;     //zakodowana jedynka
      }else{
        SPI0->D=zero; 	//zakodowane zero
			}	
			green <<= 1;		//przesuwamy bitowo liczbe 
			while ((SPI_status() & 0x20) != 0x20){}  //czekamy na wyslanie    
    } 
	PORTA->PCR[7] =0;
		/*@end geen*/
		////////////////////////////////////////////////////////
		/*red*/
  PORTA->PCR[7] = PORT_PCR_MUX(3);
	for (unsigned char bit = 0; bit < 8; bit++){		
      if (red & 0x80){				
        SPI0->D=one;     
      }else{
        SPI0->D=zero;
			}	
			red <<= 1;
			while ((SPI_status() & 0x20) != 0x20){}

      
    } 
	PORTA->PCR[7] =0;	
		/*@end red*/
	//////////////////////////////////////////////////////////
		/*blue*/
	PORTA->PCR[7] = PORT_PCR_MUX(3);
	for (unsigned char bit = 0; bit < 8; bit++){		
      if (blue & 0x80){				
        SPI0->D=one;     
      }else{
        SPI0->D=zero;
			}			
			if(bit!=8){
				blue <<= 1;
				while ((SPI_status() & 0x20) != 0x20){}			 
			}else{
				PORTA->PCR[7] =0;
			}		 
    } 
	PORTA->PCR[7] =0;
		/*@end blue*/
	__enable_irq();
}
