//
// Created by wbai on 5/2/2022.
//

#ifndef FRONT_PANEL_GPIO_H
#define FRONT_PANEL_GPIO_H
#include "k32lb11.h"

/*
 * PIN Direction
 */
#define GPIO_INPUT      0x0
#define GPIO_OUTPUT     0x1

typedef struct {
    _vo uint8_t GPIO_PinNumber;
    _vo uint8_t GPIO_PinDirection;
}GPIO_Config_t;

typedef struct {
    pGPIO_RegDef_t pGPIOx;
    GPIO_Config_t GPIO_Config;
}GPIO_Handle_t;


/*
 * GPIO Init
 */
void GPIO_Init(GPIO_Handle_t* pGPIOHandle);

/*
 * GPIO pin controls
 */
void GPIO_PinToggle(pGPIO_RegDef_t pGPIOx, uint8_t PinNumber);

void GPIO_WriteOutputPin(pGPIO_RegDef_t pGPIOx, uint8_t PinNumber, uint8_t state);

uint8_t GPIO_ReadFromPin(pGPIO_RegDef_t pGPIOx, uint8_t PinNumber);

#endif //FRONT_PANEL_GPIO_H
