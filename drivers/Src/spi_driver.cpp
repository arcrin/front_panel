//
// Created by wbai on 5/31/2022.
//
#include "spi_driver.h"

/********************************************************
 * @fn				- SPI_PeriClockControl
 *
 * @brief			- Initialize or de-initialize peripheral clock for given SPI peripheral
 *
 * @param[pGPIO_RegDef_t]
 *                  - pointer to a SPI peripheral, for stm32f407, SPI1, SPI2, SPI3 are available
 * @param[uint8_t]  - ENABLE or DISABLE
 *
 * @return			- None
 *
 * @Note			- None
 ********************************************************/
void SPI_PeriClockControl(pSPI_RegDef_t pSPIx, uint8_t ENorDI){
    if (ENorDI == ENABLE) {
        if(pSPIx == SPI0){
            SPI0_CLOCK_EN();
        } else if (pSPIx == SPI1) {
            SPI1_CLOCK_EN();
        }
    } else if (ENorDI == DISABLE) {
        if (pSPIx == SPI0) {
            SPI0_CLOCK_DI();
        } else if (pSPIx == SPI1) {
            SPI1_CLOCK_DI();
        }
    }
}
/********************************************************
 * @fn				- SPI_Init
 *
 * @brief			- Initialize or Deinitialize peripheral clock for given SPI peripheral
 *
 * @param[pGPIO_RegDef_t]
 *                  - pointer to a SPI peripheral, for stm32f407, SPI1, SPI2, SPI3 are available
 * @param[uint8_t]  - ENABLE or DISABLE
 *
 * @return			- None
 *
 * @Note			- None
 ********************************************************/
void SPI_Init(pSPI_Handle_t pSPIHandle){
    // enable peripheral clock
    SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);
    // enable 2-pin mode, this should be full duplex mode
    pSPIHandle->pSPIx->C2 &= ~(1 << SPI_C2_SPC0);
    // speed config (baud rate)
    pSPIHandle->pSPIx->BR |= pSPIHandle->SPIConfig.SPI_BR_PRE << SPI_BR_PRE_BITS;
    pSPIHandle->pSPIx->BR |= pSPIHandle->SPIConfig.SPI_BR_DIV << SPI_BR_DIV_BITS;
    // data frame format config
    pSPIHandle->pSPIx->C2 |= pSPIHandle->SPIConfig.SPI_MODE << SPI_C2_SPIMODE;
    // device mode
    pSPIHandle->pSPIx->C1 |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_C1_MSTR;
    // clock polarity
    pSPIHandle->pSPIx->C1 |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_C1_CPOL;
    // clock phase
    if(pSPIHandle->SPIConfig.SPI_CPHA == SPI_CPHA_LOW)
        pSPIHandle->pSPIx->C1 &= ~(1 << SPI_C1_CPHA);
    else if(pSPIHandle->SPIConfig.SPI_CPHA == SPI_CPHA_HIGH)
        pSPIHandle->pSPIx->C1 |= 1 << SPI_C1_CPHA;

    // Following configuration provides software (automatic) slave select control
    // TODO: This should also be a configurable option
    pSPIHandle->pSPIx->C2 |= 1 << SPI_C2_MODFEN; // clear MODFEN pin, NSS pin is set to GPIO
    pSPIHandle->pSPIx->C1 |= 1 << SPI_C1_SSOE;
    // MSB is transmitted first by default
}

uint8_t SPI_GetFlagStatus(pSPI_RegDef_t pSPIx, uint32_t FlagName){
    if (pSPIx->S & FlagName) {
        return FLAG_SET;
    }
    return FLAG_RESET;
}

/********************************************************
 * @fn				- SPI_Send_Data
 *
 * @brief			- blocking api for sending data
 *
 * @param[pSPI_RegDef_t]
 *                  - pointer to SPI peripheral
 * @param[uint8_t]  - pointer to transmit buffer
 * @param[uint23_t] - length of the data to be sent (number of bytes)
 *
 * @return			- None
 *
 * @Note			- blocking call
 ********************************************************/
void SPI_Send_Data(pSPI_RegDef_t pSPIx, uint8_t *pTxBuffer, uint32_t Len){
    while (Len > 0) {
        // 1. wait until transmit flag is set
        while(SPI_GetFlagStatus(pSPIx, SPI_TX_EMPTY_FLAG) == FLAG_RESET); // while the TX buffer is NOT empty
        // 2. check the data frame bit in C2
        if(pSPIx->C2 & (1 << SPI_C2_SPIMODE)){
            // 16-bit mode
            // 1. load the data to the DR(Data Register) register
            pSPIx->DL = *((uint16_t *) pTxBuffer);
            Len--;
            Len--;
            (uint16_t *) pTxBuffer++;
        } else {
            // 8-bit mode
            pSPIx->DL = *pTxBuffer;
            Len--;
            pTxBuffer++;
        }
    }
}

void SPI_Receive_Data(pSPI_RegDef_t pSPIx, uint8_t *pRxBuffer, uint32_t Len){
    while (Len > 0) {
        while(SPI_GetFlagStatus(pSPIx, SPI_RX_FULL_FLAG) == SPI_RX_FULL_FLAG); // while the RX buffer is empty
        if (pSPIx->C2 & (1 << SPI_C2_SPIMODE)) {
            // 16-bit mode
            *(uint16_t *) pRxBuffer = pSPIx->DL;
            Len--;
            Len--;
            (uint16_t *) pRxBuffer++;
        } else {
            *pRxBuffer = pSPIx->DL;
            Len--;
            pRxBuffer++;
        }
    }
}

/*
 * other peripheral APIs
 */
void SPI_PeriControl(SPI_RegDef_t *pSPIx, uint8_t ENorDI){
    if (ENorDI == ENABLE) {
        pSPIx->C1 |= (1 << SPI_C1_SPE);
    } else if (ENorDI == DISABLE) {
        pSPIx->C1 &= ~(1 << SPI_C1_SPE);
    }
}