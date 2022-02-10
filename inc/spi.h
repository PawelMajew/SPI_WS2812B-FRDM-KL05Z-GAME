#include "MKL05Z4.h"
#ifndef SPI
#define SPI
void SPI_init(void);
uint8_t SPI_status(void);
void res(int a);
void ramka(unsigned char red, unsigned char green, unsigned char blue);
#endif
