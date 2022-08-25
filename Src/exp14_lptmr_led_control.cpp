//
// Created by wbai on 8/22/2022.
//
#include "k32lb11.h"

PORT_Handle_t test_port_handle;
GPIO_Handle_t test_gpio_handle;

int main(){
    DISABLE_IRQ();
    MCG_Init();
    MCG->MC |= (1 << 7); // enable high frequency internal reference clock
    test_port_handle.pPORT = PORTB;
    test_port_handle.PORT_Config.PORT_Pin_Number = 16;
    test_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    test_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    test_port_handle.PORT_Config.PORT_Pin_Pull_Enable = ENABLE;
    test_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_UP;
    PORT_Init(&test_port_handle);

    test_gpio_handle.pGPIOx = GPIOB;
    test_gpio_handle.GPIO_Config.GPIO_PinNumber = 16;
    test_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_OUTPUT;
    GPIO_Init(&test_gpio_handle);

//    LPTMR_Init();
//    LPTMR->CMR = 0x1;

//    IRQPriorityConfig(IRQ_NUMBER_LPTMR0, 3);
//    InterruptConfig(IRQ_NUMBER_LPTMR0, ENABLE);

    SysTick_Init(480);
    ENABLE_IRQ();
    LED_OFF(test_gpio_handle.pGPIOx, test_gpio_handle.GPIO_Config.GPIO_PinNumber);
    LED_OFF(test_gpio_handle.pGPIOx, test_gpio_handle.GPIO_Config.GPIO_PinNumber);
    delay(10);
    LED_OFF(test_gpio_handle.pGPIOx, test_gpio_handle.GPIO_Config.GPIO_PinNumber);
    LED_OFF(test_gpio_handle.pGPIOx, test_gpio_handle.GPIO_Config.GPIO_PinNumber);
//    LED_GREEN(test_gpio_handle.pGPIOx, test_gpio_handle.GPIO_Config.GPIO_PinNumber);
//    LED_GREEN(test_gpio_handle.pGPIOx, test_gpio_handle.GPIO_Config.GPIO_PinNumber);
//    LED_RED(test_gpio_handle.pGPIOx, test_gpio_handle.GPIO_Config.GPIO_PinNumber);
//    LED_RED(test_gpio_handle.pGPIOx, test_gpio_handle.GPIO_Config.GPIO_PinNumber);
//    LED_BLUE(test_gpio_handle.pGPIOx, test_gpio_handle.GPIO_Config.GPIO_PinNumber);
//    LED_BLUE(test_gpio_handle.pGPIOx, test_gpio_handle.GPIO_Config.GPIO_PinNumber);


    while(1);
}

extern "C"{
    void LPTMR0_Handler(){
        LPTMR->CSR |= (1 << 7);
        GPIO_PinToggle(test_gpio_handle.pGPIOx, 20);
    }
}

