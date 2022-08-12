//
// Created by wbai on 8/11/2022.
//

#ifndef FRONT_PANEL_UART_DRIVER_H
#define FRONT_PANEL_UART_DRIVER_H
#include "k32lb11.h"

/******************************
 * C1 - Control Register 1
 ******************************/
#define C1_LOOPS        7   // Loop Mode Select
#define C1_RSRC         5   // Receiver Source Select
#define C1_M            4   // 9-bit or 8-bit Mode Select
#define C1_WAKE         3   // Receiver Wakeup Method Select
#define C1_ILT          2   // Idle Like Type Select
#define C1_PE           1   // Parity Enable
#define C1_PT           0   // Parity Type

/*********************************
* C2 - Control Register 2
**********************************/
#define C2_TIE          7   // Transmitte rInterrupt or DMA Enable
#define C2_TCIE         6   // Transmission Complete Interupt Enable
#define C2_RIE          5   // Receiver Full Interrupt Enable
#define C2_ILIE         4   // Idle Line Interrupt Enable
#define C2_TIE          3   // Transmitter Enable
#define C2_RE           2   // Receiver Enable
#define C2_RWU          1   // Receiver Wakeup Control
#define C2_SBK          0   // Send Break

/*********************************
* S1 - Status Register 1
**********************************/
#define S1_TDRE         7   // Transmit Data Register Empty Flag
#define S1_TC           6   // Transmit Complete Flag
#define S1_RDRF         5   // Receive Data Register Full Flag
#define S1_IDLE         4   // Idle Line Flag
#define S1_OR           3   // Receiver Overrun Flag
#define S1_NF           2   // Noise Flag
#define S1_FE           1   // Framing Error Flag
#define S1_PF           0   // Parity Error Flag

/***********************************
 * S2 - Status Register 2
 ***********************************/
#define S2_RXEDGIF      6   // RxD Pin Active Edge Interrupt Flag
#define S2_MSBF         5   // Most Significant Bit First
#define S2_RXINV        4   // Receive Data Inversion
#define S2_RWUID        3   // Receive Wakeup Idle Detect
#define S2_BRK13        2   // Break Transmit Character Length
#define S2_RAF          0   // Receiver Active Flag

/**********************************
 * C3 - Control Register 3
 **********************************/
#define C3_R8           7   // Received Bit 8
#define C3_T8           6   // Transmit Bit 8
#define C3_TXDIR        5   // Transmitter Pin Data Direction in Single-Wire mode
#define C3_TXINV        4   // Transmit Data Inversion
#define C3_ORIE         3   // Overrun Error Interrupt Enable
#define C3_NEIE         2   // Noise Error Interrupt Enable
#define C3_FEIE         1   // Framing Error Interrupt Enable
#define C4_PEIE         0   // Parity Error Interrupt Enable

/**********************************
 * C4 - Control Register 4
 **********************************/
#define C4_MAEN1         7  // Match Address Mode Enable 1
#define C4_MAEN2         6  // Match Address Mode Enable 2
#define C4_M10           5  // 10-bit Mode select
#define C4_BRFA          0  // Baud Rate Fine Adjust

/***********************************
 * C5 - Control Register 5
 ***********************************/
#define C5_TDMAS        7   // Transmitter DMA select
#define C5_RDMAS        5   // Receiver Full DMA Select


#endif //FRONT_PANEL_UART_DRIVER_H
