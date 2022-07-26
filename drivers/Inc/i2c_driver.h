//
// Created by andy- on 2022-07-17.
//

#ifndef FRONT_PANEL_I2C_DRIVER_H
#define FRONT_PANEL_I2C_DRIVER_H

#include "k32lb11.h"

typedef struct {
    uint32_t I2C_SCLSpeed;
    uint8_t I2C_DeviceAddress;
    uint8_t I2C_AckControl;
    uint8_t I2C_FMDutyCycle;
} I2C_Config_t;

typedef struct {
    I2C_RegDef_t *pI2Cx;
    I2C_Config_t I2C_Config;
    uint8_t *pTxBuffer;
    uint8_t *pRxBuffer;
    uint32_t TxLen;
    uint32_t RxLen;
    uint8_t TxRxState;
    uint8_t DevAddr;
    uint32_t RxSize;
    uint8_t Sr;
} I2C_Handle_t;

/*
 * I2C Application State
 */
#define I2C_READY       0
#define I2C_BUSY_IN_RX  1
#define I2C_BUSY_IN_TX  2


/*****************************************
 * Register bit definition
 *****************************************/
// Control Register 1
#define I2C_C1_DMAEN    0 // DMA enable
#define I2C_C1_WUEN     1 // Wakeup enable
#define I2C_C1_RSTA     2 // Repeat Start
#define I2C_C1_TXAK     3 // Transmit Ack Enable
#define I2C_C1_TX       4 // Transmit Mode Select
#define I2C_C1_MST      5 // Master Mode Select
#define I2C_C1_IICIE    6 // I2C Interrupt Enable
#define I2C_C1_IICEN    7 // I2C Enable

// Control Register 2
#define I2C_C2_AD       0 // Slave Address bit [10:8] on 10 bit address mode
#define I2C_C2_RMEN     3 // Range Address Matching Enable
#define I2C_C2_SBRC     4 // Slave Baud Rate Control
#define I2C_C2_HDRS     5 // High Drive Select
#define I2C_C2_ADEXT    6 // Address Extension
#define I2C_C2_GCAEN    7 // General Call Address Enable

// Status Register
#define I2C_S_RXAK      0 // Receive Acknowledge
#define I2C_S_IICIF     1 // Interrupt Flag
#define I2C_S_SRW       2 // Slave Read/Write
#define I2C_S_RAM       3 // Range Address Match
#define I2C_S_ARBL      4 // Arbitration Lost
#define I2C_S_BUSY      5 // Bus Busy
#define I2C_S_IAAS      6 // Addressed As A Slave
#define I2C_S_TCF       7 // Transfer Complete Flag

// Status Register 2
#define I2C_S2_EMPTY    0 // Empty Flag
#define I2C_S2_ERROR    1 // Error Flag

/********************************************
 * Other macros
 ********************************************/
#define I2C_DISABLE_SR  	RESET
#define I2C_ENABLE_SR   	SET
#define I2C_ACK_ENABLE        1
#define I2C_ACK_DISABLE       0

/***************************************
 * Flag definition
 ***************************************/
// Flags in Status Register
#define I2C_FLAG_RXAK   (1 << I2C_S_RXAK)
#define I2C_FLAG_IICIF  (1 << I2C_S_IICIF)
#define I2C_FLAG_SRW    (1 << I2C_S_SRW)
#define I2C_FLAG_RAM    (1 << I2C_S_RAM)
#define I2C_FLAG_ARBL   (1 << I2C_S_ARBL)
#define I2C_FLAG_BUSY   (1 << I2C_S_BUSY)
#define I2C_FLAG_IAAS   (1 << I2C_S_IAAS)
#define I2C_FLAG_TCF    (1 << I2C_S_TCF)

// Flags in Status Register 2
#define I2C_FLAG_EMPTY  (1 << I2C_S2_EMPTY)
#define I2C_FLAG_ERROR  (1 << I2C_S2_ERROR)

/***************************
 * API
 ***************************/
 /*
  * Peripheral Clock Setup
  */
 void I2C_PeripheralClockControl(I2C_RegDef_t *pI2C, uint8_t EnOrDi);

/*
 * Init
 */
void I2C_Init(I2C_Handle_t *pI2CHandle);

/*
 * set and receive data
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t stop_condition, bool after_rs);

void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t stop_condition, bool initial_start_condition);

void I2C_SlaveSendData();

void I2C_SlaveReceiveData();

/*
 * read status
 */
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName, uint8_t SrNumber);

/*
 * ACK management
 */
void I2C_ManageAcking();

/*
 * start and stop conditions
 */
uint8_t I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);

uint8_t I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);

void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);

void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);

#endif //FRONT_PANEL_I2C_DRIVER_H
