//
// Created by andy- on 2022-08-15.
//
#include "k32lb11.h"

GPIO_Handle_t sw1_gpio_handle;
UART_Handle_t uart_handle;

char msg[64] = "UART Tx testing...\n\r";
char display_buffer[15] = "Front Panel->";
char empty_buffer[256] = "                                                   ";
char wrong_cmd_buffer[19] = "\ncmd not supported";
char firmware_info_buffer[5] = "\n0.0";
char cmd_buffer[256];
char byte_buffer[1];

void UART_Port_Init(){
    PORT_Handle_t uart_port_handle;
    uart_port_handle.pPORT = PORTE;
    uart_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION4;
    uart_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    uart_port_handle.PORT_Config.PORT_Pin_Pull_Enable = ENABLE;
    uart_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_UP;

    // Tx
    uart_port_handle.PORT_Config.PORT_Pin_Number = 22;
    PORT_Init(&uart_port_handle);

    // Rx
    uart_port_handle.PORT_Config.PORT_Pin_Number = 23;
    PORT_Init(&uart_port_handle);
}

void UART2_Init(){
    uart_handle.pUARTx = UART2;
    uart_handle.UART_Config.UART_Mode = UART_TXRX_MODE;
    uart_handle.UART_Config.UART_WordLength = UART_WORDLEN_8BITS;
    uart_handle.UART_Config.UART_ParityControl = UART_PARITY_DISABLE;
    uart_handle.UART_Config.UART_Baud = 115200;
    uart_handle.RxLen = 0;
    uart_handle.TxLen = 0;
    UART_Init(&uart_handle);
//    uart_handle.pUARTx->C2 |= (1 << UART_C2_RIE);
//    uart_handle.pUARTx->C2 &= ~(1 << UART_C2_TIE);
//    uart_handle.pUARTx->C2 &= ~(1 << UART_C2_TCIE);
}

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

    PORT_Handle_t sw1_port_handle;


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

int main(){
    DISABLE_IRQ();
    MCG_Init();
    SysTick_Init(8000);
    SIM->SIM_CLKDIV1 &= ~(0x7 << 16); // for 8 MHz bus clock
    UART_Port_Init();
    UART2_Init();
    SW1_Init();
    IRQPriorityConfig(IRQ_NUMBER_UART2_FLEXIO, 3);
    InterruptConfig(IRQ_NUMBER_UART2_FLEXIO, ENABLE);
    RESET_INTERRUPT();
    ENABLE_IRQ();
    while(1){
        while(GPIO_ReadFromInputPin(sw1_gpio_handle.pGPIOx, sw1_gpio_handle.GPIO_Config.GPIO_PinNumber));
        delay(50);
        UART_SendData(&uart_handle, (uint8_t*) msg, strlen(msg));
    }
//    while(1);
}

extern "C"{
    void UART2_FlexIO_Handler(){
        UART_IRQHandling(&uart_handle);
    }
}

void UART_ApplicationEventCallback(pUART_Handle_t pUARTHandle, uint8_t app_event){
    if (app_event == UART_BYTE_RCV_EVENT) {
        if (pUARTHandle->ByteReceived == '\r'){
            if(strlen(cmd_buffer) == 0) {
                UART_SendByte(pUARTHandle, '\n');
                UART_SendData(pUARTHandle, (uint8_t*) display_buffer, strlen(display_buffer));
            } else if(strcmp(cmd_buffer, "firmware") == 0){
                memset(cmd_buffer, 0, 256);
                UART_SendData(pUARTHandle, (uint8_t*) firmware_info_buffer, strlen(firmware_info_buffer));
                UART_SendByte(pUARTHandle, '\n');
                UART_SendData(pUARTHandle, (uint8_t*) display_buffer, strlen(display_buffer));
            } else{
                memset(cmd_buffer, 0, 256);
                UART_SendData(pUARTHandle, (uint8_t*) wrong_cmd_buffer, strlen(wrong_cmd_buffer));
                UART_SendByte(pUARTHandle, '\n');
                UART_SendData(pUARTHandle, (uint8_t*) display_buffer, strlen(display_buffer));
            }
        }else if(pUARTHandle->ByteReceived == '\b'){
            if (strlen(cmd_buffer) > 0) {
                cmd_buffer[strlen(cmd_buffer) - 1] = 0;
                UART_SendByte(pUARTHandle, 0xD);
                UART_SendData(pUARTHandle, (uint8_t*) empty_buffer, strlen(empty_buffer));
                UART_SendByte(pUARTHandle, 0xD);
                UART_SendData(pUARTHandle, (uint8_t*) display_buffer, strlen(display_buffer));
                UART_SendData(pUARTHandle, (uint8_t*) cmd_buffer, strlen(cmd_buffer));

            }
        } else {
            byte_buffer[0] = pUARTHandle->ByteReceived;
            UART_SendData(pUARTHandle, (uint8_t*) byte_buffer, 1);
            uint8_t cmd_len = strlen(cmd_buffer);
            cmd_buffer[cmd_len] = byte_buffer[0];
        }
    }
}