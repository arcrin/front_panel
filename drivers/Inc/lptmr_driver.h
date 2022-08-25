//
// Created by wbai on 8/22/2022.
//

#ifndef FRONT_PANEL_LPTMR_DRIVER_H
#define FRONT_PANEL_LPTMR_DRIVER_H

#include "k32lb11.h"

/*************************************
 * CSR bit config
 *************************************/
#define LPTMR_CSR_TCF       7
#define LPTMR_CSR_TIE       6
#define LPTMR_CSR_TPS       4
#define LPTMR_CSR_TPP       3
#define LPTMR_CSR_TFC       2
#define LPTMR_CSR_TMS       1
#define LPTMR_CSR_TEN       0

/*************************************
 * PSR bit config
 *************************************/
#define LPTMR_PSR_PRESCAKE      3
#define LPTMR_PSR_PBYP          2
#define LPTMR_PSR_PCS           0

void LPTMR_Init();

void LPTMR_Delay(uint16_t counter);

#endif //FRONT_PANEL_LPTMR_DRIVER_H
