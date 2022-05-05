//
// Created by wbai on 4/28/2022.
//

#ifndef FRONT_PANEL_PORT_H
#define FRONT_PANEL_PORT_H
#include "k32lb11.h"

/*
 * Interrupt Configuration
 */
#define ISF_DISABLE     0x0
#define ISF_DMA_RT      0x1
#define ISF_DMA_FT      0x2
#define ISF_DMA_ET      0x3
#define ISF_PIN_0T      0x8
#define ISF_PIN_RT      0x9
#define ISF_PIN_FT      0xA
#define ISF_PIN_ET      0xB
#define ISF_PIN_1T      0xC

/*
 * Pin Mux Control
 */
#define PIN_DISABLED    0x0
#define ALT_FUNCTION1   0x1
#define ALT_FUNCTION2   0x2
#define ALT_FUNCTION3   0x3
#define ALT_FUNCTION4   0x4
#define ALT_FUNCTION5   0x5
#define ALT_FUNCTION6   0x6
#define ALT_FUNCTION7   0x7

/*
 * Pull Select
 */
#define PULL_DOWN       0x0
#define PULL_UP         0x1

typedef struct {
    _vo uint8_t PORT_Pin_Number;
    _vo uint8_t PORT_Pin_Function;
    _vo uint8_t PORT_Pin_Pull_Enable;
    _vo uint8_t PORT_Pin_Pull_Select;
    _vo uint8_t PORT_Pin_Interrupt_cfg;
}PORT_Config_t;

typedef struct {
    pPORT_RegDef_t pPORT;
    PORT_Config_t PORT_Config;
}PORT_Handle_t;

/*
 * PORT clock setup
 */
void PORT_ClockControl(pPORT_RegDef_t pPORTx, uint8_t ENorDI);

/*
 * PORT init
 */
void PORT_Init(PORT_Handle_t* pPortHandle);

/*
 * Interrupt configuration
 */
void PORT_InterruptConfig(uint8_t IRQNumber, uint8_t ENorDI);

void PORT_IRQPriorityConfig(uint8_t IRQNumber, uint32_t priority);

void PORT_IRQHandling(pPORT_RegDef_t pPORT, uint8_t PinNumber);

#endif //FRONT_PANEL_PORT_H
