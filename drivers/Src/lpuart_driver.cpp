//
// Created by andy- on 2022-08-17.
//
#include "k32lb11.h"

void LPUART_SetBaudRate(pLPUART_Handle_t pLPUARTHandle, uint32_t BaudRate){

}

void LPUART_Init(pLPUART_Handle_t pLPUARTHandle){
    if (pLPUARTHandle->pLPUARTx == LPUART0) {
        LPUART0_CLOCK_EN();
    } else if (pLPUARTHandle->pLPUARTx == LPUART1) {
        LPUART1_CLOCK_EN();
    }

    if (pLPUARTHandle->pLPUARTx == LPUART0) {
        SIM->SIM_SOPT2 |= (0x1 << 26); // Need to manually enable the clock for LPUART module
    } else if (pLPUARTHandle->pLPUARTx == LPUART1) {
        SIM->SIM_SOPT2 |= (0x1 << 28); // Need to manually enable the clock for LPUART module
    }


    // Configure the word length
    pLPUARTHandle->pLPUARTx->CTRL |= (pLPUARTHandle->LPUART_Config.LPUART_WordLength << LPUART_CTRL_M);

    // Configure the parity control bit fileds
    if (pLPUARTHandle->LPUART_Config.LPUART_ParityControl == UART_PARITY_EN_EVEN) {
        pLPUARTHandle->pLPUARTx->CTRL |= (1 << LPUART_CTRL_PE);
        pLPUARTHandle->pLPUARTx->CTRL &= ~(1 << LPUART_CTRL_PT);
    } else if (pLPUARTHandle->LPUART_Config.LPUART_ParityControl == UART_PARITY_EN_ODD) {
        pLPUARTHandle->pLPUARTx->CTRL |= (1 << LPUART_CTRL_PE);
        pLPUARTHandle->pLPUARTx->CTRL |= (1 << LPUART_CTRL_PT);
    } else if (pLPUARTHandle->LPUART_Config.LPUART_ParityControl == UART_PARITY_DISABLE) {
        pLPUARTHandle->pLPUARTx->CTRL &= ~(1 << LPUART_CTRL_PE);
    }

    // Configure stop bit
    pLPUARTHandle->pLPUARTx->BAUD |= (pLPUARTHandle->LPUART_Config.UART_NumOfStopBits << LPUART_BAUD_SBNS);

    // Can not configure hardware flow control
    /******************************************************
     * The following is specific for 115200 baud rate
     * Not sure if there is a better way to do this,
     * but I couldn't find any formula
     ******************************************************/
    pLPUARTHandle->pLPUARTx->BAUD |= ((0xD & 0x1FFF) << 0);

    // Configure OSR - Oversampling ratio
    pLPUARTHandle->pLPUARTx->BAUD |= (0x1F << 24);
//    pLPUARTHandle->pLPUARTx->BAUD |= (1 << 28);
//    pLPUARTHandle->pLPUARTx->BAUD &= ~(1 << 27);
//    pLPUARTHandle->pLPUARTx->BAUD &= ~(1 << 26);
//    pLPUARTHandle->pLPUARTx->BAUD &= ~(1 << 25);
//    pLPUARTHandle->pLPUARTx->BAUD &= ~(1 << 24);


    pLPUARTHandle->pLPUARTx->BAUD &= ~(1 <<LPUART_BAUD_M10);

    if (pLPUARTHandle->LPUART_Config.LPUART_Mode == LPUART_TX_ONLY_MODE) {
        pLPUARTHandle->pLPUARTx->CTRL |= (1 << LPUART_CTRL_TE);
    } else if (pLPUARTHandle->LPUART_Config.LPUART_Mode == LPUART_RX_ONLY_MODE) {
        pLPUARTHandle->pLPUARTx->CTRL |= (1 << LPUART_CTRL_RE);
    } else if (pLPUARTHandle->LPUART_Config.LPUART_Mode == LPUART_TXRX_MODE) {
        pLPUARTHandle->pLPUARTx->CTRL |= (1 << LPUART_CTRL_TE);
        pLPUARTHandle->pLPUARTx->CTRL |= (1 << LPUART_CTRL_RE);
    }
}

void LPUART_SendData(pLPUART_Handle_t pLPUARTHandle, uint8_t *pTxBuffer, uint32_t Len){
    for (uint32_t i = 0; i < Len; i++) {
        while (!(pLPUARTHandle->pLPUARTx->STAT & (1 << LPUART_STAT_TDRE)));
        pLPUARTHandle->pLPUARTx->DATA |= (*pTxBuffer & 0xFF);
        pTxBuffer++;
    }
    while(!(pLPUARTHandle->pLPUARTx->STAT & (1 << LPUART_STAT_TC)));
}

void LPUART_SendByte(pLPUART_Handle_t pLPUARTHandle, uint8_t byte_to_send){
    while(!(pLPUARTHandle->pLPUARTx->STAT & (1 << LPUART_STAT_TDRE)));
    pLPUARTHandle->pLPUARTx->DATA |= byte_to_send;
}

void LPUART_IRQHandling(pLPUART_Handle_t pLPUARTHandle){
    uint32_t temp_stat_reading;
    temp_stat_reading = pLPUARTHandle->pLPUARTx->STAT;
//    temp2 = ((pLPUARTHandle->pLPUARTx->CTRL & (1 << LPUART_CTRL_RIE)) >> LPUART_CTRL_RIE);

    /******************Receive******************************/
    if (((temp_stat_reading & (1 << LPUART_STAT_RDRF)) >> LPUART_STAT_RDRF)) {
        pLPUARTHandle->ByteReceived = (uint8_t) (pLPUARTHandle->pLPUARTx->DATA & (uint8_t) 0xFF);
    }
    LPUART_ApplicationEventCallback(pLPUARTHandle, LPUART_BYTE_RCV_EVENT);
}
