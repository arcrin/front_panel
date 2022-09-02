//
// Created by andy- on 2022-08-17.
//
#include "Inc/front_panel_peripheral.h"

char display_buffer[15] = "Front Panel->";
char empty_buffer[256] = "                                                   ";
char wrong_cmd_buffer[19] = "\ncmd not supported";
char firmware_info_buffer[5] = "\n0.0";
char cmd_buffer[256];
char byte_buffer[1];

char button_pressed_message[16] = "\nButton pressed";

char button_short_released_message[16] = "\nSHORT released";
char button_long_released_message[15] = "\nLONG released";

char button_reseat_message[16] = "\nRESEAT pressed";

uint32_t tenth_seconds = 0;

extern GPIO_Handle_t test_status_led_gpio_handle;

extern LPUART_Handle_t lpuart_handle;

extern ADC_Handle_t act_feedback_adc_handle;

extern GPIO_Handle_t latch_control_gpio_handle;

extern GPIO_Handle_t master_relay_gpio_handle;

extern uint8_t TEST_LED_COLOR;
extern uint8_t JIG_LED_COLOR;

extern uint8_t TEST_LED_STATUS;
extern uint8_t JIG_LED_STATUS;

uint32_t test_led_time_stamp;
uint32_t jig_led_time_stamp;

uint32_t start_release_button_time_stamp;

extern uint8_t ACT1_STATUS;
extern uint8_t ACT2_STATUS;

extern uint8_t ACT_SPEED;

uint8_t JIG_STATUS;


uint32_t get_tenth_seconds(){
    return tenth_seconds;
}

int main(){
    DISABLE_IRQ();
    MCG_Init();
    MCG->MC |= (1 << 7);
    SysTick_Init(480); // 10us delay
    FRONT_PANEL_LATCH_CONTROL_INIT();
    FRONT_PANEL_MASTER_RELAY_INIT();
    FRONT_PANEL_START_RELEASE_BUTTON_INIT();
    FRONT_PANEL_RESEAT_BUTTON_INIT();
    FRONT_PANEL_ACT1_CONTROL_INIT();
    FRONT_PANEL_ACT2_CONTROL_INIT();
    FRONT_PANEL_TEST_STATUS_LED_INIT();
    FRONT_PANEL_LPUART_INIT();
    FRONT_PANEL_ADC0_INIT();
    FRONT_PANEL_BUTTON_RGB_INIT();
    LPTMR_Init();
    ENABLE_IRQ();
    JIG_STATUS = READY;
    GPIO_WriteOutputPin(latch_control_gpio_handle.pGPIOx,
                        latch_control_gpio_handle.GPIO_Config.GPIO_PinNumber, HIGH);

    GPIO_WriteOutputPin(master_relay_gpio_handle.pGPIOx,
                        master_relay_gpio_handle.GPIO_Config.GPIO_PinNumber, HIGH);

    ACT1_STATUS = REVERSE;
    ACT2_STATUS = REVERSE;
    ACT_SPEED = 100;
    FRONT_PANEL_JIG_LED_OFF();
    FRONT_PANEL_TEST_LED_OFF();
    while (1){
        if(ACT1_STATUS == FORWARD){
            FRONT_PANEL_ACT1_FORWARD(ACT_SPEED);
            if (ADC_Read(&act_feedback_adc_handle, AD8) > 0xE665){
                FRONT_PANEL_ACT1_STOP();
                JIG_LED_STATUS = SOLID;
                FRONT_PANEL_JIG_LED_GREEN();
            }
        }
        else if(ACT1_STATUS == REVERSE){
            FRONT_PANEL_ACT1_REVERSE(ACT_SPEED);
            if (ADC_Read(&act_feedback_adc_handle, AD8) < 0x6B84){
                FRONT_PANEL_ACT1_STOP();
                JIG_LED_STATUS = SOLID;
                FRONT_PANEL_JIG_LED_GREEN();
            }
        }

        if(ACT2_STATUS == FORWARD){
            FRONT_PANEL_ACT2_FORWARD(ACT_SPEED);
            if (ADC_Read(&act_feedback_adc_handle, AD9) > 0xE665){
                FRONT_PANEL_ACT2_STOP();
                JIG_LED_STATUS = SOLID;
                FRONT_PANEL_JIG_LED_GREEN();
            }
        }
        else if(ACT2_STATUS == REVERSE){
            FRONT_PANEL_ACT2_REVERSE(ACT_SPEED);
            if (ADC_Read(&act_feedback_adc_handle, AD9) < 0x6B84){
                FRONT_PANEL_ACT2_STOP();
                JIG_LED_STATUS = SOLID;
                FRONT_PANEL_JIG_LED_GREEN();
            }
        }

        if (TEST_LED_STATUS == BLINK) {
            if ((get_tenth_seconds() - test_led_time_stamp) >= 3) {
                FRONT_PANEL_TEST_LED_BLINK_AMBER();
                test_led_time_stamp = get_tenth_seconds();
            }
        }

        if (JIG_LED_STATUS == BLINK) {
            if ((get_tenth_seconds() - jig_led_time_stamp) >= 3) {
                FRONT_PANEL_JIG_LED_BLINK_AMBER();
                jig_led_time_stamp = get_tenth_seconds();
            }
        }
    }
}

extern "C"{
void LPUART1_Handler() {
    LPUART_IRQHandling(&lpuart_handle);
    }
}

void LPUART_ApplicationEventCallback(pLPUART_Handle_t pLPUARTHandle, uint8_t app_event) {
    if (app_event == LPUART_BYTE_RCV_EVENT) {
        if (pLPUARTHandle->ByteReceived == '\r'){
            if(strlen(cmd_buffer) == 0) {
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t*) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "firmware") == 0){
                memset(cmd_buffer, 0, 256);
                LPUART_SendData(pLPUARTHandle, (uint8_t*) firmware_info_buffer, strlen(firmware_info_buffer));
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t*) display_buffer, strlen(display_buffer));
            }
            else if((cmd_buffer[0] + cmd_buffer[1]) == 152){ // forward
                uint8_t speed = 0;
                if (strlen(cmd_buffer) == 4)
                {
                    speed = (cmd_buffer[2] - 0x30) * 10 + (cmd_buffer[3] - 0x30);
                } else if (strlen(cmd_buffer) == 3){
                    speed = cmd_buffer[2] - 0x30;
                } else if (strlen(cmd_buffer) == 5){
                    speed = (cmd_buffer[2] - 0x30) * 100 + (cmd_buffer[3] - 0x30) * 10 +  (cmd_buffer[4] - 0x30);
                }
                memset(cmd_buffer, 0, 256);
                ACT_SPEED = speed;
                ACT1_STATUS = FORWARD;
                ACT2_STATUS = FORWARD;
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t*) display_buffer, strlen(display_buffer));
            }
            else if((cmd_buffer[0] + cmd_buffer[1]) == 154){ // reverse
                uint8_t speed = (cmd_buffer[2] - 0x30) * 10 + (cmd_buffer[3] - 0x30);
                if (strlen(cmd_buffer) == 4)
                {
                    speed = (cmd_buffer[2] - 0x30) * 10 + (cmd_buffer[3] - 0x30);
                } else if (strlen(cmd_buffer) == 3){
                    speed = cmd_buffer[2] - 0x30;
                } else if (strlen(cmd_buffer) == 5){
                    speed = (cmd_buffer[2] - 0x30) * 100 + (cmd_buffer[3] - 0x30) * 10 +  (cmd_buffer[4] - 0x30);
                }
                memset(cmd_buffer, 0, 256);
                ACT_SPEED = speed;
                ACT1_STATUS = REVERSE;
                ACT2_STATUS = REVERSE;
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t*) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "ms") == 0){
                memset(cmd_buffer, 0, 256);
                FRONT_PANEL_ACT1_STOP();
                FRONT_PANEL_ACT2_STOP();
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t*) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "led off") == 0) {
                memset(cmd_buffer, 0, 256);
                LED_OFF(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                        &TEST_LED_COLOR);
                LED_OFF(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                        &JIG_LED_COLOR);
                TEST_LED_STATUS = SOLID;
                JIG_LED_STATUS = SOLID;
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "test led green") == 0) {
                memset(cmd_buffer, 0, 256);
                TEST_LED_STATUS = SOLID;
                FRONT_PANEL_TEST_LED_GREEN();
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "test led red") == 0) {
                memset(cmd_buffer, 0, 256);
                TEST_LED_STATUS = SOLID;
                FRONT_PANEL_TEST_LED_RED();
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "test led amber") == 0) {
                memset(cmd_buffer, 0, 256);
                TEST_LED_STATUS = SOLID;
                FRONT_PANEL_TEST_LED_AMBER();
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "test led blink") == 0) {
                memset(cmd_buffer, 0, 256);
                TEST_LED_STATUS = BLINK;
                test_led_time_stamp = get_tenth_seconds();
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "test led off") == 0) {
                memset(cmd_buffer, 0, 256);
                TEST_LED_STATUS = SOLID;
                FRONT_PANEL_TEST_LED_OFF();
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "jig led green") == 0) {
                memset(cmd_buffer, 0, 256);
                JIG_LED_STATUS = SOLID;
                FRONT_PANEL_JIG_LED_GREEN();
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "jig led red") == 0) {
                memset(cmd_buffer, 0, 256);
                JIG_LED_STATUS = SOLID;
                FRONT_PANEL_JIG_LED_RED();
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "jig led amber") == 0) {
                memset(cmd_buffer, 0, 256);
                JIG_LED_STATUS = SOLID;
                FRONT_PANEL_JIG_LED_AMBER();
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "jig led blink") == 0) {
                memset(cmd_buffer, 0, 256);
                JIG_LED_STATUS = BLINK;
                jig_led_time_stamp = get_tenth_seconds();
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "jig led off") == 0) {
                memset(cmd_buffer, 0, 256);
                JIG_LED_STATUS = SOLID;
                FRONT_PANEL_JIG_LED_OFF();
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "act2 pos") == 0) {
                memset(cmd_buffer, 0, 256);
                uint16_t act1_pos_read;
                act1_pos_read = ADC_Read(&act_feedback_adc_handle, AD9);
                act1_pos_read = (3300 * act1_pos_read) / 65535;
                uint8_t whole_number = act1_pos_read / 1000;
                uint8_t fraction_first_digit = (act1_pos_read - whole_number * 1000) / 100;
                uint8_t fraction_second_digit = (act1_pos_read - whole_number * 1000 - fraction_first_digit * 100) / 10;
                uint8_t fraction_third_digit = (act1_pos_read - whole_number * 1000 - fraction_first_digit * 100 -
                                                fraction_second_digit * 10);
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendByte(pLPUARTHandle, whole_number + 0x30);
                LPUART_SendByte(pLPUARTHandle, 0x2E);
                LPUART_SendByte(pLPUARTHandle, fraction_first_digit + 0x30);
                LPUART_SendByte(pLPUARTHandle, fraction_second_digit + 0x30 );
                LPUART_SendByte(pLPUARTHandle, fraction_third_digit + 0x30);
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "act1 pos") == 0) {
                memset(cmd_buffer, 0, 256);
                uint16_t act1_pos_read;
                act1_pos_read = ADC_Read(&act_feedback_adc_handle, AD8);
                act1_pos_read = (3300 * act1_pos_read) / 65535;
                uint8_t whole_number = act1_pos_read / 1000;
                uint8_t fraction_first_digit = (act1_pos_read - whole_number * 1000) / 100;
                uint8_t fraction_second_digit = (act1_pos_read - whole_number * 1000 - fraction_first_digit * 100) / 10;
                uint8_t fraction_third_digit = (act1_pos_read - whole_number * 1000 - fraction_first_digit * 100 -
                                                fraction_second_digit * 10);
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendByte(pLPUARTHandle, whole_number + 0x30);
                LPUART_SendByte(pLPUARTHandle, 0x2E);
                LPUART_SendByte(pLPUARTHandle, fraction_first_digit + 0x30);
                LPUART_SendByte(pLPUARTHandle, fraction_second_digit + 0x30 );
                LPUART_SendByte(pLPUARTHandle, fraction_third_digit + 0x30);
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "latch on") == 0) {
                memset(cmd_buffer, 0, 256);
                GPIO_WriteOutputPin(latch_control_gpio_handle.pGPIOx,
                                    latch_control_gpio_handle.GPIO_Config.GPIO_PinNumber, HIGH);
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "latch off") == 0) {
                memset(cmd_buffer, 0, 256);
                GPIO_WriteOutputPin(latch_control_gpio_handle.pGPIOx,
                                    latch_control_gpio_handle.GPIO_Config.GPIO_PinNumber, LOW);
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "button red") == 0) {
                memset(cmd_buffer, 0, 256);
                FRONT_PANEL_START_RELEASE_BUTTON_RGB_CONTROL(COLOR_RED);
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "button green") == 0) {
                memset(cmd_buffer, 0, 256);
                FRONT_PANEL_START_RELEASE_BUTTON_RGB_CONTROL(COLOR_GREEN);
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "button blue") == 0) {
                memset(cmd_buffer, 0, 256);
                FRONT_PANEL_START_RELEASE_BUTTON_RGB_CONTROL(COLOR_BLUE);
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "button amber") == 0) {
                memset(cmd_buffer, 0, 256);
                FRONT_PANEL_START_RELEASE_BUTTON_RGB_CONTROL(COLOR_AMBER);
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "button off") == 0) {
                memset(cmd_buffer, 0, 256);
                FRONT_PANEL_START_RELEASE_BUTTON_RGB_CONTROL(COLOR_OFF);
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else{
                memset(cmd_buffer, 0, 256);
                LPUART_SendData(pLPUARTHandle, (uint8_t*) wrong_cmd_buffer, strlen(wrong_cmd_buffer));
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t*) display_buffer, strlen(display_buffer));
            }
        } else if(pLPUARTHandle->ByteReceived == '\b'){
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

extern "C"{
    void LPTMR0_Handler(){
        LPTMR->CSR |= (1 << LPTMR_CSR_TCF);
        tenth_seconds++;
    }
}

extern "C"{
    void PortCD_SingleInterrupt_Handler() {
        delay(100); // avoid debouncing
        if (PORTC->PORT_PCR[4] & (1 << 24)) {
            PORT_IRQHandling(PORTC, 4);
            if ((GPIOC->PDIR & 0x10) >> 4 == 1) {
                start_release_button_time_stamp = get_tenth_seconds();
                LPUART_SendData(&lpuart_handle, (uint8_t *) button_pressed_message, 16);
                FRONT_PANEL_START_RELEASE_BUTTON_RGB_CONTROL(COLOR_RED);

            } else if(((GPIOC->PDIR & 0x10)) >> 4 == 0){
                if((get_tenth_seconds() - start_release_button_time_stamp) <= 10) {
                    LPUART_SendData(&lpuart_handle, (uint8_t *) button_short_released_message, 16);
                    FRONT_PANEL_START_RELEASE_BUTTON_RGB_CONTROL(COLOR_GREEN);
                    ACT1_STATUS = FORWARD;
                    ACT2_STATUS = FORWARD;
                    ACT_SPEED = 100;
                    JIG_LED_STATUS = BLINK;
                } else {
                    LPUART_SendData(&lpuart_handle, (uint8_t *) button_long_released_message, 15);
                    FRONT_PANEL_START_RELEASE_BUTTON_RGB_CONTROL(COLOR_AMBER);
                    ACT1_STATUS = REVERSE;
                    ACT2_STATUS = REVERSE;
                    ACT_SPEED = 100;
                    JIG_LED_STATUS = BLINK;
                }

            }
        } else if(PORTC->PORT_PCR[21] & (1 << 24)){
            delay(100); // avoid debouncing
            PORT_IRQHandling(PORTC, 21);
            if (((GPIOC->PDIR & 0x200000) >> 21) == 1){
                LPUART_SendData(&lpuart_handle, (uint8_t *) button_reseat_message, 16);
                FRONT_PANEL_ACT1_STOP();
                FRONT_PANEL_ACT2_STOP();
            }
        }
    }
}