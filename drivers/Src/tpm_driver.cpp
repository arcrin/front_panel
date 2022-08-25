//
// Created by wbai on 8/24/2022.
//

#include "k32lb11.h"

void TPM_Init(){
    TPM1_CLOCK_EN();
    TPM1->C0SC |= (0x2 << 4);
    TPM1->C0SC |= (0x2 << 2);

}