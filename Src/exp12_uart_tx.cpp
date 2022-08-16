//
// Created by andy- on 2022-08-15.
//
#include "k32lb11.h"
UART_Handle_t uart_handle;

int main(){
    uart_handle.pUARTx = UART2;
    uart_handle.UART_Config.UART_Mode = UART_TX_ONLY_MODE;
    uart_handle.UART_Config.UART_WordLength = UART_WORDLEN_8BITS;
    uart_handle.UART_Config.UART_ParityControl = UART_PARITY_DISABLE;
    UART_Init(&uart_handle);
    while(1);
}