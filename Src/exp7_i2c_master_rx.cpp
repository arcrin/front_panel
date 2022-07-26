//
// Created by andy- on 2022-07-24.
//

#include "k32lb11.h"
#include <cstring>

#define own_address     0x61
#define slave_address   0x68

I2C_Handle_t i2c0_handle;
PORT_Handle_t sw1_port_handle;
GPIO_Handle_t sw1_gpio_handle;

uint8_t rcv_buffer[32];
uint8_t sample_data = 0x51;

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
    i2c0_handle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
    I2C_Init(&i2c0_handle);
}

int main(){
    DISABLE_IRQ();
    uint8_t command_code;

    MCG_Init();
    SysTick_Init(800);
    I2C0_Port_Pin_Init();
    I2C0_Init();
    SW1_Init();
    // Enable I2C module
    i2c0_handle.pI2Cx->C1 |= 1 << I2C_C1_IICEN;
//    i2c0_handle.pI2Cx->C1 |= 1 << I2C_C1_IICIE;
    ENABLE_IRQ();

    while(1) {
        uint8_t rcv_data_len;
        while(GPIO_ReadFromPin(sw1_gpio_handle.pGPIOx, sw1_gpio_handle.GPIO_Config.GPIO_PinNumber));
        delay(1000);
        uint8_t temp;
        command_code = 0x51;
//        I2C_MasterSendData(&i2c0_handle, &command_code, 1, slave_address, false, true);
//        I2C_MasterReceiveData(&i2c0_handle, &rcv_data_len, 1, slave_address,true, false);
//        command_code = 0x52;
//        I2C_MasterSendData(&i2c0_handle, &command_code, 1, slave_address, false, false);
//        I2C_MasterReceiveData(&i2c0_handle, rcv_buffer, rcv_data_len, slave_address, true, false);

        I2C_MasterReceiveData(&i2c0_handle, &rcv_data_len, 1, slave_address,true, true);
        temp = rcv_data_len;
    }
}