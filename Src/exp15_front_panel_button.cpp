//
// Created by wbai on 8/23/2022.
//
#include "k32lb11.h"

extern PORT_Handle_t start_release_button_port_handle;
extern GPIO_Handle_t start_release_button_gpio_handle;

extern PORT_Handle_t test_status_led_port_handle;
extern GPIO_Handle_t test_status_led_gpio_handle;

extern PORT_Handle_t reseat_button_port_handle;
extern GPIO_Handle_t reseat_button_gpio_handle;

int main(){
    DISABLE_IRQ();
    MCG_Init();
    MCG->MC |= (1 << 7);
    SysTick_Init(480); // 10 us delay
    TEST_STATUS_LED_INIT();
    START_RELEASE_BUTTON_INIT();
    RESEAT_BUTTON_INIT();
    ENABLE_IRQ();
    while(1);
}

extern "C"{
    void PortCD_SingleInterrupt_Handler(){
        if (PORTC->PORT_PCR[4] & (1 << 24)) {
            PORT_IRQHandling(PORTC, 4);
            LED_OFF(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber);
            LED_OFF(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber);
        } else if (PORTC->PORT_PCR[21] & (1 << 24)) {
            PORT_IRQHandling(PORTC, 21);
            LED_GREEN(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber);
            LED_GREEN(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber);
        }
    }
}