//
// Created by andy- on 2022-08-17.
//
#include "k32lb11.h"

char msg[64] = "UART Tx testing...\n\r";
char display_buffer[15] = "Front Panel->";
char empty_buffer[256] = "                                                   ";
char wrong_cmd_buffer[19] = "\ncmd not supported";
char firmware_info_buffer[5] = "\n0.0";
char cmd_buffer[256];
char byte_buffer[1];

GPIO_Handle_t sw1_gpio_handle;
LPUART_Handle_t lpuart_handle;

void LPUART_Port_Init(){
    PORT_Handle_t lpuart_port_handle;
    lpuart_port_handle.pPORT = PORTE;
    lpuart_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION3;
    lpuart_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    lpuart_port_handle.PORT_Config.PORT_Pin_Pull_Enable = ENABLE;
    lpuart_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_UP;

    // Tx
    lpuart_port_handle.PORT_Config.PORT_Pin_Number = 0;
    PORT_Init(&lpuart_port_handle);

    // Rx
    lpuart_port_handle.PORT_Config.PORT_Pin_Number = 1;
    PORT_Init(&lpuart_port_handle);
}

void LPUART1_Init(){
    lpuart_handle.pLPUARTx = LPUART1;
    lpuart_handle.LPUART_Config.LPUART_Mode = LPUART_TXRX_MODE;
    lpuart_handle.LPUART_Config.LPUART_WordLength = LPUART_WORDLEN_8BITS;
    lpuart_handle.LPUART_Config.LPUART_ParityControl = LPUART_PARITY_DISABLE;
    lpuart_handle.LPUART_Config.LPUART_Baud = 9600;
    lpuart_handle.RxLen = 0;
    lpuart_handle.TxLen = 0;
    LPUART_Init(&lpuart_handle);
    lpuart_handle.pLPUARTx->CTRL |= (1 << LPUART_CTRL_RIE);
    lpuart_handle.pLPUARTx->CTRL &= ~(1 << LPUART_CTRL_TIE);
    lpuart_handle.pLPUARTx->CTRL &= ~(1 << LPUART_CTRL_TCIE);
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
    SIM->SIM_CLKDIV1 &= ~(0x7 << 16);
    LPUART_Port_Init();
    LPUART1_Init();
    SW1_Init();
    IRQPriorityConfig(IRQ_NUMBER_LPUART1, 3);
    InterruptConfig(IRQ_NUMBER_LPUART1, ENABLE);
    RESET_INTERRUPT();
    ENABLE_IRQ();
//    while(1){
//        while((GPIO_ReadFromInputPin(sw1_gpio_handle.pGPIOx, sw1_gpio_handle.GPIO_Config.GPIO_PinNumber)));
//        delay(50);
//        LPUART_SendData(&lpuart_handle, (uint8_t *) msg, strlen(msg));
//    }
    while(1);
}

extern "C"{
    void LPUART1_Handler(){
        LPUART_IRQHandling(&lpuart_handle);
    }
}

void LPUART_ApplicationEventCallback(pLPUART_Handle_t pLPUARTHandle, uint8_t app_event) {
    if (app_event == LPUART_BYTE_RCV_EVENT) {
        if (pLPUARTHandle->ByteReceived == '\r'){
            if(strlen(cmd_buffer) == 0) {
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t*) display_buffer, strlen(display_buffer));
            } else if(strcmp(cmd_buffer, "firmware") == 0){
                memset(cmd_buffer, 0, 256);
                LPUART_SendData(pLPUARTHandle, (uint8_t*) firmware_info_buffer, strlen(firmware_info_buffer));
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t*) display_buffer, strlen(display_buffer));
            } else{
                memset(cmd_buffer, 0, 256);
                LPUART_SendData(pLPUARTHandle, (uint8_t*) wrong_cmd_buffer, strlen(wrong_cmd_buffer));
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t*) display_buffer, strlen(display_buffer));
            }
        }else if(pLPUARTHandle->ByteReceived == '\b'){
            if (strlen(cmd_buffer) > 0) {
                cmd_buffer[strlen(cmd_buffer) - 1] = 0;
                LPUART_SendByte(pLPUARTHandle, 0xD);
                LPUART_SendData(pLPUARTHandle, (uint8_t*) empty_buffer, strlen(empty_buffer));
                LPUART_SendByte(pLPUARTHandle, 0xD);
                LPUART_SendData(pLPUARTHandle, (uint8_t*) display_buffer, strlen(display_buffer));
                LPUART_SendData(pLPUARTHandle, (uint8_t*) cmd_buffer, strlen(cmd_buffer));

            }
        } else {
            byte_buffer[0] = pLPUARTHandle->ByteReceived;
            LPUART_SendData(pLPUARTHandle, (uint8_t*) byte_buffer, 1);
            uint8_t cmd_len = strlen(cmd_buffer);
            cmd_buffer[cmd_len] = byte_buffer[0];
        }
    }
}