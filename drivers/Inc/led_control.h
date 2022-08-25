//
// Created by wbai on 8/22/2022.
//

#ifndef FRONT_PANEL_LED_CONTROL_H
#define FRONT_PANEL_LED_CONTROL_H

#include "gpio_driver.h"

void LED_GREEN(pGPIO_RegDef_t pGPIOx, uint8_t pin_number);

void LED_RED(pGPIO_RegDef_t pGPIOx, uint8_t pin_number);

void LED_BLUE(pGPIO_RegDef_t pGPIOx, uint8_t pin_number);

void LED_OFF(pGPIO_RegDef_t pGPIOx, uint8_t pin_number);

#endif //FRONT_PANEL_LED_CONTROL_H
