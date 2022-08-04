//
// Created by andy- on 2022-07-17.
//
#include "k32lb11.h"
#include <cstring>

uint32_t I2C_START_COUNT = 0;

uint8_t I2C_GenerateStartCondition(I2C_RegDef_t *pI2C){
    if (pI2C->C1 & (1 << 5)){ // turn on MST bit only if it is 0
        return false;
    }
    pI2C->C1 |= (1 << 5);
    return true;
}

void I2C_RepeatedStart(I2C_RegDef_t *pI2C){
    pI2C->C1 |= 1 << I2C_C1_RSTA;
}

void I2C_GetStartDetectionFlagThenClear(I2C_RegDef_t *pI2Cx){
    while(!(pI2Cx->FLT & (1 << I2C_FLT_STARTF)));
    pI2Cx->FLT |= (1 << I2C_FLT_STARTF);
}

uint8_t I2C_GenerateStopCondition(I2C_RegDef_t *pI2C){
    if (pI2C->C1 & (1 << 5)){ // turn off MST bit only if it is 1
        pI2C->C1 &= ~(1 << 5);
        return true;
    }
    return false;
}

void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr){
    SlaveAddr = SlaveAddr << 1;
    SlaveAddr &= ~(1);
    pI2Cx->D = SlaveAddr;
}

void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr){
    SlaveAddr = SlaveAddr << 1;
    SlaveAddr |= 1; // set R/nW bit to 1, the 8th bit
    pI2Cx->D = SlaveAddr;
}

void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi){
    if (EnOrDi == I2C_ACK_DISABLE) {
        // enable the ack
        pI2Cx->C1 |= 1 << I2C_C1_TXAK;
    } else if (EnOrDi == I2C_ACK_ENABLE) {
        // disable the ack
        pI2Cx->C1 &= ~(1 << I2C_C1_TXAK);
    }
}

void I2C_PeripheralClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi){
    if (EnOrDi == ENABLE) {
        if (pI2Cx == I2C0) {
            I2C0_CLOCK_EN();
        } else if (pI2Cx == I2C1) {
            I2C1_CLOCK_EN();
        }
    } else if (EnOrDi == DISABLE) {
        if (pI2Cx == I2C0) {
            I2C0_CLOCK_DI();
        } else if (pI2Cx == I2C1) {
            I2C1_CLOCK_DI();
        }
    }
}

void I2C_Init(I2C_Handle_t *pI2CHandle){
    uint32_t tempreg = 0;
    // enable clock gate
    I2C_PeripheralClockControl(pI2CHandle->pI2Cx, ENABLE);
    // ack control?

    // Write to frequency divider register to set the I2C baud rate (100 kHz)
    pI2CHandle->pI2Cx->F |= (0x2 << 6); // MULT bits
    pI2CHandle->pI2Cx->F &= ~(0x3f << 0); // ICR bits

    // program device's own address
    tempreg = 0;
    tempreg |= pI2CHandle->I2C_Config.I2C_DeviceAddress << 1;
    pI2CHandle->pI2Cx->A1 = tempreg;
    // Write to Control Register 1 to enable I2C module and interrupts
    // Initialize RAM variables for transmit data (Tx buffer)
    // Initialize RAM variables for routine
    // Write to Control Register 1 to enable TX
    // Write to Control Register 1 to enable MST (master mode)
    // Write to Data Register with the address of the target slave
}

void I2C_InterruptControl(I2C_RegDef_t *pI2C, uint8_t  EnOrDi){
    if (EnOrDi == ENABLE)
        pI2C->C1 |= (1 << I2C_C1_IICIE);
    else
        pI2C->C1 &= ~(1 << I2C_C1_IICIE);
}

void I2C_StartStopDetectionControl(I2C_RegDef_t *pI2C, uint8_t EnOrDi){
    if (EnOrDi == ENABLE)
        pI2C->FLT |= (1 << I2C_FLT_SSIE);
    else
        pI2C->FLT &= ~(1 << I2C_FLT_SSIE);
}

void I2C_PeripheralControl(I2C_RegDef_t *pI2C, uint8_t EnOrDi){
    if(EnOrDi == ENABLE)
        pI2C->C1 |= (1 << I2C_C1_IICEN);
    else
        pI2C->C1 &= ~(1 << I2C_C1_IICEN);
}


uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName, uint8_t SrNumber){
    if (SrNumber == 1){
        if (pI2Cx->S & FlagName) {
            return FLAG_SET;
        }
        return FLAG_RESET;
    } else if (SrNumber == 2){
        if (pI2Cx->S2 & FlagName){
            return FLAG_SET;
        }
        return FLAG_RESET;
    } else {
        return FLAG_RESET;
    }
}

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle,
                        uint8_t *pTxBuffer,
                        uint32_t Len,
                        uint8_t SlaveAddr,
                        uint8_t stop_condition,
                        bool initial_start_condition){
    // Transmit mode
    pI2CHandle->pI2Cx->C1 |= 1 << I2C_C1_TX;
    // Start condition
    if (initial_start_condition){
        I2C_GenerateStartCondition(pI2CHandle->pI2Cx);
    } else {
        I2C_RepeatedStart(pI2CHandle->pI2Cx);
    }
    I2C_GetStartDetectionFlagThenClear(pI2CHandle->pI2Cx);
    I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, SlaveAddr);
    while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TCF, 1));
    while(Len > 0){
        pI2CHandle->pI2Cx->D = *pTxBuffer;
        while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TCF, 1));
        pTxBuffer++;
        Len--;
    }
    if (stop_condition){
        I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
    }
}

void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t stop_condition, bool initial_start_condition){
    // stay in transmit mode until AddressRead is sent out?
    pI2CHandle->pI2Cx->C1 |= (1 << I2C_C1_TX);
    // Start condition
    if(initial_start_condition){
        I2C_GenerateStartCondition(pI2CHandle->pI2Cx);
    } else {
        I2C_RepeatedStart(pI2CHandle->pI2Cx);
    }
    I2C_GetStartDetectionFlagThenClear(pI2CHandle->pI2Cx);
    I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, SlaveAddr);
    while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TCF, 1));
    I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_ENABLE);

    if(Len == 1){
        I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);
        pI2CHandle->pI2Cx->C1 &= ~(1 << I2C_C1_TX); // as soon as receive mode is enabled, the slave will send a byte
        *pRxBuffer = pI2CHandle->pI2Cx->D; // note: the first read will cause the data to be transmitted on the i2c bus,
                                           //       hence I can see it on the analyzer; however, that data is not stored in the RxBuffer,
                                           //       it is still in the data register. I need to do another read to get it.
        while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TCF, 1));
        if (stop_condition) {
            I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
        }
        *pRxBuffer = pI2CHandle->pI2Cx->D; // note: this read happens after stop condition, so it won't show up on the bus
        while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TCF, 1));
    }

    if(Len > 1){
        pI2CHandle->pI2Cx->C1 &= ~(1 << I2C_C1_TX); // as soon as receive mode is enabled, the slave will send a byte
        for (uint32_t i = Len + 1; i > 0; i--) {
            if (i == 2) {
                I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);
            }
            if (i == 1) {
                if (stop_condition) {
                    I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
                }
            }
            *pRxBuffer = pI2CHandle->pI2Cx->D;
            while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TCF, 1));
            pRxBuffer++;
        }
    }
}

void I2C_IRQHandling(I2C_Handle_t *pI2CHandle){
    I2C_RegDef_t *i2c = pI2CHandle->pI2Cx;
    uint8_t interrupt_flag = (i2c->S & (1 << I2C_S_IICIF)) >> I2C_S_IICIF;
    if(!interrupt_flag)
        return;

    //check STOPF flag in Glitch Filter Register
    uint8_t stop_flag = (i2c->FLT & (1 << I2C_FLT_STOPF)) >> I2C_FLT_STOPF;
    if(stop_flag){
        i2c->S |= (1 << I2C_S_IICIF);
        i2c->FLT |= (1 << I2C_FLT_STOPF);
        I2C_START_COUNT = 0;
        return;
    } else {
        //check STARTF flag in Glitch Filter Register
        uint8_t start_flag = (i2c->FLT & (1 << I2C_FLT_STARTF)) >> I2C_FLT_STARTF;
        if (start_flag) {
            i2c->FLT |= (1 << I2C_FLT_STARTF);
            i2c->S |= (1 << I2C_S_IICIF);
            I2C_START_COUNT++;
            if (I2C_START_COUNT == 1) {
                //not a repeated start
                if ((i2c->C1 & (1 << I2C_C1_MST)) >> I2C_C1_MST) {
                    // master mode
                    if ((i2c->C1 & (1 << I2C_C1_TX)) >> I2C_C1_TX) {
                        // transmit mode
                        if (pI2CHandle->TxLen == 0) {
                            // last byte transmitted
                            I2C_GenerateStopCondition(i2c);
                        } else {
                            //
                        }
                    } else {
                        // receive mode
                    }
                }
            } else {
                // repeated start
            }
        } else{
            // start condition not detected
        }
    }
}
