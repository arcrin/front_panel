//
// Created by wbai on 5/31/2022.
//


#include "k32lb11.h"
#include <cstring>

PORT_Handle_t sw1_port_handle;
GPIO_Handle_t sw1_gpio_handle;
SPI_Handle_t spi0_handle;
PORT_Handle_t green_led_port_handle;
GPIO_Handle_t green_led_gpio_handle;

const char user_data[] = "Hello World";

void delay(){
    for(uint32_t i = 0; i < 500000/10; i++);
}

// 1. set up the port(pins) for SPI functionality
// need a port init function
void SPI0_Port_Pin_Init(void) {
    /*
     * SPI pin map:
     * NSS: PTC4
     * SCK: PTC5
     * MOSI: PTC6
     * MISO: PTC7
     */
    PORT_Handle_t spi0_port_handle;
    spi0_port_handle.pPORT = PORTC;
    spi0_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION2;
    spi0_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    spi0_port_handle.PORT_Config.PORT_Pin_Pull_Enable = ENABLE;
    spi0_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_UP;

    // NSS pin init
    spi0_port_handle.PORT_Config.PORT_Pin_Number = 4;
    PORT_Init(&spi0_port_handle);

    // SCK pin
    spi0_port_handle.PORT_Config.PORT_Pin_Number = 5;
    PORT_Init(&spi0_port_handle);

    // MOSI pin
    spi0_port_handle.PORT_Config.PORT_Pin_Number = 6;
    PORT_Init(&spi0_port_handle);

    // MISO pin
    spi0_port_handle.PORT_Config.PORT_Pin_Number = 7;
    PORT_Init(&spi0_port_handle);
}

// 2. configure the SPI module to my needs
// need a  SPI init function
void SPI0_Init(void){

    spi0_handle.pSPIx = SPI0;
    spi0_handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MASTER_MODE;
    spi0_handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
    spi0_handle.SPIConfig.SPI_BR_PRE = SPI_SCLK_PRE_1;
    spi0_handle.SPIConfig.SPI_BR_DIV = SPI_SCLK_SPEED_DIV2;
    spi0_handle.SPIConfig.SPI_MODE = SPI_8BIT_MODE;
    spi0_handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
    spi0_handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
    SPI_Init(&spi0_handle);
}

void GPIO_Button_Init(){
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
}

void GPIO_Green_LED_Init() {
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
}


// 3. transmit data
// this would happen in main function
int main(){
    DISABLE_IRQ();
    SPI0_Port_Pin_Init();
    SPI0_Init();
    GPIO_Button_Init();
    GPIO_Green_LED_Init();
    RESET_INTERRUPT();
    ENABLE_IRQ();
    GPIO_WriteOutputPin(green_led_gpio_handle.pGPIOx, 5, HIGH);
    while (1);
}

extern "C"{
    void PortA_PinDetect_Handler(){
        delay();
        PORT_IRQHandling(sw1_port_handle.pPORT, 4);
        if (GPIO_ReadFromInputPin(sw1_gpio_handle.pGPIOx, sw1_gpio_handle.GPIO_Config.GPIO_PinNumber)) {
            GPIO_WriteOutputPin(green_led_gpio_handle.pGPIOx, 5, HIGH);
        } else {
            GPIO_WriteOutputPin(green_led_gpio_handle.pGPIOx, 5, LOW);
            delay();
            SPI_PeriControl(spi0_handle.pSPIx, ENABLE);
            uint8_t data_length = strlen(user_data);
            SPI_Send_Data(spi0_handle.pSPIx, &data_length, 1);
            SPI_Send_Data(spi0_handle.pSPIx, (uint8_t *) user_data, strlen(user_data));
            SPI_PeriControl(spi0_handle.pSPIx, DISABLE);
            delay();
        }
    }
}