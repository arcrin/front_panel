//
// Created by wbai on 4/27/2022.
//

#ifndef FRONT_PANEL_K32LB3X_H
#define FRONT_PANEL_K32LB3X_H

#include <cstdint>
#include <cstring>

#define _vo     volatile

#define FLASH_BASEADDR      0x00000000
#define SRAM_LOW            0x1FFFE000
#define SRAM_UPPER          0x20000000


/*
 * NVIC registers
 */
#define NVIC_ISER           (_vo uint32_t*) 0xE000E100
#define NVIC_ICER           (_vo uint32_t*) 0xE000E180
#define NVIC_ISPR           (_vo uint32_t*) 0xE000E200
#define NVIC_ICPR           (_vo uint32_t*) 0xE000E280
#define NVIC_IPR0           (_vo uint32_t*) 0xE000E400
#define NVIC_IPR1           (_vo uint32_t*) 0xE000E404
#define NVIC_IPR2           (_vo uint32_t*) 0xE000E408
#define NVIC_IPR3           (_vo uint32_t*) 0xE000E40C
#define NVIC_IPR4           (_vo uint32_t*) 0xE000E401
#define NVIC_IPR5           (_vo uint32_t*) 0xE000E414
#define NVIC_IPR6           (_vo uint32_t*) 0xE000E418
#define NVIC_IPR7           (_vo uint32_t*) 0xE000E41C

#define NVIC_IPR_BASEADDR   NVIC_IPR0

#define NVIC_PRIORITY_BITS_IMPLEMENTED  2


/**********************************************
 * Port Pin Control Registers peripheral def
 **********************************************/
#define PORTA_BASEADDR      0x40049000
#define PORTB_BASEADDR      0x4004A000
#define PORTC_BASEADDR      0x4004B000
#define PORTD_BASEADDR      0x4004C000
#define PORTE_BASEADDR      0x4004D000

typedef struct {
    _vo uint32_t PORT_PCR[32];
    _vo uint32_t PORT_GPCLR;
    _vo uint32_t PORT_GPCHR;
} PORT_RegDef_t, *pPORT_RegDef_t;

void InterruptConfig(uint8_t IRQNumber, uint8_t ENorDI);
void IRQPriorityConfig(uint8_t IRQNumber, uint32_t priority);

/*****************************************************
 * System Integration Module Registers peripheral def
 *****************************************************/
#define SIM_BASEADDR        0x40047000

typedef struct {
    _vo uint32_t SIM_SOPT1;
    _vo uint32_t SIM_SOPT1CFG;
    uint32_t RESERVED0[1023];
    _vo uint32_t SIM_SOPT2;
    uint32_t RESERVED1;
    _vo uint32_t SIM_SOPT4;
    _vo uint32_t SIM_SOPT5;
    uint32_t RESERVED2;
    _vo uint32_t SIM_SOPT7;
    uint32_t RESERVED3[2];
    _vo uint32_t SIM_SDID;
    uint32_t RESERVED4[3];
    _vo uint32_t SIM_SCGC4;
    _vo uint32_t SIM_SCGC5;
    _vo uint32_t SIM_SCGC6;
    _vo uint32_t SIM_SCGC7;
    _vo uint32_t SIM_CLKDIV1;
    uint32_t RESERVED5;
    _vo uint32_t SIM_FCFG1;
    _vo uint32_t SIM_FCFG2;
    uint32_t RESERVED6;
    _vo uint32_t SIM_UIDMH;
    _vo uint32_t SIM_UIDML;
    _vo uint32_t SIM_UIDL;
    uint32_t RESERVED7[39];
    _vo uint32_t SIM_COPC;
    _vo uint32_t SIM_SRVCOP;
}SIM_RegDef_t, *pSIM_RegDef_t;


/************************************************
 * GPIO peripheral def
 ************************************************/
#define GPIOA_BASEADDR      0x400FF000
#define GPIOB_BASEADDR      0x400FF040
#define GPIOC_BASEADDR      0x400FF080
#define GPIOD_BASEADDR      0x400FF0C0
#define GPIOE_BASEADDR      0x400FF100

typedef struct {
    _vo uint32_t PDOR;
    _vo uint32_t PSOR;
    _vo uint32_t PCOR;
    _vo uint32_t PTOR;
    _vo uint32_t PDIR;
    _vo uint32_t PDDR;
} GPIO_RegDef_t, *pGPIO_RegDef_t;

#define SIM     ((pSIM_RegDef_t) SIM_BASEADDR)

#define PORTA    ((pPORT_RegDef_t) PORTA_BASEADDR)
#define PORTB    ((pPORT_RegDef_t) PORTB_BASEADDR)
#define PORTC    ((pPORT_RegDef_t) PORTC_BASEADDR)
#define PORTD    ((pPORT_RegDef_t) PORTD_BASEADDR)
#define PORTE    ((pPORT_RegDef_t) PORTE_BASEADDR)

#define GPIOA   ((pGPIO_RegDef_t) GPIOA_BASEADDR)
#define GPIOB   ((pGPIO_RegDef_t) GPIOB_BASEADDR)
#define GPIOC   ((pGPIO_RegDef_t) GPIOC_BASEADDR)
#define GPIOD   ((pGPIO_RegDef_t) GPIOD_BASEADDR)
#define GPIOE   ((pGPIO_RegDef_t) GPIOE_BASEADDR)


// Clock controls
#define PORTA_CLOCK_EN()    SIM->SIM_SCGC5 |= 1 << 9
#define PORTB_CLOCK_EN()    SIM->SIM_SCGC5 |= 1 << 10
#define PORTC_CLOCK_EN()    SIM->SIM_SCGC5 |= 1 << 11
#define PORTD_CLOCK_EN()    SIM->SIM_SCGC5 |= 1 << 12
#define PORTE_CLOCK_EN()    SIM->SIM_SCGC5 |= 1 << 13

#define PORTA_CLOCK_DI()    SIM->SIM_SCGC5 &= ~(1 << 9)
#define PORTB_CLOCK_DI()    SIM->SIM_SCGC5 &= ~(1 << 10)
#define PORTC_CLOCK_DI()    SIM->SIM_SCGC5 &= ~(1 << 11)
#define PORTD_CLOCK_DI()    SIM->SIM_SCGC5 &= ~(1 << 12)
#define PORTE_CLOCK_DI()    SIM->SIM_SCGC5 &= ~(1 << 13)


/*********************************************
 * SPI peripheral def
 *********************************************/
#define SPI0_BASEADDR   0x40076000
#define SPI1_BASEADDR   0x40077000

typedef struct {
    _vo uint8_t S; // status register
    _vo uint8_t BR; // baud rate
    _vo uint8_t C2; // control register 2
    _vo uint8_t C1; // control register 1
    _vo uint8_t ML; // match register low
    _vo uint8_t MH; // match register high
    _vo uint8_t DL; // data register low
    _vo uint8_t DH; // data register high
    _vo uint8_t CI; // clear interrupt
    _vo uint8_t C3; // control register 3
} SPI_RegDef_t, *pSPI_RegDef_t;

#define SPI0    ((pSPI_RegDef_t) SPI0_BASEADDR)
#define SPI1    ((pSPI_RegDef_t) SPI1_BASEADDR)

#define SPI0_CLOCK_EN()     SIM->SIM_SCGC4 |= 1 << 22
#define SPI1_CLOCK_EN()     SIM->SIM_SCGC4 |= 1 << 23

#define SPI0_CLOCK_DI()     SIM->SIM_SCGC4 &= ~(1 << 22)
#define SPI1_CLOCK_DI()     SIM->SIM_SCGC4 &= ~(1 << 23)


/**************************************************
 * I2C peripheral def
 **************************************************/
#define I2C0_BASEADDR 0x40066000
#define I2C1_BASEADDR 0x40067000

typedef struct {
    _vo uint8_t A1;
    _vo uint8_t F;
    _vo uint8_t C1;
    _vo uint8_t S;
    _vo uint8_t D;
    _vo uint8_t C2;
    _vo uint8_t FLT;
    _vo uint8_t RA;
    _vo uint8_t SMB;
    _vo uint8_t A2;
    _vo uint8_t SLTH;
    _vo uint8_t SLTL;
    _vo uint8_t S2;
} I2C_RegDef_t, *pI2C_RegDef_t;

#define I2C0    ((pI2C_RegDef_t) I2C0_BASEADDR)
#define I2C1    ((pI2C_RegDef_t) I2C1_BASEADDR)

#define I2C0_CLOCK_EN() SIM->SIM_SCGC4 |= (1 << 6)
#define I2C1_CLOCK_EN() SIM->SIM_SCGC4 |= (1 << 7)

#define I2C0_CLOCK_DI() SIM->SIM_SCGC4 &= ~(1 << 6)
#define I2C1_CLOCK_DI() SIM->SIM_SCGC4 &= ~(1 << 7)

/*******************************************
 * ADC peripheral def
 *******************************************/
#define ADC0_BASEADDR   0x4003B000

typedef struct {
    _vo uint32_t SC1A;
    _vo uint32_t SC1B;
    _vo uint32_t CFG1;
    _vo uint32_t CFG2;
    _vo uint32_t RA;
    _vo uint32_t RB;
    _vo uint32_t CV1;
    _vo uint32_t CV2;
    _vo uint32_t SC2;
    _vo uint32_t SC3;
    _vo uint32_t OFS;
    _vo uint32_t PG;
    _vo uint32_t MG;
    _vo uint32_t CLPD;
    _vo uint32_t CLPS;
    _vo uint32_t CLP4;
    _vo uint32_t CLP3;
    _vo uint32_t CLP2;
    _vo uint32_t CLP1;
    _vo uint32_t CLP0;
    _vo uint32_t CLMD;
    _vo uint32_t CLMS;
    _vo uint32_t CLM4;
    _vo uint32_t CLM3;
    _vo uint32_t CLM2;
    _vo uint32_t CLM1;
    _vo uint32_t CLM0;
}ADC_RegDef_t, *pADC_RegDef_t;

#define ADC0                ((pADC_RegDef_t) ADC0_BASEADDR)
#define ADC0_CLOCK_EN()     SIM->SIM_SCGC6 |= (1 << 27)
#define ADC0_CLOCK_DI()     SIM->SIM_SCGC6 &= ~(1 << 27)



/***********************************************
 * UART peripheral def
 ***********************************************/
#define UART2_BASEADDR      0x4006C000
typedef struct {
    _vo uint8_t BDH;
    _vo uint8_t BDL;
    _vo uint8_t C1;
    _vo uint8_t C2;
    _vo uint8_t S1;
    _vo uint8_t S2;
    _vo uint8_t C3;
    _vo uint8_t D;
    _vo uint8_t MA1;
    _vo uint8_t MA2;
    _vo uint8_t C4;
    _vo uint8_t C5;
}UART_RegDef_t, *pUART_RegDef_t;

#define UART2   ((pUART_RegDef_t) UART2_BASEADDR)

#define UART2_CLOCK_EN()    SIM->SIM_SCGC4 |= (1 << 12)
#define UART2_CLOCK_DI()    SIM->SIM_SCGC4 &= ~(1 << 12)

/******************************************
 * LPUART peripheral def
 ******************************************/
#define LPUART0_BASEADDR        0x40054000
#define LPUART1_BASEADDR        0x40055000

typedef struct {
    _vo uint32_t BAUD;
    _vo uint32_t STAT;
    _vo uint32_t CTRL;
    _vo uint32_t DATA;
} LPUART_RegDef_t, *pLPUART_RegDef_t;

#define LPUART0     ((pLPUART_RegDef_t) LPUART0_BASEADDR)
#define LPUART1     ((pLPUART_RegDef_t) LPUART1_BASEADDR)

#define LPUART0_CLOCK_EN()      SIM->SIM_SCGC5 |= (1 << 20)
#define LPUART1_CLOCK_EN()      SIM->SIM_SCGC5 |= (1 << 21)

#define LPUART0_CLOCK_DI()      SIM->SIM_SCGC5 &= ~(1 << 20)
#define LPUART1_CLOCK_DI()      SIM->SIM_SCGC5 &= ~(1 << 21)

/***********************************************
 * LPTMR peripheral def
 ***********************************************/
#define LPTMR_BASEADDR      0x40040000

typedef struct {
    _vo uint32_t CSR;
    _vo uint32_t PSR;
    _vo uint32_t CMR;
    _vo uint32_t CNR;
} LPTMR_RegDef_t, *pLPTMR_RegDef_t;

#define LPTMR   ((pLPTMR_RegDef_t) LPTMR_BASEADDR)

#define LPTMR_CLOCK_EN()        SIM->SIM_SCGC5 |= (1 << 0)
#define LPTMR_CLOCK_DI()        SIM->SIM_SCGC5 &= ~(1 << 0)

/***************************************
 * TPM peripheral def
 ***************************************/
#define TPM0_BASEADDR       0x40038000
#define TPM1_BASEADDR       0x40039000
#define TPM2_BASEADDR       0x4003A000

typedef struct {
    _vo uint32_t SC;
    _vo uint32_t CNT;
    _vo uint32_t MOD;
    _vo uint32_t C0SC;
    _vo uint32_t C0V;
    _vo uint32_t C1SC;
    _vo uint32_t C1V;
    _vo uint32_t C2SC;
    _vo uint32_t C2V;
    _vo uint32_t C3SC;
    _vo uint32_t C3V;
    _vo uint32_t C4SC;
    _vo uint32_t C4V;
    _vo uint32_t C5SC;
    _vo uint32_t C5V;
    _vo uint32_t STATUS;
    _vo uint32_t POL;
    _vo uint32_t CONF;
} TPM_Reg_Def_t, *pTPM_Reg_Def_t;

#define TPM0    ((pTPM_Reg_Def_t) TPM0_BASEADDR)
#define TPM1    ((pTPM_Reg_Def_t) TPM1_BASEADDR)
#define TPM2    ((pTPM_Reg_Def_t) TPM2_BASEADDR)

#define TPM0_CLOCK_EN()     SIM->SIM_SCGC6 |= (1 << 24)
#define TPM1_CLOCK_EN()     SIM->SIM_SCGC6 |= (1 << 25)
#define TPM2_CLOCK_EN()     SIM->SIM_SCGC6 |= (1 << 26)

#define TPM0_CLOCK_DI()     SIM->SIM_SCCGC6 &= ~(1 << 24)
#define TPM1_CLOCK_DI()     SIM->SIM_SCCGC6 &= ~(1 << 25)
#define TPM2_CLOCK_DI()     SIM->SIM_SCCGC6 &= ~(1 << 26)

/******************************************
 * MCG peripheral def
 ******************************************/
#define MCG_BASEADDR    0x40064000
typedef struct {
    _vo uint8_t C1;
    _vo uint8_t C2;
    _vo uint8_t reserve0;
    _vo uint8_t reserve1;
    _vo uint8_t reserve2;
    _vo uint8_t reserve3;
    _vo uint8_t S;
    _vo uint8_t reserve4;
    _vo uint8_t SC;
    _vo uint8_t reserved5[15];
    _vo uint8_t MC;
} MCG_RegDef_t;

#define MCG ((MCG_RegDef_t*) MCG_BASEADDR)

void MCG_Init();

/*****************************************************************************
 * SysTick peripheral def
 * The default clock source is low frequency internal reference clock (LIRC)
 * See MCG Lite module for more configuration (Chapter 27)
 *****************************************************************************/
typedef struct {
    _vo uint32_t CSR;
    _vo uint32_t RVR;
    _vo uint32_t CVR;
    _vo uint32_t CALIB;
} SysTick_RegDef, *pSysTick_RegDef_t;

#define SysTick     ((pSysTick_RegDef_t) 0XE000E010)

extern volatile uint32_t sysTick_count;

void SysTick_Init(uint32_t load_value);

uint32_t get_tick();

void delay(uint32_t delay_in_ms);


/*
 * IRQ Numbers
 */
#define IRQ_NUMBER_DMA0             0
#define IRQ_NUMBER_DMA1             1
#define IRQ_NUMBER_DMA2             2
#define IRQ_NUMBER_DMA3             3
#define IRQ_NUMBER_FTFA             5
#define IRQ_NUMBER_PMC              6
#define IRQ_NUMBER_LLWU             7
#define IRQ_NUMBER_I2C0             8
#define IRQ_NUMBER_I2C1             9
#define IRQ_NUMBER_SPI0             10
#define IRQ_NUMBER_SPI1             11
#define IRQ_NUMBER_LPUART0          12
#define IRQ_NUMBER_LPUART1          13
#define IRQ_NUMBER_UART2_FLEXIO     14
#define IRQ_NUMBER_ADC0             15
#define IRQ_NUMBER_CMP0             16
#define IRQ_NUMBER_TMP0             17
#define IRQ_NUMBER_TMP1             18
#define IRQ_NUMBER_TMP2             19
#define IRQ_NUMBER_RTC_ALARM        20
#define IRQ_NUMBER_RTC_SEC          21
#define IRQ_NUMBER_PIT              22
#define IRQ_NUMBER_USB              24
#define IRQ_NUMBER_DAC0             25
#define IRQ_NUMBER_LPTMR0           28
#define IRQ_NUMBER_LCD              29
#define IRQ_NUMBER_PORTA            30
#define IRQ_NUMBER_PORTDC           31


/*
 * Generic macros
 */
#define DISABLE     0x0
#define ENABLE      0x1
#define LOW         0x0
#define HIGH        0x1
#define SET         0x1
#define RESET       0x0
#define FLAG_SET    SET
#define FLAG_RESET  RESET
#define GPIO_PIN_SET    SET
#define GPIO_PIN_RESET  RESET

#define DISABLE_IRQ()       do{asm volatile("cpsid i");} while(0)
#define ENABLE_IRQ()        do{asm volatile("cpsie i");} while(0)
#define RESET_INTERRUPT()   do{*NVIC_ICPR |= 0xFFFFFFFF;} while(0)

#include "gpio_driver.h"
#include "port_driver.h"
#include "spi_driver.h"
#include "i2c_driver.h"
#include "adc_driver.h"
#include "uart_driver.h"
#include "lpuart_driver.h"
#include "lptmr_driver.h"
#include "tpm_driver.h"
#include "led_control.h"
#include "front_panel_peripheral.h"

#endif //FRONT_PANEL_K32LB3X_H

