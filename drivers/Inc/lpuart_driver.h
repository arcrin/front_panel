//
// Created by andy- on 2022-08-17.
//

#ifndef FRONT_PANEL_LPUART_DRIVER_H
#define FRONT_PANEL_LPUART_DRIVER_H
#include "k32lb11.h"

/************************************
 * BAUD - Baud Rate Register
 ************************************/
#define LPUART_BAUD_MAEN1       31
#define LPUART_BAUD_MAEN2       30
#define LPUART_BAUD_M10         29
#define LPUART_BAUD_OSR         24
#define LPUART_BAUD_TDMAE       23
#define LPUART_BAUD_RDMAE       21
#define LPUART_BAUD_MATCFG      18
#define LPUART_BAUD_BOTHEDGE    17
#define LPUART_BAUD_RESYNCDIS   16
#define LPUART_BAUD_LBKDIE      15
#define LPUART_BAUD_RXEDGIE     14
#define LPUART_BAUD_SBNS        13
#define LPUART_BAUD_SBR         0

/**********************************
 * STAT - Status Register
 **********************************/
#define LPUART_STAT_LBKDIF      31
#define LPUART_STAT_RXEDGIF     30
#define LPUART_STAT_MSBF        29
#define LPUART_STAT_RXINV       28
#define LPUART_STAT_RWUID       27
#define LPUART_STAT_BRK13       26
#define LPUART_STAT_LBKDE       25
#define LPUART_STAT_RAF         24
#define LPUART_STAT_TDRE        23
#define LPUART_STAT_TC          22
#define LPUART_STAT_RDRF        21
#define LPUART_STAT_IDLE        20
#define LPUART_STAT_OR          19
#define LPUART_STAT_NF          18
#define LPUART_STAT_FE          17
#define LPUART_STAT_PF          16
#define LPUART_STAT_MA1F        15
#define LPUART_STAT_MA2F        14

/********************************
 * CTRL - Control Register
 ********************************/
#define LPUART_CTRL_R8T9        31
#define LPUART_CTRL_R9T8        30
#define LPUART_CTRL_TXDIR       29
#define LPUART_CTRL_TXINV       28
#define LPUART_CTRL_ORIE        27
#define LPUART_CTRL_NEIE        26
#define LPUART_CTRL_FEIE        25
#define LPUART_CTRL_PEIE        24
#define LPUART_CTRL_TIE         23
#define LPUART_CTRL_TCIE        22
#define LPUART_CTRL_RIE         21
#define LPUART_CTRL_ILIE        20
#define LPUART_CTRL_TE          19
#define LPUART_CTRL_RE          18
#define LPUART_CTRL_RWU         17
#define LPUART_CTRL_SBK         16
#define LPUART_CTRL_MA1IE       15
#define LPUART_CTRL_MA2IE       14
#define LPUART_CTRL_IDLECFG     8
#define LPUART_CTRL_LOOPS       7
#define LPUART_CTRL_DOZEEN      6
#define LPUART_CTRL_RSR         5
#define LPUART_CTRL_M           4
#define LPUART_CTRL_WAKE        3
#define LPUART_CTRL_ILT         2
#define LPUART_CTRL_PE          1
#define LPUART_CTRL_PT          0


typedef struct {
    _vo uint8_t LPUART_Mode;
    _vo uint32_t LPUART_Baud;
    _vo uint8_t LPUART_WordLength;
    _vo uint8_t LPUART_ParityControl;
    _vo uint8_t UART_NumOfStopBits;
//    _vo uint8_t UART_HWFlowControl;
} LPUART_Config_t;

typedef struct {
    pLPUART_RegDef_t pLPUARTx;
    LPUART_Config_t LPUART_Config;
    uint8_t *pTxBuffer;
    uint8_t *pRxBuffer;
    uint32_t TxLen;
    uint32_t RxLen;
    uint8_t TxBusyState;
    uint8_t RxBusyState;
    uint8_t ByteReceived;
} LPUART_Handle_t, *pLPUART_Handle_t;

#define LPUART_TX_ONLY_MODE       1
#define LPUART_RX_ONLY_MODE       2
#define LPUART_TXRX_MODE          3

#define LPUART_WORDLEN_8BITS      0
#define LPUART_WORDLEN_9BITS      1

#define LPUART_PARITY_EN_ODD      2
#define LPUART_PARITY_EN_EVEN     1
#define LPUART_PARITY_DISABLE     0

#define LPUART_STOP_1BIT          0
#define LPUART_STOP_2BIT          1


/****************************
 * Application Event
 ****************************/
#define LPUART_BYTE_RCV_EVENT     0


/**************************
 * API
 *************************/
void LPUART_Init(pLPUART_Handle_t pLPUARTHandle);

void LPUART_SendData(pLPUART_Handle_t pLPUARTHandle, uint8_t *pTxBuffer, uint32_t Len);

void LPUART_SendByte(pLPUART_Handle_t pLPUARTHandle, uint8_t byte_to_send);

void LPUART_IRQHandling(pLPUART_Handle_t pLPUARTHandle);

void LPUART_ApplicationEventCallback(pLPUART_Handle_t pLPUARTHandle, uint8_t app_event)__attribute__((weak));

#endif //FRONT_PANEL_LPUART_DRIVER_H
