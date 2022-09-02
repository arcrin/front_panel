//
// Created by wbai on 8/22/2022.
//

#ifndef FRONT_PANEL_LED_CONTROL_H
#define FRONT_PANEL_LED_CONTROL_H

#include "gpio_driver.h"

/*******************************
 * LED Color
 *******************************/
#define COLOR_OFF         0
#define COLOR_GREEN       1
#define COLOR_RED         2
#define COLOR_BLUE        3
#define COLOR_AMBER       4

/******************************
 * LED Status
 ******************************/
#define SOLID       0
#define BLINK       1

void LED_GREEN(pGPIO_RegDef_t pGPIOx, uint8_t pin_number, uint8_t* led_name);

void LED_RED(pGPIO_RegDef_t pGPIOx, uint8_t pin_number, uint8_t* led_name);

void LED_BLUE(pGPIO_RegDef_t pGPIOx, uint8_t pin_number, uint8_t* led_name);

void LED_AMBER(pGPIO_RegDef_t pGPIOx, uint8_t pin_number, uint8_t* led_name);

void LED_OFF(pGPIO_RegDef_t pGPIOx, uint8_t pin_number, uint8_t* led_name);

#endif //FRONT_PANEL_LED_CONTROL_H
