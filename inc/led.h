#ifndef LED_H
#define LED_H
#include "MKL05Z4.h"

#define L_1	(1<<13)		// Maska dla led L1
#define L_2	(1<<9)		// Maska dla led L2
#define L_3	(1<<8)		// Maska dla led L3
#define L1	13						// Numer bitu dla led L1
#define L2	9						// Numer bitu dla led L2
#define L3	8						// Numer bitu dla led L3

void LED_Init(void);
#endif  /* KLAW_H */
