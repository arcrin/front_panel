//
// Created by wbai on 5/31/2022.
//

#ifndef FRONT_PANEL_SPI_DRIVER_H
#define FRONT_PANEL_SPI_DRIVER_H

#include <cstdint>
#include "k32lb11.h"



/*
 * @SPI_DeviceMode
 */
#define SPI_DEVICE_SLAVE_MODE       0
#define SPI_DEVICE_MASTER_MODE      1

/*
 * @SPI_BusConfig
 */
#define SPI_BUS_CONFIG_FD   1 // full duplex
#define SPI_BUS_CONFIG_HD   2 // half duplex

/*
 * @SPI_MODE
 */
#define SPI_8BIT_MODE       0
#define SPI_16BIT_MODE      1

/*
 * @SPI_CPOL (clock polarity)
 */
#define SPI_CPOL_LOW        0
#define SPI_CPOL_HIGH       0

/*
 * @SPI_CPHA (clock phase)
 */
#define SPI_CPHA_LOW        0
#define SPI_CPHA_HIGH       1

/*
 * @SPI_SclkSpeed pre-scaler
 */
#define SPI_SCLK_PRE_1       0
#define SPI_SCLK_PRE_2       1
#define SPI_SCLK_PRE_3       2
#define SPI_SCLK_PRE_4       3
#define SPI_SCLK_PRE_5       4
#define SPI_SCLK_PRE_6       5
#define SPI_SCLK_PRE_7       6
#define SPI_SCLK_PRE_8       7

/*
 * @SPI_SclkSpeed (serial clock speed)
 */
#define SPI_SCLK_SPEED_DIV2         0
#define SPI_SCLK_SPEED_DIV4         1
#define SPI_SCLK_SPEED_DIV8         2
#define SPI_SCLK_SPEED_DIV16        3
#define SPI_SCLK_SPEED_DIV32        4
#define SPI_SCLK_SPEED_DIV64        5
#define SPI_SCLK_SPEED_DIV128       6
#define SPI_SCLK_SPEED_DIV256       7
#define SPI_SCLK_SPEED_DIV512       8

/*
 * SPI status register bit map
 */
#define SPI_S_RFIFOEF       0 // SPI read FIFO empty flag
#define SPI_S_TXFULLF       1 // Transmit FIFO full flag
#define SPI_S_TNEAREF       2 // Transmit FIFO nearly empty flag
#define SPI_S_RNFULLF       3 // Receive FIFO nearly full flag
#define SPI_S_MODF          4 // Master mode fault flag
#define SPI_S_SPTEF         5 // SPI transmit buffer empty flag
#define SPI_S_SPRF          6 // SPI match flag
#define SPI_SPRF            7 // SPI read buffer full flag

// SPI flags
#define SPI_TX_EMPTY_FLAG           (1 << SPI_S_SPTEF)
#define SPI_RX_FULL_FLAG            (1 << SPI_S_SPRF)


/*
 * SPI control register 1 bit map
 */
#define SPI_C1_LSBFE        0
#define SPI_C1_SSOE         1
#define SPI_C1_CPHA         2
#define SPI_C1_CPOL         3
#define SPI_C1_MSTR         4
#define SPI_C1_SPTIE        5
#define SPI_C1_SPE          6
#define SPI_C1_SPIE         7

/*
 * SPI control register 2 bit map
 */
#define SPI_C2_SPC0         0
#define SPI_C2_SPISWAI      1
#define SPI_C2_RXDMAE       2
#define SPI_C2_BIDIROE      3
#define SPI_C2_MODFEN       4
#define SPI_C2_TXDMAE       5
#define SPI_C2_SPIMODE      6
#define SPI_C2_SPMIE        7

/*
 * SPI baud rate register bit map
 */
#define SPI_BR_DIV_BITS          0
#define SPI_BR_PRE_BITS          4

/*
 * Configuration structure for SPI
 */
typedef struct {
    uint8_t SPI_DeviceMode;
    uint8_t SPI_BusConfig;
    uint8_t SPI_BR_PRE;
    uint8_t SPI_BR_DIV;
    uint8_t SPI_MODE; // data frame: 8-bit or 16-bit
    uint8_t SPI_CPOL;
    uint8_t SPI_CPHA;
    uint8_t SPI_SSOE;
} SPI_Config_t;

typedef struct {
    pSPI_RegDef_t pSPIx;
    SPI_Config_t SPIConfig;
} SPI_Handle_t, *pSPI_Handle_t;

/*
 * Peripheral Clock Setup
 */
void SPI_PeriClockControl(pSPI_RegDef_t pSPIx, uint8_t ENorDI);

/*
 * Init
 */
void SPI_Init(pSPI_Handle_t pSpiHandle);

/*
 * send and receive data
 */
void SPI_Send_Data(pSPI_RegDef_t sSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_Receive_Data(pSPI_RegDef_t pSPIx, uint8_t *pRxBuffer, uint32_t Len);

/*
 * IRQ configuration and ISR handling
 */
void SPI_IRQITConfig(uint8_t IRQNumber, uint8_t ENorDI);

void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint8_t Priority);

void SPI_IRQHandling(pSPI_Handle_t pSpiHandle);

/*
 * other peripheral APIs
 */

void SPI_PeriControl(pSPI_RegDef_t pSPIx, uint8_t ENorDI);

void SPI_SSIConfig(pSPI_RegDef_t pSPIx, uint8_t ENorDI);

void SPI_SSOEConfig(pSPI_RegDef_t pSPIx, uint8_t ENorDI);

uint8_t SPI_GetFlagStatus(pSPI_RegDef_t pSPIx, uint32_t FlagName);

#endif //FRONT_PANEL_SPI_DRIVER_H
