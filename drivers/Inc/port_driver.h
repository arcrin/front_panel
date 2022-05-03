//
// Created by wbai on 4/28/2022.
//

#ifndef FRONT_PANEL_PORT_H
#define FRONT_PANEL_PORT_H
#include "k32lb11.h"

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

#endif //FRONT_PANEL_PORT_H
