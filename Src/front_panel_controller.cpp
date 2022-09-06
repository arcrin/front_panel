//
// Created by andy- on 2022-08-17.
//
#include "Inc/front_panel_peripheral.h"
#include <cstdint>
#include <cstdlib>

char display_buffer[15] = "Front Panel->";
char empty_buffer[256] = "                                                   ";
char wrong_cmd_buffer[19] = "\ncmd not supported";
char firmware_info_buffer[5] = "\n0.0";
//unsigned char label_image[5625];

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

extern GPIO_Handle_t limit_switch_feedback_gpio_handle;

extern GPIO_Handle_t dut_power_control_gpio_handle;

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

extern I2C_Handle_t i2c0_handle;

uint8_t i2c_command_code;

uint8_t i2c_tx_buffer[] = "HiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHi...123A";

uint8_t i2c_tx_data_len = 0;

uint8_t i2c_data_type = I2C_COMMAND;

uint32_t i2c_rgb_parameters = 0x0;

uint8_t label_image[5625];
uint32_t label_image_buffer_count = 0;

uint8_t JIG_STATUS;


uint32_t get_tenth_seconds(){
    return tenth_seconds;
}

void latch_on(){
    GPIO_WriteOutputPin(latch_control_gpio_handle.pGPIOx,
                        latch_control_gpio_handle.GPIO_Config.GPIO_PinNumber, HIGH);
}

void latch_off(){
    GPIO_WriteOutputPin(latch_control_gpio_handle.pGPIOx,
                        latch_control_gpio_handle.GPIO_Config.GPIO_PinNumber, LOW);
}

void master_relay_on() {
    GPIO_WriteOutputPin(master_relay_gpio_handle.pGPIOx,
                        master_relay_gpio_handle.GPIO_Config.GPIO_PinNumber, HIGH);
}

void master_relay_off() {
    GPIO_WriteOutputPin(master_relay_gpio_handle.pGPIOx,
                        master_relay_gpio_handle.GPIO_Config.GPIO_PinNumber, LOW);
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
    FRONT_PANEL_I2C_INT();
    LPTMR_Init();
//    EPD_GPIO_Init();
//    EPD_SPI_Init();
    RESET_INTERRUPT();
    ENABLE_IRQ();
    JIG_STATUS = READY;

    latch_on();
    master_relay_on();

    ACT1_STATUS = REVERSE;
    ACT2_STATUS = REVERSE;
    ACT_SPEED = 100;
    FRONT_PANEL_JIG_LED_OFF();
    FRONT_PANEL_TEST_LED_OFF();

//    EPD_2IN66_Init();
//    EPD_2IN66B_Clear();

//    uint8_t *BWImage, *RedImage;
//
//    uint16_t Imagesize =
//            ((EPD_2IN66B_WIDTH % 8 == 0) ? (EPD_2IN66B_WIDTH / 8) : (EPD_2IN66B_WIDTH / 8 + 1)) * EPD_2IN66B_HEIGHT;
//    if ((BWImage = (uint8_t *) malloc(Imagesize)) == NULL) {
//        return -1;
//    }
//    if ((RedImage = (uint8_t *) malloc(Imagesize)) == NULL) {
//        return -1;
//    }
//    Paint_NewImage(BWImage, EPD_2IN66B_WIDTH, EPD_2IN66B_HEIGHT, 270, WHITE);
//    Paint_NewImage(RedImage, EPD_2IN66B_WIDTH, EPD_2IN66B_HEIGHT, 270, WHITE);
//    Paint_Clear(WHITE);
//    EPD_2IN66B_Display(BWImage, RedImage);
//    delay(15000000);
//    EPD_2IN66B_Sleep();
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

        if (TEST_LED_STATUS == BLINK_AMBER) {
            if ((get_tenth_seconds() - test_led_time_stamp) >= 3) {
                FRONT_PANEL_TEST_LED_BLINK_AMBER();
                test_led_time_stamp = get_tenth_seconds();
            }
        }

        if (JIG_LED_STATUS == BLINK_AMBER) {
            if ((get_tenth_seconds() - jig_led_time_stamp) >= 3) {
                FRONT_PANEL_JIG_LED_BLINK_AMBER();
                jig_led_time_stamp = get_tenth_seconds();
            }
        }

        if (TEST_LED_STATUS == BLINK_RED) {
            if ((get_tenth_seconds() - test_led_time_stamp) >= 3) {
                FRONT_PANEL_TEST_LED_BLINK_RED();
                test_led_time_stamp = get_tenth_seconds();
            }
        }

        if (JIG_LED_STATUS == BLINK_RED) {
            if ((get_tenth_seconds() - jig_led_time_stamp) >= 3) {
                FRONT_PANEL_JIG_LED_BLINK_RED();
                jig_led_time_stamp = get_tenth_seconds();
            }
        }

        if (((i2c_rgb_parameters & 0xf0000) >> 16) == 0xf) {
            if (JIG_LED_COLOR != LED_COLOR_RED) {
                FRONT_PANEL_JIG_LED_RED();
            }
        }

        if (((i2c_rgb_parameters & 0xf000) >> 12) == 0xf) {
            if (JIG_LED_COLOR != LED_COLOR_GREEN) {
                FRONT_PANEL_JIG_LED_GREEN();
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
            else if(strcmp(cmd_buffer, "test led blink amber") == 0) {
                memset(cmd_buffer, 0, 256);
                TEST_LED_STATUS = BLINK_AMBER;
                test_led_time_stamp = get_tenth_seconds();
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "test led blink red") == 0) {
                memset(cmd_buffer, 0, 256);
                TEST_LED_STATUS = BLINK_RED;
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
            else if(strcmp(cmd_buffer, "jig led blink amber") == 0) {
                memset(cmd_buffer, 0, 256);
                JIG_LED_STATUS = BLINK_AMBER;
                jig_led_time_stamp = get_tenth_seconds();
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "jig led blink red") == 0) {
                memset(cmd_buffer, 0, 256);
                JIG_LED_STATUS = BLINK_RED;
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
                FRONT_PANEL_START_RELEASE_BUTTON_RGB_CONTROL(LED_COLOR_RED);
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "button green") == 0) {
                memset(cmd_buffer, 0, 256);
                FRONT_PANEL_START_RELEASE_BUTTON_RGB_CONTROL(LED_COLOR_GREEN);
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "button blue") == 0) {
                memset(cmd_buffer, 0, 256);
                FRONT_PANEL_START_RELEASE_BUTTON_RGB_CONTROL(LED_COLOR_BLUE);
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "button amber") == 0) {
                memset(cmd_buffer, 0, 256);
                FRONT_PANEL_START_RELEASE_BUTTON_RGB_CONTROL(LED_COLOR_AMBER);
                LPUART_SendByte(pLPUARTHandle, '\n');
                LPUART_SendData(pLPUARTHandle, (uint8_t *) display_buffer, strlen(display_buffer));
            }
            else if(strcmp(cmd_buffer, "button off") == 0) {
                memset(cmd_buffer, 0, 256);
                FRONT_PANEL_START_RELEASE_BUTTON_RGB_CONTROL(LED_COLOR_OFF);
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
                FRONT_PANEL_START_RELEASE_BUTTON_RGB_CONTROL(LED_COLOR_RED);

            } else if(((GPIOC->PDIR & 0x10)) >> 4 == 0){
                if((get_tenth_seconds() - start_release_button_time_stamp) <= 10) {
                    LPUART_SendData(&lpuart_handle, (uint8_t *) button_short_released_message, 16);
                    FRONT_PANEL_START_RELEASE_BUTTON_RGB_CONTROL(LED_COLOR_GREEN);
                    ACT1_STATUS = FORWARD;
                    ACT2_STATUS = FORWARD;
                    ACT_SPEED = 100;
                    JIG_LED_STATUS = BLINK_AMBER;
                } else {
                    LPUART_SendData(&lpuart_handle, (uint8_t *) button_long_released_message, 15);
                    FRONT_PANEL_START_RELEASE_BUTTON_RGB_CONTROL(LED_COLOR_AMBER);
                    ACT1_STATUS = REVERSE;
                    ACT2_STATUS = REVERSE;
                    ACT_SPEED = 100;
                    JIG_LED_STATUS = BLINK_AMBER;
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

extern "C" {
    void I2C0_Handler(){
        I2C_IRQHandling(&i2c0_handle);
    }
}

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t app_event){
    static uint32_t count = 0;
    static uint32_t word_pointer = 0;
    uint8_t temp;
    if (app_event == I2C_EV_DATA_STOP) {
        i2c_data_type = I2C_COMMAND;
    }
    if (app_event == I2C_EV_DATA_RCV) {
        if (i2c_data_type == I2C_COMMAND) {
            i2c_command_code = I2C_SlaveReceiveData(pI2CHandle->pI2Cx);
            if (i2c_command_code == 0x3) {
                ACT1_STATUS = FORWARD;
                ACT2_STATUS = FORWARD;
                ACT_SPEED = 100;
                JIG_LED_STATUS = BLINK_AMBER;
            } else if (i2c_command_code == 0x4) {
                ACT1_STATUS = REVERSE;
                ACT2_STATUS = REVERSE;
                ACT_SPEED = 100;
                JIG_LED_STATUS = BLINK_AMBER;
            } else if (i2c_command_code == 0x6) {
                i2c_data_type = I2C_PARAMETER;
            }
        }
        else if (i2c_data_type == I2C_PARAMETER) {
            if (i2c_command_code == 0x6) {
                i2c_rgb_parameters |= I2C_SlaveReceiveData(pI2CHandle->pI2Cx);
                i2c_rgb_parameters = i2c_rgb_parameters << 4;
            }
        }

        else if (i2c_data_type == I2C_DISPLAY_DATA) {
            // note: DAQ is sending an extra character each time, need to investigate
            if (label_image_buffer_count <= (sizeof(label_image) * 2)) {
                label_image[label_image_buffer_count / 2] = I2C_SlaveReceiveData(pI2CHandle->pI2Cx);
                label_image_buffer_count++;
            }
            if (label_image_buffer_count == (sizeof(label_image) * 2)) {
                label_image_buffer_count = 0;
                i2c_data_type = I2C_COMMAND;
            }
        }
    }

    if (app_event == I2C_EV_DATA_SEND) {
        if (i2c_command_code == 0x53) {
            I2C_SlaveSendData(pI2CHandle->pI2Cx, ((sizeof(i2c_tx_buffer) >> (count % 2) * 8)) & 0xff);
            count++;
        }
        else if (i2c_command_code == 0x52) {
            I2C_SlaveSendData(pI2CHandle->pI2Cx, i2c_tx_buffer[word_pointer++]);
        }
        else if (i2c_command_code == 0x51) {
            count = 0;
            word_pointer = 0;
            I2C_SlaveSendData(pI2CHandle->pI2Cx, 0x51);
        } else if (i2c_command_code == 0x1) {
            i2c_data_type = I2C_DISPLAY_DATA;
            I2C_SlaveSendData(pI2CHandle->pI2Cx, i2c_command_code);
        }
    }
}