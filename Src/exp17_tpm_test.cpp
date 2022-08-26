//
// Created by wbai on 8/25/2022.
//
#include "k32lb11.h"

PORT_Handle_t tpm_port_handle;

void TPM0_PORT_INIT(){
    tpm_port_handle.pPORT = PORTE;
    tpm_port_handle.PORT_Config.PORT_Pin_Number = 20;
    tpm_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION3;
    tpm_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    tpm_port_handle.PORT_Config.PORT_Pin_Pull_Enable = DISABLE;
//    tpm_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_DOWN;
    PORT_Init(&tpm_port_handle);
}

int main() {
    DISABLE_IRQ();
    MCG_Init();
    TPM0_PORT_INIT();
    TPM1_Init();
    ENABLE_IRQ();
    while(1);
}
