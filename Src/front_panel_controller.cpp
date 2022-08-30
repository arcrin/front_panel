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

extern GPIO_Handle_t test_status_led_gpio_handle;

extern LPUART_Handle_t lpuart_handle;

extern ADC_Handle_t act_feedback_adc_handle;

extern GPIO_Handle_t latch_control_gpio_handle;

extern uint8_t TEST_LED_COLOR;
extern uint8_t JIG_LED_COLOR;

extern uint8_t ACT1_STATUS;
extern uint8_t ACT2_STATUS;

extern uint8_t ACT_SPEED;

uint8_t JIG_STATUS;



int main(){
    DISABLE_IRQ();
    MCG_Init();
    MCG->MC |= (1 << 7);
    SysTick_Init(480);
    FRONT_PANEL_LATCH_CONTROL_INIT();
    FRONT_PANEL_START_RELEASE_BUTTON_INIT();
    FRONT_PANEL_RESEAT_BUTTON_INIT();
    FRONT_PANEL_ACT1_CONTROL_INIT();
    FRONT_PANEL_ACT2_CONTROL_INIT();
    FRONT_PANEL_TEST_STATUS_LED_INIT();
    FRONT_PANEL_LPUART_INIT();
    FRONT_PANEL_ADC0_INIT();
    RTC_ENABLE();
    RTC->SR &= ~(1 << 4);
    RTC->TSR = 0x0;
    RTC->SR |= (1 << 4);
    ENABLE_IRQ();
    JIG_STATUS = READY;
    GPIO_WriteOutputPin(latch_control_gpio_handle.pGPIOx,
                        latch_control_gpio_handle.GPIO_Config.GPIO_PinNumber, HIGH);
    while (1){
        if(ACT1_STATUS == FORWARD){
            FRONT_PANEL_ACT1_FORWARD(ACT_SPEED);
            if (ADC_Read(&act_feedback_adc_handle, AD8) > 0xE665){
                FRONT_PANEL_ACT1_STOP();
            }
        }
        else if(ACT1_STATUS == REVERSE){
            FRONT_PANEL_ACT1_REVERSE(ACT_SPEED);
            if (ADC_Read(&act_feedback_adc_handle, AD8) < 0x6B84){
                FRONT_PANEL_ACT1_STOP();
            }
        }

        if(ACT2_STATUS == FORWARD){
            FRONT_PANEL_ACT2_FORWARD(ACT_SPEED);
            if (ADC_Read(&act_feedback_adc_handle, AD9) > 0xE665){
                FRONT_PANEL_ACT2_STOP();
            }
        }
        else if(ACT2_STATUS == REVERSE){
            FRONT_PANEL_ACT2_REVERSE(ACT_SPEED);
            if (ADC_Read(&act_feedback_adc_handle, AD9) < 0x6B84){
                FRONT_PANEL_ACT2_STOP();
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
            } else if(strcmp(cmd_buffer, "firmware") == 0){
                memset(cmd_buffer, 0, 256);
                LPUART_SendData(pLPUARTHandle, (uint8_t*) firmware_info_buffer, strlen(firmware_info_buffer));
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t*) display_buffer, strlen(display_buffer));
            } else if((cmd_buffer[0] + cmd_buffer[1]) == 152){ // forward
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
            } else if((cmd_buffer[0] + cmd_buffer[1]) == 154){ // reverse
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
            } else if(strcmp(cmd_buffer, "ms") == 0){
                memset(cmd_buffer, 0, 256);
                FRONT_PANEL_ACT1_STOP();
                FRONT_PANEL_ACT2_STOP();
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t*) display_buffer, strlen(display_buffer));
            } else if(strcmp(cmd_buffer, "led off") == 0) {
                memset(cmd_buffer, 0, 256);
                LED_OFF(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                        &TEST_LED_COLOR);
                LED_OFF(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                        &JIG_LED_COLOR);
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            } else if(strcmp(cmd_buffer, "test led green") == 0) {
                memset(cmd_buffer, 0, 256);
                LED_GREEN(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                          &TEST_LED_COLOR);
                if (JIG_LED_COLOR == COLOR_GREEN){
                    LED_GREEN(test_status_led_gpio_handle.pGPIOx,
                              test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber, &JIG_LED_COLOR);
                } else if (JIG_LED_COLOR == COLOR_RED) {
                    LED_RED(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &JIG_LED_COLOR);
                } else if (JIG_LED_COLOR == COLOR_AMBER) {
                    LED_RED(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &JIG_LED_COLOR);
                } else if (JIG_LED_COLOR == COLOR_OFF) {
                    LED_OFF(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &JIG_LED_COLOR);
                }
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            } else if(strcmp(cmd_buffer, "test led red") == 0) {
                memset(cmd_buffer, 0, 256);
                LED_RED(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                        &TEST_LED_COLOR);
                if (JIG_LED_COLOR == COLOR_GREEN){
                    LED_GREEN(test_status_led_gpio_handle.pGPIOx,
                              test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber, &JIG_LED_COLOR);
                } else if (JIG_LED_COLOR == COLOR_RED) {
                    LED_RED(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &JIG_LED_COLOR);
                } else if (JIG_LED_COLOR == COLOR_AMBER) {
                    LED_AMBER(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &JIG_LED_COLOR);
                } else if (JIG_LED_COLOR == COLOR_OFF) {
                    LED_OFF(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &JIG_LED_COLOR);
                }
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            } else if(strcmp(cmd_buffer, "test led amber") == 0) {
                memset(cmd_buffer, 0, 256);
                LED_AMBER(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                          &TEST_LED_COLOR);
                if (JIG_LED_COLOR == COLOR_GREEN){
                    LED_GREEN(test_status_led_gpio_handle.pGPIOx,
                              test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber, &JIG_LED_COLOR);
                } else if (JIG_LED_COLOR == COLOR_RED) {
                    LED_RED(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &JIG_LED_COLOR);
                } else if (JIG_LED_COLOR == COLOR_AMBER) {
                    LED_AMBER(test_status_led_gpio_handle.pGPIOx,
                              test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber, &JIG_LED_COLOR);
                } else if (JIG_LED_COLOR == COLOR_OFF) {
                    LED_OFF(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &JIG_LED_COLOR);
                }
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            } else if(strcmp(cmd_buffer, "test led off") == 0) {
                memset(cmd_buffer, 0, 256);
                LED_OFF(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                        &TEST_LED_COLOR);
                if (JIG_LED_COLOR == COLOR_GREEN){
                    LED_GREEN(test_status_led_gpio_handle.pGPIOx,
                              test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber, &JIG_LED_COLOR);
                } else if (JIG_LED_COLOR == COLOR_RED) {
                    LED_RED(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &JIG_LED_COLOR);
                }  else if (JIG_LED_COLOR == COLOR_AMBER) {
                    LED_AMBER(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &JIG_LED_COLOR);
                } else if (JIG_LED_COLOR == COLOR_OFF) {
                    LED_OFF(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &JIG_LED_COLOR);
                }
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            } else if(strcmp(cmd_buffer, "jig led green") == 0) {
                memset(cmd_buffer, 0, 256);
                if (TEST_LED_COLOR == COLOR_GREEN){
                    LED_GREEN(test_status_led_gpio_handle.pGPIOx,
                              test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber, &TEST_LED_COLOR);
                } else if (TEST_LED_COLOR == COLOR_RED) {
                    LED_RED(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &TEST_LED_COLOR);
                } else if (TEST_LED_COLOR == COLOR_AMBER) {
                    LED_AMBER(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &TEST_LED_COLOR);
                } else if (TEST_LED_COLOR == COLOR_OFF) {
                    LED_OFF(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &TEST_LED_COLOR);
                }
                LED_GREEN(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                          &JIG_LED_COLOR);
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            } else if(strcmp(cmd_buffer, "jig led red") == 0) {
                memset(cmd_buffer, 0, 256);
                if (TEST_LED_COLOR == COLOR_GREEN){
                    LED_GREEN(test_status_led_gpio_handle.pGPIOx,
                              test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber, &TEST_LED_COLOR);
                } else if (TEST_LED_COLOR == COLOR_RED) {
                    LED_RED(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &TEST_LED_COLOR);
                } else if (TEST_LED_COLOR == COLOR_AMBER) {
                    LED_AMBER(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &TEST_LED_COLOR);
                } else if (TEST_LED_COLOR == COLOR_OFF) {
                    LED_OFF(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &TEST_LED_COLOR);
                }
                LED_RED(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                        &JIG_LED_COLOR);
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }

            else if(strcmp(cmd_buffer, "jig led amber") == 0) {
                memset(cmd_buffer, 0, 256);
                if (TEST_LED_COLOR == COLOR_GREEN){
                    LED_GREEN(test_status_led_gpio_handle.pGPIOx,
                              test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber, &TEST_LED_COLOR);
                } else if (TEST_LED_COLOR == COLOR_RED) {
                    LED_RED(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &TEST_LED_COLOR);
                } else if (TEST_LED_COLOR == COLOR_AMBER) {
                    LED_AMBER(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &TEST_LED_COLOR);
                } else if (TEST_LED_COLOR == COLOR_OFF) {
                    LED_OFF(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &TEST_LED_COLOR);
                }
                LED_AMBER(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                        &JIG_LED_COLOR);
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }



            else if(strcmp(cmd_buffer, "jig led off") == 0) {
                memset(cmd_buffer, 0, 256);
                if (TEST_LED_COLOR == COLOR_GREEN){
                    LED_GREEN(test_status_led_gpio_handle.pGPIOx,
                              test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber, &TEST_LED_COLOR);
                } else if (TEST_LED_COLOR == COLOR_RED) {
                    LED_RED(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &TEST_LED_COLOR);
                } else if (TEST_LED_COLOR == COLOR_AMBER) {
                    LED_AMBER(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &TEST_LED_COLOR);
                } else if (TEST_LED_COLOR == COLOR_OFF) {
                    LED_OFF(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                            &TEST_LED_COLOR);
                }
                LED_OFF(test_status_led_gpio_handle.pGPIOx, test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber,
                        &JIG_LED_COLOR);
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