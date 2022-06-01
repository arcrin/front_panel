//
// Created by wbai on 4/27/2022.
//

#ifndef FRONT_PANEL_K32LB3X_H
#define FRONT_PANEL_K32LB3X_H

#include <cstdint>

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

#define DISABLE_IEQ()       do{asm volatile("cpsid i");} while(0)
#define ENABLE_IEQ()        do{asm volatile("cpsie i");} while(0)
#define RESET_INTERRUPT()   do{*NVIC_ICPR |= 0xFFFFFFFF;} while(0)

#endif //FRONT_PANEL_K32LB3X_H

#include "gpio_driver.h"
#include "port_driver.h"