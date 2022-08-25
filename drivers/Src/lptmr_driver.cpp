//
// Created by wbai on 8/22/2022.
//

#include "lptmr_driver.h"

void LPTMR_Init(){
    LPTMR_CLOCK_EN();
    LPTMR->CSR &= ~(1 << LPTMR_CSR_TEN);
    LPTMR->PSR &= ~(0x2 << 0); // MCGIRCLK as clock source
    LPTMR->PSR |= (1 << LPTMR_PSR_PBYP); // bypass prescaler
//    LPTMR->PSR |= (0x2 << 3);
    LPTMR->CSR &= ~(1 << LPTMR_CSR_TMS); // Time counter mode selected
    LPTMR->CSR |= (1 << LPTMR_CSR_TIE); // Interrupt enable
    LPTMR->CSR |= (1 << LPTMR_CSR_TEN);
}

void LPTMR_Delay(uint16_t counter){
    LPTMR->CMR = (counter & 0xFFFF);
    LPTMR->CSR |= (1 << LPTMR_CSR_TEN);
    while(!(LPTMR->CSR & (1 << LPTMR_CSR_TCF)));
    LPTMR->CSR &= ~(1 << LPTMR_CSR_TEN);
}