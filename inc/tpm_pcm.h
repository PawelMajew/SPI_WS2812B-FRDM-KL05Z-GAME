#ifndef TPM_PCM_H
#define TPM_PCM_H

//#include "frdm_bsp.h"

/**
 * @brief TPM0 initialization. PCM.
 */
void TPM0_Init_PCM(void);
/**
 * @brief Play wave once.
 *
 */
void TPM0_PCM_Play(int sam,int flag);

#endif /* TPM_PCM_H */
