//
// Created by wbai on 8/24/2022.
//

#include "k32lb11.h"


void TPM1_Init(){
    TPM1_CLOCK_EN();
    SIM->SIM_SOPT2 |= (0x3 << 24);
    TPM1->SC |= (0x1 << 3);
    TPM1->C0SC |= (0xA << 2); // MSB, MSA, ELSB and ELSA should be written to at the same time
    TPM1->C1SC |= (0xA << 2);
    TPM1->MOD = (uint16_t) 0xA0;
//    TPM1->C0V = (uint16_t) 0x50; // this value decides how long the pulse stays high
}


void TPM2_Init(){
    TPM2_CLOCK_EN();
    SIM->SIM_SOPT2 |= (0x3 << 24);
    TPM2->SC |= (0x1 << 3);
    TPM2->C0SC |= (0xA << 2); // MSB, MSA, ELSB and ELSA should be written to at the same time
    TPM2->C1SC |= (0xA << 2);
    TPM2->MOD = (uint16_t) 0xA0;
//    TPM2->C0V = (uint16_t) 0x50; // this value decides how long the pulse stays high
}