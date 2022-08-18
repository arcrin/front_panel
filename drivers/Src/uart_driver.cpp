//
// Created by wbai on 8/11/2022.
//
#include "k32lb11.h"

void UART_SetBaudRate(pUART_Handle_t pUARTHandle, uint32_t BaudRate){
    // Bus clock 8 Mhz TODO: proper clock check in the future
    uint32_t PCLK = 8000000; // Module clock
    uint32_t uartdiv;
    uint32_t M_part, F_part;

    uartdiv = ((25 * PCLK) / (4 * BaudRate));

    M_part = uartdiv / 100;

    pUARTHandle->pUARTx->BDH &= ~(0x1F);
    pUARTHandle->pUARTx->BDH |= ((M_part & 0x1F00) >> 8);

    pUARTHandle->pUARTx->BDL &= ~(0xFF);
    pUARTHandle->pUARTx->BDL |= (M_part & 0xFF);

    F_part = (uartdiv - (M_part * 100));

    F_part = (F_part * 32 + 50) / 100;

    pUARTHandle->pUARTx->C4 |= (F_part & 0x1F);
}

void UART_InterruptControl(pUART_RegDef_t pUARTx, uint8_t EnOrDi){
    if(EnOrDi == ENABLE){
        pUARTx->C2 |= (1 << UART_C2_TIE);
        pUARTx->C2 |= (1 << UART_C2_RIE);
        pUARTx->C2 |= (1 << UART_C2_TCIE);
    } else if(EnOrDi == DISABLE){
        pUARTx->C2 &= ~(1 << UART_C2_TIE);
        pUARTx->C2 &= ~(1 << UART_C2_RIE);
        pUARTx->C2 &= ~(1 << UART_C2_TCIE);
    }
}

void UART_Init(pUART_Handle_t pUARTHandle){
    UART2_CLOCK_EN();
    // ENable UART Tx and Rx engines according to the UART_Mode configuration item
    if (pUARTHandle->UART_Config.UART_Mode == UART_TX_ONLY_MODE) {
        pUARTHandle->pUARTx->C2 |= (1 << UART_C2_TE);
    } else if (pUARTHandle->UART_Config.UART_Mode == UART_RX_ONLY_MODE) {
        pUARTHandle->pUARTx->C2 |= (1 << UART_C2_RE);
    } else if (pUARTHandle->UART_Config.UART_Mode == UART_TXRX_MODE) {
        pUARTHandle->pUARTx->C2 |= ((1 << UART_C2_TE) | (1 << UART_C2_RE));
    }
    // Configure the word length
    pUARTHandle->pUARTx->C1 |= (pUARTHandle->UART_Config.UART_WordLength << UART_C1_M);

    // Configure the parity control bit fields
    if (pUARTHandle->UART_Config.UART_ParityControl == UART_PARITY_EN_EVEN) {
        pUARTHandle->pUARTx->C1 |= (1 << UART_C1_PE);
        pUARTHandle->pUARTx->C1 &= ~(1 << UART_C1_PT);
    } else if (pUARTHandle->UART_Config.UART_ParityControl == UART_PARITY_EN_ODD) {
        pUARTHandle->pUARTx->C1 |= (1 << UART_C1_PE);
        pUARTHandle->pUARTx->C1 |= (1 << UART_C1_PT);
    } else if (pUARTHandle->UART_Config.UART_ParityControl == UART_PARITY_DISABLE) {
        pUARTHandle->pUARTx->C1 &= ~(1 << UART_C1_PE);
    }
    // stop bit is default as 1 bit and there isn't a way to configure it
    // can not configure hardware flow control (CTS, RTS)
    // Configure baud rate
    UART_SetBaudRate(pUARTHandle, pUARTHandle->UART_Config.UART_Baud);
}

void UART_SendData(pUART_Handle_t pUARTHandle, uint8_t *pTxBuffer, uint32_t Len){
    // only handling 8 bit data format
    for (uint32_t i = 0; i < Len; i++) {
        while(!(pUARTHandle->pUARTx->S1 & (1 << UART_S1_TDRE)));
        pUARTHandle->pUARTx->D = *pTxBuffer;
        pTxBuffer++;
    }
    while(!(pUARTHandle->pUARTx->S1 & (1 << UART_S1_TC)));
}

void UART_SendByte(pUART_Handle_t pUARTHandle, uint8_t byte_to_send){
    while(!(pUARTHandle->pUARTx->S1 & (1 << UART_S1_TDRE)));
    pUARTHandle->pUARTx->D = byte_to_send;
}

void UART_IRQHandling(pUART_Handle_t pUARTHandle){
    uint32_t temp_s1_reading, temp2;
    uint8_t byte_received;

    /*****************************Receive****************************/
    temp_s1_reading = pUARTHandle->pUARTx->S1;
    temp2 = (pUARTHandle->pUARTx->C2 & (1 << UART_C2_RIE)) >> UART_C2_RIE;

    if (((temp_s1_reading & (1 << UART_S1_RDRF)) >> UART_S1_RDRF) && temp2) {
        if (pUARTHandle->UART_Config.UART_ParityControl == UART_PARITY_DISABLE) {
            pUARTHandle->ByteReceived = (uint8_t) (pUARTHandle->pUARTx->D & (uint8_t) 0xFF);
        } else {
            pUARTHandle->ByteReceived = (uint8_t) (pUARTHandle->pUARTx->D & (uint8_t) 0x7F);
        }
        UART_ApplicationEventCallback(pUARTHandle, UART_BYTE_RCV_EVENT);
    }
}


