//
// Created by wbai on 8/22/2022.
//
#include "k32lb11.h"

#define LED_SOLID   0
#define LED_BLINK   1

PORT_Handle_t test_port_handle;
GPIO_Handle_t test_gpio_handle;

PORT_Handle_t green_led_port_handle;
GPIO_Handle_t green_led_gpio_handle;

PORT_Handle_t sw1_port_handle;
GPIO_Handle_t sw1_gpio_handle;

uint32_t led_time_stamp = 0;

uint8_t led_blink_status = LED_SOLID;

uint32_t seconds = 0;

uint32_t get_tenth_seconds(){
    return seconds;
}


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
    sw1_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_PIN_FT;
    sw1_port_handle.PORT_Config.PORT_Pin_Pull_Enable = ENABLE;
    sw1_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_UP;
    PORT_Init(&sw1_port_handle);


    sw1_gpio_handle.pGPIOx = GPIOA;
    sw1_gpio_handle.GPIO_Config.GPIO_PinNumber = 4;
    sw1_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_INPUT;
    GPIO_Init(&sw1_gpio_handle);

    LPTMR_Init();

    IRQPriorityConfig(IRQ_NUMBER_PORTA, 3);
    InterruptConfig(IRQ_NUMBER_PORTA, ENABLE);

    IRQPriorityConfig(IRQ_NUMBER_LPTMR0, 3);
    InterruptConfig(IRQ_NUMBER_LPTMR0, ENABLE);

    SysTick_Init(480);
    ENABLE_IRQ();
//    GPIO_WriteOutputPin(green_led_gpio_handle.pGPIOx, green_led_gpio_handle.GPIO_Config.GPIO_PinNumber, LOW);
    GPIO_WriteOutputPin(test_gpio_handle.pGPIOx, test_gpio_handle.GPIO_Config.GPIO_PinNumber, LOW);
    while(1){
        if (led_blink_status == LED_BLINK) {
            if((get_seconds() - led_time_stamp) >= 1){
//                GPIO_PinToggle(green_led_gpio_handle.pGPIOx, green_led_gpio_handle.GPIO_Config.GPIO_PinNumber);
                GPIO_PinToggle(test_gpio_handle.pGPIOx, test_gpio_handle.GPIO_Config.GPIO_PinNumber);
                led_time_stamp = get_seconds();
            }
        } else if (led_blink_status == LED_SOLID) {
//            GPIO_WriteOutputPin(green_led_gpio_handle.pGPIOx, green_led_gpio_handle.GPIO_Config.GPIO_PinNumber, LOW);
            GPIO_WriteOutputPin(test_gpio_handle.pGPIOx, test_gpio_handle.GPIO_Config.GPIO_PinNumber, LOW);
        }
    }
}

extern "C"{
    void LPTMR0_Handler(){
        LPTMR->CSR |= (1 << LPTMR_CSR_TCF);
        seconds++;
    }
}

extern "C"{
    void PortA_PinDetect_Handler() {
        PORT_IRQHandling(sw1_port_handle.pPORT, 4); // clear IRQ flag
        if (led_blink_status == LED_SOLID){
            led_blink_status = LED_BLINK;
            led_time_stamp = get_seconds();
        } else if (led_blink_status == LED_BLINK){
            led_blink_status = LED_SOLID;
        }
    }
}


