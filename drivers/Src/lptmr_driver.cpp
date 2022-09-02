//
// Created by wbai on 8/22/2022.
//

#include "lptmr_driver.h"

void LPTMR_Init(){
    LPTMR_CLOCK_EN();
    LPTMR->CSR &= ~(1 << LPTMR_CSR_TEN); // Timer disabled
    LPTMR->PSR &= ~(0x2 << 0); // MCGIRCLK as clock source
//    LPTMR->PSR |= (1 << LPTMR_PSR_PBYP); // bypass prescaler
    LPTMR->PSR |= (0x7 << LPTMR_PSR_PRESCALE);
    LPTMR->CSR &= ~(1 << LPTMR_CSR_TMS); // Time counter mode selected
    LPTMR->CSR |= (1 << LPTMR_CSR_TIE); // Interrupt enable
//    LPTMR->CMR = (0x7A12 & 0xFFFF);
    LPTMR->CMR = (0xC35 & 0xFFFF);
    LPTMR->CSR |= (1 << LPTMR_CSR_TEN); // Timer enabled
    IRQPriorityConfig(IRQ_NUMBER_LPTMR0, 3);
    InterruptConfig(IRQ_NUMBER_LPTMR0, ENABLE);
}

void LPTMR_Delay(uint16_t counter){
    LPTMR->CMR = (counter & 0xFFFF);
    LPTMR->CSR |= (1 << LPTMR_CSR_TEN);
    while(!(LPTMR->CSR & (1 << LPTMR_CSR_TCF)));
    LPTMR->CSR &= ~(1 << LPTMR_CSR_TEN);
}