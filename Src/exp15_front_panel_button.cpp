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
    FRONT_PANEL_TEST_STATUS_LED_INIT();
    FRONT_PANEL_START_RELEASE_BUTTON_INIT();
    FRONT_PANEL_RESEAT_BUTTON_INIT();
    ENABLE_IRQ();
    while(1);
}

extern "C"{
    void PortCD_SingleInterrupt_Handler(){
        if (PORTC->PORT_PCR[4] & (1 << 24)) {
            PORT_IRQHandling(PORTC, 4);
            FRONT_PANEL_JIG_LED_GREEN();
        } else if (PORTC->PORT_PCR[21] & (1 << 24)) {
            PORT_IRQHandling(PORTC, 21);
            FRONT_PANEL_JIG_LED_OFF();
        }
    }
}