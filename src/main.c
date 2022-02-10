#include "MKL05Z4.h"
#include "spi.h"
//#include <stdio.h>
//#include <string.h>
#include <stdlib.h>
#include "klaw.h"
#include "led.h"
#include <time.h>
#include "tpm_pcm.h" 
#include "led.h"
#include "lcd1602.h"
//#define predkosc 400000
#define dl_weza 5
static volatile uint8_t S2_press=0;	// zmiana kierunku poruszania sie 
static volatile uint8_t S3_press=0;	// zmiana koloru[menu]
static volatile uint8_t S4_press=0;	// zatwierdzanie zmian i rozpoczecie gry[menu]

/*podprogra obslugujacy przerwania dla S2 S3 S4*/
void PORTA_IRQHandler(void){											
	uint32_t buf;
	buf=PORTA->ISFR & (S2_MASK | S3_MASK | S4_MASK);
	switch(buf){
		case S2_MASK:
			S2_press^=0xFF;//jesli S2 zostal wcisniety to S2_press zmienia wartosc na przeciwna 
		break;
		
		case S3_MASK:
			S3_press=1;//jezli S3 zostal wcisniety to S3_press równa sie 1
		break;
		
		case S4_MASK:
			S4_press^=0xFF;//jesli S4 zostal wcisniety to S4_press zmienia wartosc na przeciwna 
		break;
		
		default: break;	
	}
	PORTA->ISFR |= S2_MASK | S3_MASK | S4_MASK;	// Kasowanie wszystkich bitów ISF
	NVIC_ClearPendingIRQ(PORTA_IRQn);		
}
/*@koniec podprogramu obslugujacego przerwania dla S2 S3 S4*/

/*main*/
int main(void){
	Klaw_Init();				//inicjalizacja klawiatury
	Klaw_S2_4_Int();		// Klawisze S2, S3 i S4 zglaszaja przerwanie
	TPM0_Init_PCM (); 	//inicjalizacja glosnika 
	LED_Init();					//inicjalizacja led
	SPI_init();					//inicjalizacja SPI
	LCD1602_Init(); /* initialize LCD */

	int a=0,b=1,i=0,j=0,z=0,x=0,ilosc=0,licznik=0,pola=0,predkosc=400000,speed=0;
	unsigned char czerwony=0x00,zielony=0x00,niebieski=0x00;
	
	start:TPM0_PCM_Play(0,1);
	LCD1602_ClearAll();
	LCD1602_SetCursor(3,0);
	LCD1602_Print("nowa gra");
	LCD1602_SetCursor(2,1);
	LCD1602_Print("powodzenia!!");
	DELAY(800)//8000
	LCD1602_ClearAll();
	/*main loop*/
	while(1){
		/*menu(wybor koloru i potwierdzenie rozpoczecia rozgrywki)*/
		while(1){
			/*S3*/			
			if(S3_press){///sprawdzanie czy S3 zostal wcisniety
				ilosc++;//jezeli start to zwiekszamy licznik czyli wybieramy kolejny kolor
				LCD1602_ClearAll();
				if(ilosc==3){// mamy do wyboru tylko 3 kolory 
					ilosc=0;					
				}
				S3_press=0;
			}
			/*end S3*/
			
			/*S2*/
			if(S2_press){///sprawdzanie czy S2 zostal wcisniety
				speed++;//jezeli start to zwiekszamy licznik czyli wybieramy kolejny tryb
				LCD1602_ClearAll();
				if(speed==3){// mamy do wyboru tylko 3 tryby 
					speed=0;					
				}
				S2_press=0;
			}
			/*end S2*/
			
			/*ilosc*///w zaleznosci od ilosci wcisniec S3 wybierany jest odpowiedni kolor
			if(ilosc==0){//zero wcisniec czyli kolor bedzie niebieski
				niebieski=0xff;
				zielony=0x00;
				czerwony=0x00;
				LCD1602_SetCursor(0,0);
				LCD1602_Print("wybrany kolor: ");	
				LCD1602_SetCursor(0,1);
				LCD1602_Print("niebieski");					
			}
			if(ilosc==1){//jedno wcisniecie czyli kolor bedzie zielony
				niebieski=0x00;
				zielony=0xff;
				czerwony=0x00;	
				LCD1602_SetCursor(0,0);
				LCD1602_Print("wybrany kolor: ");	
				LCD1602_SetCursor(0,1);
				LCD1602_Print("zielony");					
			}
			if(ilosc==2){//dwa wcisniecia czyli kolor bedzie czerwony
				niebieski=0x00;
				zielony=0x00;
				czerwony=0xff;
				LCD1602_SetCursor(0,0);
				LCD1602_Print("wybrany kolor: ");	
				LCD1602_SetCursor(0,1);
				LCD1602_Print("czerwony");					
			}
			/*@ end ilosc*/
			
			/*predkosc*///w zaleznosci od ilosci wcisniec S2 wybierany jest odpowiedni tryb
			if(speed==0){//zero wcisniec czyli tryb easy
				predkosc=30000;
				LCD1602_SetCursor(10,1);
				LCD1602_Print("tryb:1");						
			}
			if(speed==1){//jedno wcisniecie czyli tryb medium
				predkosc=20000;	
				LCD1602_SetCursor(10,1);
				LCD1602_Print("tryb:2");				
			}
			if(speed==2){//dwa wcisniecia czyli tryb hard
				predkosc=10000;
				LCD1602_SetCursor(10,1);
				LCD1602_Print("tryb:3");				
			}
			/*@ end predkosc*/
			
			/*zatwierdzenie koloru/rozpoczecie gry*/
			if(S4_press){
				if(niebieski==0xFF | zielony==0xFF | czerwony==0xFF){//sprawdzenie czy na pewno zostal wybrany jakis kolor
					ilosc=0;			 	//
					pola=0;
					S2_press=0;			//zerowanie przyciskow 
					S3_press=0;			//poniewaz mogly zostac przypadkowo wcisniete
					S4_press=0;		  //
					LCD1602_ClearAll();
					LCD1602_SetCursor(3,0);
					LCD1602_Print("start!!!!");
					TPM0_PCM_Play(0,0);
					
					break;					//przerwanie petli "menu"
				}			
			}
			/*@koniec zatwierdzenia*/
		}
		/*@ end menu*/


		poczatek:
//////////////////////////////////		
		res(400);									////
		PORTA->PCR[7]=0;					////
		for(int p=0;p<64;p++){		////restart matrycy 8x8
			ramka(0x00,0x00,0x00);		////	
		}													////
//////////////////////////////////	
		a=rand() % 46;						//losowanie liczby z przedzialu od 8 do 54
		a+=8;											//
		/*porusznanie sie w przod*/
		if(S2_press){//czy S2 jest 0 czy 1?
			first:
			/*skok o jeden w przod*/
			for(j=z;j<64;j++){
								//LCD1602_ClearAll();
								//LCD1602_SetCursor(0,0);
							//	LCD1602_Print("wynik:");							
							//	LCD1602_SetCursor(0, 1);
							//	LCD1602_PrintNum(pola);	
				/*zderzenie*/
				if(j==a){
					pola+=j;
					b++;//zwiekszamy dlugosc weza
					j=0;//zerujemy liczniki
					i=0;//
					for(int p=0;p<64;p++){		////
						ramka(0xFF,0x00,0xFF);		////	sygnal swetlny gdy trafimy w przeszkode			
					}													////
					res(400000);							////
					/*dlugosc*/
					if(b==dl_weza){//jesli dlugosc weza bedzie rowna podanej to gasimy jedna led "zycie" i redukujemy weza
						licznik++;//zwikszamy ilosc straconych zyc
						b=1;//zmniejszamy dl weza do poczatkowej

						if(licznik==1){
							PTB->PDOR&=~(1<<13);//zgaszenie 1 led
						}
						if(licznik==2){
							PTB->PDOR&=~(1<<8);	//zgaszenie 2 led
						}
						if(licznik==3){							
							PTA->PDOR&=~(1<<9);//zgaszenie 3 led
						}
						
						if(licznik>3){///jesli stracimy zycia to restartujemy cala gre
							for(int k=0;k<64;k++){		//
								ramka(0x00,0x00,0x00);		//reset matrycy 
							}
							PTB->PDOR|=(1<<13);///							
							PTA->PDOR|=(1<<9);///	zapalenie led	
							PTB->PDOR|=(1<<8);///
							licznik=0;
								LCD1602_ClearAll();
								LCD1602_SetCursor(1,0);
								LCD1602_Print("game over");
								LCD1602_SetCursor(0,1);
								LCD1602_Print("wynik:");							
								LCD1602_SetCursor(8, 1);
								LCD1602_PrintNum(pola);							
								DELAY(3000)
							goto start;		///skok do poczatku gry					
						}else{
							a=0;b=1;i=0;j=0;z=0;x=0;ilosc=0;
							goto poczatek;//skok do nastepnej rundy
						}
					}
					/*@end dlugosc*/				
				}
				/*@end zderzenie*/
/////////////////////////////////				
				if(j==63){						//przejscie przez "sciane"
					a=rand() % 46;			//
					a+=8;								//
					j=0;								//
					z=0;								//			
				}											//
////////////////////////////////						
				
				if(S2_press==0x00){						///
					pola+=j;
					goto next;					///jesli wcisniemy przycisk to zmieniamy kierunek 
				}											///
				/*odswierzanie matrycy*/
				for(i=0;i<64;i++){
					if(i==j){//sprawdzanie czy to nasz waz
						for(int k=0;k<b;k++){
							ramka(czerwony,zielony,niebieski);	//wyswietlanie odpowiedniej dl weza
						}						
					}else{
						if(i==a){
							ramka(0xAA,0x00,0xAA);	///przeszkoda
						}else{
							ramka(0x00,0x00,0x00);//reszta led 
						}					
					}				
				}
				/*@end odswierzanie matrycy*/	
				res(predkosc);/////////zmiana predkosci poruszania sie
			}
			/*@end skok o jeden w przod*/			
/*------------------------------------------------------------------------------------------------*/		
		}else{/*@end porusznanie sie w przod*/	
			/*porusznanie sie w tyl*/
			next:
			/*skok jeden w tyl*/
			for(z=j;z>=0;z--){
							//	LCD1602_ClearAll();
							//	LCD1602_SetCursor(0,0);
							//	LCD1602_Print("wynik:");							
							//	LCD1602_SetCursor(0, 1);
							//	LCD1602_PrintNum(pola);	
				/*zderzenie*/
				if(z==a){
					pola+=(63-z);
					b++;//zwiekszamy dlugosc weza
					z=0;//zerujemy liczniki
					x=0;//
					for(int p=0;p<64;p++){		////
						ramka(0xFF,0x00,0xFF);		////	sygnal swetlny gdy trafimy w przeszkode		
					}													////
					res(400000);							////
					/*dlugosc*/
					if(b==dl_weza){//jesli dlugosc weza bedzie rowna podanej to gasimy jedna led "zycie" i redukujemy weza
						licznik++;//zwikszamy ilosc straconych zyc
						b=1;//zmniejszamy dl weza do poczatkowej
						
						if(licznik==1){
							PTB->PDOR&=~(1<<13);	//zgaszenie 1 led						
						}
						if(licznik==2){
							PTB->PDOR&=~(1<<8);		//zgaszenie 2 led
						}
						if(licznik==3){
							PTA->PDOR&=~(1<<9);		//zgaszenie 3 led
						}
						
						if(licznik>3){///jesli stracimy zycia to restartujemy cala gre
							for(int k=0;k<64;k++){		//
								ramka(0x00,0x00,0x00);		//reset matrycy 
							}
							PTB->PDOR|=(1<<13);//
							PTA->PDOR|=(1<<9);///	zapalenie led	
							PTB->PDOR|=(1<<8);///	
							licznik=0;
							LCD1602_ClearAll();
							LCD1602_SetCursor(1,0);
							LCD1602_Print("game over");
							LCD1602_SetCursor(0,1);
							LCD1602_Print("wynik:");							
							LCD1602_SetCursor(8, 1);
							LCD1602_PrintNum(pola);							
							DELAY(3000)
							goto start;		///skok do poczatku gry	
						}else{
							a=0;b=1;i=0;j=0;z=0;x=0;ilosc=0;
							goto poczatek;//skok do nastepnej rundy
						}
					}
					/*@end dlugosc*/				
				}
				/*@end zderzenie*/
////////////////////////////				
				if(z==0){				//
					a=rand() % 46;	//
					a+=8;					//przejscie przez "sciane"
					j=63;					//
					z=63;					//
				}								//
/////////////////////////////		
				if(S2_press==0xFF){		///
					pola+=(63-z);
					goto first;					///jesli wcisniemy przycisk to zmieniamy kierunek 					
				}											///
				/*odswierzanie matrycy*/
				for(x=0;x<64;x++){
					if(x==z){//sprawdzanie czy to nasz waz
						for(int k=0;k<b;k++){	
							ramka(czerwony,zielony,niebieski);//wyswietlanie odpowiedniej dl weza
						}
					}else{
						if(x==a){
							ramka(0xAA,0x00,0xAA);		///przeszkoda
						}else{
							ramka(0x00,0x00,0x00);		///reszta led 
						}						
					}					
				}
				/*@end odswierzanie matrycy*/
				res(predkosc);
				
			}			
			/*@end skok jeden w tyl*/
		}
		/*@end porusznanie sie w tyl*/
/////////////////////////////////	
		PORTA->PCR[7]=0;					//
		for(int k=0;k<64;k++){		//
			ramka(0x00,0x00,0x00);		//reset matrycy 
		}													//
		res(400000); 							//
		PORTA->PCR[7] =0;					//
////////////////////////////////		
		
	}
	/*@ end main loop*/	
}
/*@koniec main*/
