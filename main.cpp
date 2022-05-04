#include <cstdint>
#include <cstdio>
#include "k32lb11.h"

void delay(){
    for(uint32_t i = 0; i < 500000/10; i++);
}

int main(){
    PORT_Handle_t green_led_port_handle;
    green_led_port_handle.pPORT = PORTD;
    green_led_port_handle.PORT_Config.PORT_Pin_Number = 5;
    green_led_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    green_led_port_handle.PORT_Config.PORT_Pin_Pull_Enable = ENABLE;
    green_led_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_UP;
    PORT_Init(&green_led_port_handle);

    GPIO_Handle_t green_led_gpio_handle;
    green_led_gpio_handle.pGPIOx = GPIOD;
    green_led_gpio_handle.GPIO_Config.GPIO_PinNumber = 5;
    green_led_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_OUTPUT;
    GPIO_Init(&green_led_gpio_handle);


    PORT_Handle_t red_led_port_handle;
    red_led_port_handle.pPORT = PORTE;
    red_led_port_handle.PORT_Config.PORT_Pin_Number = 31;
    red_led_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    red_led_port_handle.PORT_Config.PORT_Pin_Pull_Enable = ENABLE;
    red_led_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_UP;
    PORT_Init(&red_led_port_handle);

    GPIO_Handle_t red_led_gpio_handle;
    red_led_gpio_handle.pGPIOx = GPIOE;
    red_led_gpio_handle.GPIO_Config.GPIO_PinNumber = 31;
    red_led_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_OUTPUT;
    GPIO_Init(&red_led_gpio_handle);

    PORT_Handle_t sw1_port_handle;
    sw1_port_handle.pPORT = PORTA;
    sw1_port_handle.PORT_Config.PORT_Pin_Number = 4;
    sw1_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    sw1_port_handle.PORT_Config.PORT_Pin_Pull_Enable = ENABLE;
    sw1_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_UP;
    PORT_Init(&sw1_port_handle);

    GPIO_Handle_t sw1_gpio_handle;
    sw1_gpio_handle.pGPIOx = GPIOA;
    sw1_gpio_handle.GPIO_Config.GPIO_PinNumber = 4;
    sw1_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_INPUT;
    GPIO_Init(&sw1_gpio_handle);

    PORT_Handle_t sw3_port_handle;
    sw3_port_handle.pPORT = PORTC;
    sw3_port_handle.PORT_Config.PORT_Pin_Number = 3;
    sw3_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    sw3_port_handle.PORT_Config.PORT_Pin_Pull_Enable = ENABLE;
    sw3_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_UP;
    PORT_Init(&sw3_port_handle);

    GPIO_Handle_t sw3_gpio_handle;
    sw3_gpio_handle.pGPIOx = GPIOC;
    sw3_gpio_handle.GPIO_Config.GPIO_PinNumber = 3;
    sw3_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_INPUT;
    GPIO_Init(&sw3_gpio_handle);



    while(1){
        if(GPIO_ReadFromPin(sw1_gpio_handle.pGPIOx, sw1_gpio_handle.GPIO_Config.GPIO_PinNumber) == 0){
            GPIO_WriteOutputPin(red_led_gpio_handle.pGPIOx,
                                red_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                                LOW);
            delay();
        }
        else if (GPIO_ReadFromPin(sw3_gpio_handle.pGPIOx, sw3_gpio_handle.GPIO_Config.GPIO_PinNumber) == 0){
            GPIO_WriteOutputPin(green_led_gpio_handle.pGPIOx,
                                green_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                                LOW);
            delay();
        }
        else{
            GPIO_WriteOutputPin(red_led_gpio_handle.pGPIOx,
                                red_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                                HIGH);

            GPIO_WriteOutputPin(green_led_gpio_handle.pGPIOx,
                                green_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                                HIGH);
        }
//        GPIO_PinToggle(green_led_gpio_handle.pGPIOx, green_led_gpio_handle.GPIO_Config.GPIO_PinNumber);
//        delay();
    }

}
