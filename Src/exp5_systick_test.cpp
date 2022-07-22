//
// Created by wbai on 7/14/2022.
//

#include "k32lb11.h"

PORT_Handle_t green_led_port_handle;
GPIO_Handle_t green_led_gpio_handle;

int main(){
    DISABLE_IRQ();

    // LED1 (green) set up
    green_led_port_handle.pPORT = PORTD;
    green_led_port_handle.PORT_Config.PORT_Pin_Number = 5;
    green_led_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    green_led_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    green_led_port_handle.PORT_Config.PORT_Pin_Pull_Enable = ENABLE;
    green_led_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_UP;
    PORT_Init(&green_led_port_handle);

    green_led_gpio_handle.pGPIOx = GPIOD;
    green_led_gpio_handle.GPIO_Config.GPIO_PinNumber = 5;
    green_led_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_OUTPUT;
    GPIO_Init(&green_led_gpio_handle);

    SysTick_Init(800); // each tick is 0.1 ms

    ENABLE_IRQ();

    while(1){
        GPIO_PinToggle(green_led_gpio_handle.pGPIOx, 5);
        delay(1);
    }
}