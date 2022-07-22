//
// Created by wbai on 7/20/2022.
//
#include "k32lb11.h"
#include <cstring>

#define own_address     0x61
#define slave_address   0x68

I2C_Handle_t i2c0_handle;
PORT_Handle_t sw1_port_handle;
GPIO_Handle_t sw1_gpio_handle;

uint8_t sample_data[] = "We are testing I2C master Tx\n";

void SW1_Init(){
    PORT_Handle_t green_led_port_handle;
    green_led_port_handle.pPORT = PORTD;
    green_led_port_handle.PORT_Config.PORT_Pin_Number = 5;
    green_led_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    green_led_port_handle.PORT_Config.PORT_Pin_Pull_Enable = ENABLE;
    green_led_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_UP;
    green_led_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
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
    red_led_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;

    PORT_Init(&red_led_port_handle);

    GPIO_Handle_t red_led_gpio_handle;
    red_led_gpio_handle.pGPIOx = GPIOE;
    red_led_gpio_handle.GPIO_Config.GPIO_PinNumber = 31;
    red_led_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_OUTPUT;
    GPIO_Init(&red_led_gpio_handle);

    sw1_port_handle.pPORT = PORTA;
    sw1_port_handle.PORT_Config.PORT_Pin_Number = 4;
    sw1_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    sw1_port_handle.PORT_Config.PORT_Pin_Pull_Enable = ENABLE;
    sw1_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_UP;
    sw1_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    PORT_Init(&sw1_port_handle);

    sw1_gpio_handle.pGPIOx = GPIOA;
    sw1_gpio_handle.GPIO_Config.GPIO_PinNumber = 4;
    sw1_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_INPUT;
    GPIO_Init(&sw1_gpio_handle);

//    PORT_Handle_t sw3_port_handle;
//    sw3_port_handle.pPORT = PORTC;
//    sw3_port_handle.PORT_Config.PORT_Pin_Number = 3;
//    sw3_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
//    sw3_port_handle.PORT_Config.PORT_Pin_Pull_Enable = ENABLE;
//    sw3_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_UP;
//    sw3_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
//    PORT_Init(&sw3_port_handle);
//
//    GPIO_Handle_t sw3_gpio_handle;
//    sw3_gpio_handle.pGPIOx = GPIOC;
//    sw3_gpio_handle.GPIO_Config.GPIO_PinNumber = 3;
//    sw3_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_INPUT;
//    GPIO_Init(&sw3_gpio_handle);
}

void I2C0_Port_Pin_Init(){
    PORT_Handle_t i2c0_port_handle;
    i2c0_port_handle.pPORT = PORTB;
    i2c0_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION2;
    i2c0_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    i2c0_port_handle.PORT_Config.PORT_Pin_Pull_Enable = ENABLE;
    i2c0_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_UP;

    // SCL
    i2c0_port_handle.PORT_Config.PORT_Pin_Number = 0;
    PORT_Init(&i2c0_port_handle);

    // SDA
    i2c0_port_handle.PORT_Config.PORT_Pin_Number = 1;
    PORT_Init(&i2c0_port_handle);
}

void I2C0_Init(){
    i2c0_handle.pI2Cx = I2C0;
    i2c0_handle.I2C_Config.I2C_DeviceAddress = own_address;
    I2C_Init(&i2c0_handle);
}

int main(){
    DISABLE_IRQ();
    SysTick_Init(800);

    I2C0_Port_Pin_Init();
    // set up I2C baud rate
    I2C0_Init();

    SW1_Init();

    ENABLE_IRQ();

    while(1){
        uint8_t *pTxBuffer = sample_data;
        while(GPIO_ReadFromPin(sw1_gpio_handle.pGPIOx, sw1_gpio_handle.GPIO_Config.GPIO_PinNumber));
        delay(1000); // 100 ms delay
        // write to Control Register 1 to enable I2C module
        i2c0_handle.pI2Cx->C1 |= 1 << I2C_C1_IICEN;
        // write to Control Register 1 to TX
        i2c0_handle.pI2Cx->C1 |= 1 << I2C_C1_TX;
        // write to Control Register 1 to MST (master mode, this generates START condition)
        i2c0_handle.pI2Cx->C1 |= 1 << I2C_C1_MST;
        // write address with read to the bus
        uint8_t SlaveAddress = slave_address << 1;
        SlaveAddress &= ~(1);
        i2c0_handle.pI2Cx->D = SlaveAddress;
        while(! I2C_GetFlagStatus(i2c0_handle.pI2Cx, I2C_FLAG_TCF, 1));

        uint32_t Len = strlen((char *)sample_data);

        while (Len > 0){
            i2c0_handle.pI2Cx->D = *pTxBuffer;
            while(! I2C_GetFlagStatus(i2c0_handle.pI2Cx, I2C_FLAG_TCF, 1));
            pTxBuffer++;
            Len--;
        }
        // stop condition (change C1->MST 1 to 0)
        i2c0_handle.pI2Cx->C1 &= ~(1 << I2C_C1_MST);

    }

}