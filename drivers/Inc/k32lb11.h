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
 * Port Pin Control Registers
 */
#define PORTA_BASEADDR      0x40049000
#define PORTB_BASEADDR      0x4004A000
#define PORTC_BASEADDR      0x4004B000
#define PORTD_BASEADDR      0x4004C000
#define PORTE_BASEADDR      0x4004D000

typedef struct {
    _vo uint32_t PORT_PCR[32];
    _vo uint32_t PORT_GPCLR;
    _vo uint32_t PORT_GPCHR;
}PORT_RegDef_t, *pPORT_RegDef_t;

/*
 * System Integration Module Registers
 */
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


/*
 * GPIO Registers
 */
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


/*
 * Clock Gate Control Macros
 */
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


#endif //FRONT_PANEL_K32LB3X_H
