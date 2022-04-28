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

#define GPIOA_BASEADDR      0x400FF000
#define GPIOB_BASEADDR      0x400FF040
#define GPIOC_BASEADDR      0x400FF080
#define GPIOD_BASEADDR      0x400FF0C0
#define GPIOE_BASEADDR      0x400FF100

/*
 * GPIO register structure
 */
typedef struct {
    _vo uint32_t PDOR;
    _vo uint32_t PSOR;
    _vo uint32_t PCOR;
    _vo uint32_t PTOR;
    _vo uint32_t PDIR;
    _vo uint32_t PDDR;
} GPIO_RegDef_t, *pGPIO_RegDef_t;

#endif //FRONT_PANEL_K32LB3X_H
