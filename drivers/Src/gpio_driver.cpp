//
// Created by wbai on 5/2/2022.
//

#include "gpio_driver.h"

void GPIO_Init(GPIO_Handle_t* pGPIOHandle){
    uint8_t pin_number = pGPIOHandle->GPIO_Config.GPIO_PinNumber;

    // GPIO pin direction
    if(pGPIOHandle->GPIO_Config.GPIO_PinDirection == GPIO_OUTPUT){
        pGPIOHandle->pGPIOx->PDDR |= 0x1 << pin_number;
    }
    else if (pGPIOHandle->GPIO_Config.GPIO_PinDirection == GPIO_INPUT){
        pGPIOHandle->pGPIOx->PDDR &= (0x1 << pin_number);
    }
}

void GPIO_PinToggle(pGPIO_RegDef_t pGPIOx, uint8_t PinNumber){
    pGPIOx->PTOR |= 1 << PinNumber;
}

void GPIO_WriteOutputPin(pGPIO_RegDef_t pGPIOx, uint8_t PinNumber,uint8_t state){
    if (state == HIGH){
        pGPIOx->PDOR |= 0x1 << PinNumber;
    }
    else if (state == LOW){
        pGPIOx->PDOR &= ~(0x1 << PinNumber);
    }
}

uint8_t GPIO_ReadFromInputPin(pGPIO_RegDef_t pGPIOx, uint8_t PinNumber){
    uint8_t value;
    value = (pGPIOx->PDIR & (1 << PinNumber)) >> PinNumber;
    return value;
}