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
    pSPIHandle->pSPIx->C1 |= pSPIHandle->SPIConfig.SPI_DFF << SPI_C2_SPIMODE;
    // device mode
    pSPIHandle->pSPIx->C1 |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_C1_MSTR;
    // clock polarity
    pSPIHandle->pSPIx->C1 |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_C1_CPOL;
    // clock phase
    pSPIHandle->pSPIx->C1 |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_C1_CPHA;
    // I don't need automated NSS pin, I can use a GPIO pin to select slave
    pSPIHandle->pSPIx->C1 |= pSPIHandle->SPIConfig.SPI_SSOE << SPI_C1_SSOE;
    // MSB is transmitted first by default

}