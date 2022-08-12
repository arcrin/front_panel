//
// Created by wbai on 5/4/2022.
//
#include "k32lb11.h"

void delay(){
    for(uint32_t i = 0; i < 500000/10; i++);
}

PORT_Handle_t green_led_port_handle;
GPIO_Handle_t green_led_gpio_handle;
PORT_Handle_t sw1_port_handle;
GPIO_Handle_t sw1_gpio_handle;

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

    // sw1 (BUTTON2) set up
    sw1_port_handle.pPORT = PORTA;
    sw1_port_handle.PORT_Config.PORT_Pin_Number = 4;
    sw1_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    sw1_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_PIN_ET;
    sw1_port_handle.PORT_Config.PORT_Pin_Pull_Enable = ENABLE;
    sw1_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_UP;
    PORT_Init(&sw1_port_handle);


    sw1_gpio_handle.pGPIOx = GPIOA;
    sw1_gpio_handle.GPIO_Config.GPIO_PinNumber = 4;
    sw1_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_INPUT;
    GPIO_Init(&sw1_gpio_handle);



    IRQPriorityConfig(IRQ_NUMBER_PORTA, 3);
    InterruptConfig(IRQ_NUMBER_PORTA, ENABLE);

    RESET_INTERRUPT();
    ENABLE_IRQ();
    GPIO_WriteOutputPin(green_led_gpio_handle.pGPIOx, 5, HIGH);
    while(1);

}

extern "C"{
    void PortA_PinDetect_Handler() {
        delay();
        PORT_IRQHandling(sw1_port_handle.pPORT, 4);
//        GPIO_PinToggle(green_led_gpio_handle.pGPIOx, 5);
        if (GPIO_ReadFromInputPin(sw1_gpio_handle.pGPIOx, sw1_gpio_handle.GPIO_Config.GPIO_PinNumber)) {
            GPIO_WriteOutputPin(green_led_gpio_handle.pGPIOx, 5, HIGH);
        } else {
            GPIO_WriteOutputPin(green_led_gpio_handle.pGPIOx, 5, LOW);
        }
    }
}
