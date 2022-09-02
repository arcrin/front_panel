//
// Created by wbai on 8/22/2022.
//
#include "k32lb11.h"

void delay_400ns(){
    __asm__("nop");
//    __asm__("nop");
}

void delay_850ns(){
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
//    __asm__("nop");
//    __asm__("nop");
//    __asm__("nop");
//    __asm__("nop");
//    __asm__("nop");
//    __asm__("nop");
//    __asm__("nop");
//    __asm__("nop");
//    __asm__("nop");
//    __asm__("nop");
//    __asm__("nop");
//    __asm__("nop");
//    __asm__("nop");
//    __asm__("nop");
//    __asm__("nop");
//    __asm__("nop");
//    __asm__("nop");
//    __asm__("nop");
//    __asm__("nop");
//    __asm__("nop");
}

void code0(pGPIO_RegDef_t pGPIOx, uint8_t pin_number){
    // code 0
    GPIO_PinToggle(pGPIOx, pin_number);
    delay_400ns();
    GPIO_PinToggle(pGPIOx, pin_number);
    delay_850ns();
}

void code1(pGPIO_RegDef_t pGPIOx, uint8_t pin_number){
    // code 1
    GPIO_PinToggle(pGPIOx, pin_number);
    delay_850ns();
    GPIO_PinToggle(pGPIOx, pin_number);
    delay_400ns();
}

void LED_GREEN(pGPIO_RegDef_t pGPIOx, uint8_t pin_number, uint8_t* led_name){
    DISABLE_IRQ();
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code1(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code1(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);

    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);

    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    ENABLE_IRQ();
    *led_name = COLOR_GREEN;
}

void LED_RED(pGPIO_RegDef_t pGPIOx, uint8_t pin_number, uint8_t* led_name){
    DISABLE_IRQ();
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);

    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code1(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code1(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);

    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    ENABLE_IRQ();
    *led_name = COLOR_RED;
}

void LED_AMBER(pGPIO_RegDef_t pGPIOx, uint8_t pin_number, uint8_t* led_name){
    DISABLE_IRQ();
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code1(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code1(pGPIOx, pin_number);

    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code1(pGPIOx, pin_number);
    code1(pGPIOx, pin_number);
    code1(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);

    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    ENABLE_IRQ();
    *led_name = COLOR_AMBER;
}


void LED_BLUE(pGPIO_RegDef_t pGPIOx, uint8_t pin_number, uint8_t* led_name){
    DISABLE_IRQ();
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);

    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);

    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code1(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code1(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    ENABLE_IRQ();
    *led_name = COLOR_BLUE;
}

void LED_OFF(pGPIO_RegDef_t pGPIOx, uint8_t pin_number, uint8_t* led_name){
    DISABLE_IRQ();
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);

    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);

    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    code0(pGPIOx, pin_number);
    ENABLE_IRQ();
    *led_name = COLOR_OFF;
}