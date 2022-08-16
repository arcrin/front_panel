//
// Created by wbai on 8/11/2022.
//
#include "k32lb11.h"

void UART_SetBaudRate(pUART_Handle_t pUARTHandle, uint32_t BaudRate){
    // Bus clock 8 Mhz TODO: proper clock check in the future
    uint32_t PCLK = 8000000; // Module clock
    uint32_t tempreg = 0;
    uint32_t uartdiv;
    uint32_t M_part, F_part;

    uartdiv = ((25 * PCLK) / (4 * BaudRate));

    M_part = uartdiv / 100;

    pUARTHandle->pUARTx->BDH |= ((M_part & 0x1F00) >> 8);

    pUARTHandle->pUARTx->BDL |= (M_part & 0xFF);

    F_part = (uartdiv - (M_part * 100));

    F_part = (F_part * 32 + 50) / 100;

    pUARTHandle->pUARTx->C4 |= (F_part & 0x1F);
}

void UART_Init(pUART_Handle_t pUARTHandle){
    UART2_CLOCK_EN();
    // ENable UART Tx and Rx engines according to the UART_Mode configuration item
    if (pUARTHandle->UART_Config.UART_Mode == UART_TX_ONLY_MODE) {
        pUARTHandle->pUARTx->C2 |= (1 << C2_TE);
    } else if (pUARTHandle->UART_Config.UART_Mode == UART_RX_ONLY_MODE) {
        pUARTHandle->pUARTx->C2 |= (1 << C2_RE);
    } else if (pUARTHandle->UART_Config.UART_Mode == UART_TXRX_MODE) {
        pUARTHandle->pUARTx->C2 |= ((1 << C2_TE) | (1 << C2_RE));
    }
    // Configure the word length
    pUARTHandle->pUARTx->C1 |= (pUARTHandle->UART_Config.UART_WordLength << C1_M);

    // Configure the parity control bit fields
    if (pUARTHandle->UART_Config.UART_ParityControl == UART_PARITY_EN_EVEN) {
        pUARTHandle->pUARTx->C1 |= (1 << C1_PE);
        pUARTHandle->pUARTx->C1 &= ~(1 << C1_PT);
    } else if (pUARTHandle->UART_Config.UART_ParityControl == UART_PARITY_EN_ODD) {
        pUARTHandle->pUARTx->C1 |= (1 << C1_PE);
        pUARTHandle->pUARTx->C1 |= (1 << C1_PT);
    } else if (pUARTHandle->UART_Config.UART_ParityControl == UART_PARITY_DISABLE) {
        pUARTHandle->pUARTx->C1 &= ~(1 << C1_PE);
    }
    // stop bit is default as 1 bit and there isn't a way to configure it
    // can not configure hardware flow control (CTS, RTS)
    // Configure baud rate
    UART_SetBaudRate(pUARTHandle, 9600);
}