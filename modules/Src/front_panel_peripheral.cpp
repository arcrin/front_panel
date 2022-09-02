//
// Created by andy- on 2022-08-17.
//

#include "Inc/front_panel_peripheral.h"

PORT_Handle_t start_release_button_port_handle;
GPIO_Handle_t start_release_button_gpio_handle;

PORT_Handle_t latch_control_port_handle;
GPIO_Handle_t latch_control_gpio_handle;

PORT_Handle_t master_relay_port_handle;
GPIO_Handle_t master_relay_gpio_handle;

PORT_Handle_t reseat_button_port_handle;
GPIO_Handle_t reseat_button_gpio_handle;

PORT_Handle_t test_status_led_port_handle;
GPIO_Handle_t test_status_led_gpio_handle;

PORT_Handle_t act1_forward_control_port_handle;

PORT_Handle_t act1_reverse_control_port_handle;

PORT_Handle_t act2_forward_control_port_handle;

PORT_Handle_t act2_reverse_control_port_handle;

PORT_Handle_t lpuart_port_handle;
LPUART_Handle_t lpuart_handle;

PORT_Handle_t act1_feedback_adc_port_handle;
ADC_Handle_t act_feedback_adc_handle;

uint8_t TEST_LED_COLOR = COLOR_OFF;
uint8_t JIG_LED_COLOR = COLOR_OFF;

uint8_t TEST_LED_STATUS = SOLID;
uint8_t JIG_LED_STATUS = SOLID;

uint8_t ACT1_STATUS = STOP;
uint8_t ACT2_STATUS = STOP;

PORT_Handle_t start_release_button_red_port_handle;
GPIO_Handle_t start_release_button_red_gpio_handle;

PORT_Handle_t start_release_button_green_port_handle;
GPIO_Handle_t start_release_button_green_gpio_handle;

PORT_Handle_t start_release_button_blue_port_handle;
GPIO_Handle_t start_release_button_blue_gpio_handle;


uint8_t ACT_SPEED = 0;



void FRONT_PANEL_TEST_STATUS_LED_INIT(){
    // PTB16
    test_status_led_port_handle.pPORT = PORTB;
    test_status_led_port_handle.PORT_Config.PORT_Pin_Number = 16;
    test_status_led_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    test_status_led_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    test_status_led_port_handle.PORT_Config.PORT_Pin_Pull_Enable = ENABLE;
    test_status_led_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_DOWN;
    PORT_Init(&test_status_led_port_handle);

    test_status_led_gpio_handle.pGPIOx = GPIOB;
    test_status_led_gpio_handle.GPIO_Config.GPIO_PinNumber = 16;
    test_status_led_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_OUTPUT;
    GPIO_Init(&test_status_led_gpio_handle);
}

void FRONT_PANEL_START_RELEASE_BUTTON_INIT(){
    start_release_button_port_handle.pPORT = PORTC;
    start_release_button_port_handle.PORT_Config.PORT_Pin_Number = 4;
    start_release_button_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    start_release_button_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_PIN_ET;
    start_release_button_port_handle.PORT_Config.PORT_Pin_Pull_Enable = DISABLE;
    PORT_Init(&start_release_button_port_handle);

    start_release_button_gpio_handle.pGPIOx = GPIOC;
    start_release_button_gpio_handle.GPIO_Config.GPIO_PinNumber = 4;
    start_release_button_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_INPUT;
    GPIO_Init(&start_release_button_gpio_handle);

    IRQPriorityConfig(IRQ_NUMBER_PORTDC, 3);
    InterruptConfig(IRQ_NUMBER_PORTDC, ENABLE);
}

void FRONT_PANEL_LATCH_CONTROL_INIT(){
    latch_control_port_handle.pPORT = PORTC;
    latch_control_port_handle.PORT_Config.PORT_Pin_Number = 23;
    latch_control_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    latch_control_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    latch_control_port_handle.PORT_Config.PORT_Pin_Pull_Enable = DISABLE;
    PORT_Init(&latch_control_port_handle);

    latch_control_gpio_handle.pGPIOx = GPIOC;
    latch_control_gpio_handle.GPIO_Config.GPIO_PinNumber = 23;
    latch_control_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_OUTPUT;
    GPIO_Init(&latch_control_gpio_handle);
}

void FRONT_PANEL_MASTER_RELAY_INIT(){
    master_relay_port_handle.pPORT = PORTB;
    master_relay_port_handle.PORT_Config.PORT_Pin_Number = 18;
    master_relay_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    master_relay_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    master_relay_port_handle.PORT_Config.PORT_Pin_Pull_Enable = DISABLE;
    PORT_Init(&master_relay_port_handle);

    master_relay_gpio_handle.pGPIOx = GPIOB;
    master_relay_gpio_handle.GPIO_Config.GPIO_PinNumber = 18;
    master_relay_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_OUTPUT;
    GPIO_Init(&master_relay_gpio_handle);
}


void FRONT_PANEL_RESEAT_BUTTON_INIT(){
    reseat_button_port_handle.pPORT = PORTC;
    reseat_button_port_handle.PORT_Config.PORT_Pin_Number = 21;
    reseat_button_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    reseat_button_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_PIN_RT;
    reseat_button_port_handle.PORT_Config.PORT_Pin_Pull_Enable = DISABLE;
    PORT_Init(&reseat_button_port_handle);

    reseat_button_gpio_handle.pGPIOx = GPIOC;
    reseat_button_gpio_handle.GPIO_Config.GPIO_PinNumber = 21;
    reseat_button_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_INPUT;
    GPIO_Init(&reseat_button_gpio_handle);

    IRQPriorityConfig(IRQ_NUMBER_PORTDC, 3);
    InterruptConfig(IRQ_NUMBER_PORTDC, ENABLE);
}

void FRONT_PANEL_ACT1_CONTROL_INIT(){
    act1_forward_control_port_handle.pPORT = PORTE;
    act1_forward_control_port_handle.PORT_Config.PORT_Pin_Number = 20;
    act1_forward_control_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION3;
    act1_forward_control_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    act1_forward_control_port_handle.PORT_Config.PORT_Pin_Pull_Enable = DISABLE;
    PORT_Init(&act1_forward_control_port_handle);

    act1_reverse_control_port_handle.pPORT = PORTE;
    act1_reverse_control_port_handle.PORT_Config.PORT_Pin_Number = 21;
    act1_reverse_control_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION3;
    act1_reverse_control_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    act1_reverse_control_port_handle.PORT_Config.PORT_Pin_Pull_Enable = DISABLE;
    PORT_Init(&act1_reverse_control_port_handle);

    TPM1_Init();
}

void FRONT_PANEL_ACT2_CONTROL_INIT(){
    act2_forward_control_port_handle.pPORT = PORTA;
    act2_forward_control_port_handle.PORT_Config.PORT_Pin_Number = 2;
    act2_forward_control_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION3;
    act2_forward_control_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    act2_forward_control_port_handle.PORT_Config.PORT_Pin_Pull_Enable = DISABLE;
    PORT_Init(&act2_forward_control_port_handle);

    act2_reverse_control_port_handle.pPORT = PORTA;
    act2_reverse_control_port_handle.PORT_Config.PORT_Pin_Number = 1;
    act2_reverse_control_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION3;
    act2_reverse_control_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    act2_reverse_control_port_handle.PORT_Config.PORT_Pin_Pull_Enable = DISABLE;
    PORT_Init(&act2_reverse_control_port_handle);

    TPM2_Init();
}

void FRONT_PANEL_LPUART_INIT(){
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

    lpuart_handle.pLPUARTx = LPUART1;
    lpuart_handle.LPUART_Config.LPUART_Mode = LPUART_TXRX_MODE;
    lpuart_handle.LPUART_Config.LPUART_WordLength = LPUART_WORDLEN_8BITS;
    lpuart_handle.LPUART_Config.LPUART_ParityControl = LPUART_PARITY_DISABLE;
    lpuart_handle.RxLen = 0;
    lpuart_handle.TxLen = 0;
    LPUART_Init(&lpuart_handle);
    lpuart_handle.pLPUARTx->CTRL |= (1 << LPUART_CTRL_RIE);
    lpuart_handle.pLPUARTx->CTRL &= ~(1 << LPUART_CTRL_TIE);
    lpuart_handle.pLPUARTx->CTRL &= ~(1 << LPUART_CTRL_TCIE);

    IRQPriorityConfig(IRQ_NUMBER_LPUART1, 3);
    InterruptConfig(IRQ_NUMBER_LPUART1, ENABLE);
}

void FRONT_PANEL_ADC0_INIT(){
    act1_feedback_adc_port_handle.pPORT = PORTB;
    act1_feedback_adc_port_handle.PORT_Config.PORT_Pin_Function = PIN_DISABLED;
    act1_feedback_adc_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    act1_feedback_adc_port_handle.PORT_Config.PORT_Pin_Pull_Enable = DISABLE;
    act1_feedback_adc_port_handle.PORT_Config.PORT_Pin_Number = 0;
    PORT_Init(&act1_feedback_adc_port_handle);

    act_feedback_adc_handle.pADCx = ADC0;
    // CFG1
    act_feedback_adc_handle.LowPowerConfig = ADLPC_NORMAL;
    act_feedback_adc_handle.ClockDivideSelect = ADIV_2;
    act_feedback_adc_handle.SampleTimeConfig = ADLSMP_SHORT;
    act_feedback_adc_handle.ConversionModeSelect = MODE_16;
    act_feedback_adc_handle.InputClockSelect = ADICLK_BUS_2;

    // CFG2
    act_feedback_adc_handle.AsyncClockOutputEnable = ADACKEN_DISABLED;
    act_feedback_adc_handle.HighSpeedConfig = ADHSC_HISPEED;
    act_feedback_adc_handle.LongSampleTimeSelect = ADLSTS_2;

    // SC2
    act_feedback_adc_handle.CompareFunctionEnable = ACFE_DISABLED;
    act_feedback_adc_handle.ConversionTriggerSelect = ADTRG_SW;
    act_feedback_adc_handle.CompareFunctionGTEnable = ACFGT_GREATER;
    act_feedback_adc_handle.CompareFunctionRangeEnable = ACREN_DISABLED;
    act_feedback_adc_handle.VoltageRefSelect = REFSEL_EXT;

    // SC3
    act_feedback_adc_handle.ContinuousConversionEnable = ADCO_SINGLE;
    act_feedback_adc_handle.HardwareAverageEnable = AVGE_DISABLED;
    act_feedback_adc_handle.HardwareAverageSelect = AVGS_32;

    // SC1A
    act_feedback_adc_handle.InterruptControlA = AIEN_OFF;
    act_feedback_adc_handle.DifferentialModeA = DIFF_SINGLE;
    act_feedback_adc_handle.InputChannelA = 0x1F;

    // SC1B
    act_feedback_adc_handle.InterruptControlB = AIEN_OFF;
    act_feedback_adc_handle.DifferentialModeB = DIFF_SINGLE;
    act_feedback_adc_handle.InputChannelB = 0x1F;

    // CV
    act_feedback_adc_handle.CompareValue = 0x7FFF;

    ADC_Init(&act_feedback_adc_handle);
}

void FRONT_PANEL_ACT1_FORWARD(uint8_t speed){
    TPM1->SC &= ~(0x3 << TPM_SC_CMOD); // Disable TPM counter, this is universal for the entire TMP1 module
    uint16_t max_speed = TPM1->MOD;
    TPM1->CNT = 0xFFFF; // clear Counter register to avoid confusion of cycle start
    // configure the forward control channel with new speed (PWM duty cycle)
    uint16_t new_speed = max_speed * speed / 100;
    TPM1->C0V = new_speed;

    // backward control channel stays low
    TPM1->C1V = (uint16_t) 0x0;

    ACT1_STATUS = FORWARD;

    TPM1->SC |= (0x1 << TPM_SC_CMOD); // enable TPM counter
}

void FRONT_PANEL_ACT1_REVERSE(uint8_t speed){
    TPM1->SC &= ~(0x3 << TPM_SC_CMOD); // Disable TPM counter, this is universal for the entire TMP1 module
    uint16_t max_speed = TPM1->MOD;
    TPM1->CNT = 0xFFFF; // clear Counter register to avoid confusion of cycle start
    // configure the forward control channel with new speed (PWM duty cycle)
    uint16_t new_speed = max_speed * speed / 100;
    TPM1->C1V = new_speed;

    // backward control channel stays low
    TPM1->C0V = (uint16_t) 0x0;

    ACT1_STATUS = REVERSE;

    TPM1->SC |= (0x1 << TPM_SC_CMOD); // enable TPM counter
}

void FRONT_PANEL_ACT1_STOP(){
    TPM1->SC &= ~(0x3 << TPM_SC_CMOD);
    ACT1_STATUS = STOP;
}


void FRONT_PANEL_ACT2_REVERSE(uint8_t speed){
    TPM2->SC &= ~(0x3 << TPM_SC_CMOD); // Disable TPM counter, this is universal for the entire TMP1 module
    uint16_t max_speed = TPM2->MOD;
    TPM2->CNT = 0xFFFF; // clear Counter register to avoid confusion of cycle start
    // configure the forward control channel with new speed (PWM duty cycle)
    uint16_t new_speed = max_speed * speed / 100;
    TPM2->C0V = new_speed;

    // backward control channel stays low
    TPM2->C1V = (uint16_t) 0x0;

    ACT2_STATUS = REVERSE;

    TPM2->SC |= (0x1 << TPM_SC_CMOD); // enable TPM counter
}

void FRONT_PANEL_ACT2_FORWARD(uint8_t speed){
    TPM2->SC &= ~(0x3 << TPM_SC_CMOD); // Disable TPM counter, this is universal for the entire TMP1 module
    uint16_t max_speed = TPM2->MOD;
    TPM2->CNT = 0xFFFF; // clear Counter register to avoid confusion of cycle start
    // configure the forward control channel with new speed (PWM duty cycle)
    uint16_t new_speed = max_speed * speed / 100;
    TPM2->C1V = new_speed;

    // backward control channel stays low
    TPM2->C0V = (uint16_t) 0x0;

    ACT2_STATUS = FORWARD;

    TPM2->SC |= (0x1 << TPM_SC_CMOD); // enable TPM counter
}

void FRONT_PANEL_ACT2_STOP(){
    TPM2->SC &= ~(0x3 << TPM_SC_CMOD);
    ACT2_STATUS = STOP;
}

void FRONT_PANEL_TEST_LED_GREEN(){
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
}

void FRONT_PANEL_TEST_LED_RED(){
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
}

void FRONT_PANEL_TEST_LED_AMBER(){
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
}

void FRONT_PANEL_TEST_LED_BLINK_AMBER(){
    if (TEST_LED_COLOR != COLOR_AMBER){
        FRONT_PANEL_TEST_LED_AMBER();
    } else if (TEST_LED_COLOR == COLOR_AMBER) {
        FRONT_PANEL_TEST_LED_OFF();
    }
}

void FRONT_PANEL_TEST_LED_OFF(){
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
}

void FRONT_PANEL_JIG_LED_GREEN(){
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
}

void FRONT_PANEL_JIG_LED_RED(){
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
}

void FRONT_PANEL_JIG_LED_AMBER(){
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
}

void FRONT_PANEL_JIG_LED_BLINK_AMBER(){
    if (JIG_LED_COLOR != COLOR_AMBER){
        FRONT_PANEL_JIG_LED_AMBER();
    } else if (JIG_LED_COLOR == COLOR_AMBER) {
        FRONT_PANEL_JIG_LED_OFF();
    }
}

void FRONT_PANEL_JIG_LED_OFF(){
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
}

void FRONT_PANEL_BUTTON_RGB_INIT(){
    start_release_button_red_port_handle.pPORT = PORTC;
    start_release_button_red_port_handle.PORT_Config.PORT_Pin_Number = 3;
    start_release_button_red_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    start_release_button_red_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    start_release_button_red_port_handle.PORT_Config.PORT_Pin_Pull_Enable = DISABLE;
    PORT_Init(&start_release_button_red_port_handle);

    start_release_button_red_gpio_handle.pGPIOx = GPIOC;
    start_release_button_red_gpio_handle.GPIO_Config.GPIO_PinNumber = 3;
    start_release_button_red_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_OUTPUT;
    GPIO_Init(&start_release_button_red_gpio_handle);

    start_release_button_green_port_handle.pPORT = PORTC;
    start_release_button_green_port_handle.PORT_Config.PORT_Pin_Number = 2;
    start_release_button_green_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    start_release_button_green_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    start_release_button_green_port_handle.PORT_Config.PORT_Pin_Pull_Enable = DISABLE;
    PORT_Init(&start_release_button_green_port_handle);

    start_release_button_green_gpio_handle.pGPIOx = GPIOC;
    start_release_button_green_gpio_handle.GPIO_Config.GPIO_PinNumber = 2;
    start_release_button_green_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_OUTPUT;
    GPIO_Init(&start_release_button_green_gpio_handle);

    start_release_button_blue_port_handle.pPORT = PORTC;
    start_release_button_blue_port_handle.PORT_Config.PORT_Pin_Number = 1;
    start_release_button_blue_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    start_release_button_blue_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    start_release_button_blue_port_handle.PORT_Config.PORT_Pin_Pull_Enable = DISABLE;
    PORT_Init(&start_release_button_blue_port_handle);

    start_release_button_blue_gpio_handle.pGPIOx = GPIOC;
    start_release_button_blue_gpio_handle.GPIO_Config.GPIO_PinNumber = 1;
    start_release_button_blue_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_OUTPUT;
    GPIO_Init(&start_release_button_blue_gpio_handle);
}

void FRONT_PANEL_START_RELEASE_BUTTON_RGB_CONTROL(uint8_t color){
    if (color == COLOR_RED) {
        GPIO_WriteOutputPin(start_release_button_red_gpio_handle.pGPIOx,
                            start_release_button_red_gpio_handle.GPIO_Config.GPIO_PinNumber, HIGH);
        GPIO_WriteOutputPin(start_release_button_blue_gpio_handle.pGPIOx,
                            start_release_button_blue_gpio_handle.GPIO_Config.GPIO_PinNumber, LOW);
        GPIO_WriteOutputPin(start_release_button_green_gpio_handle.pGPIOx,
                            start_release_button_green_gpio_handle.GPIO_Config.GPIO_PinNumber, LOW);
    } else if (color == COLOR_GREEN) {
        GPIO_WriteOutputPin(start_release_button_green_gpio_handle.pGPIOx,
                            start_release_button_green_gpio_handle.GPIO_Config.GPIO_PinNumber, HIGH);
        GPIO_WriteOutputPin(start_release_button_blue_gpio_handle.pGPIOx,
                            start_release_button_blue_gpio_handle.GPIO_Config.GPIO_PinNumber, LOW);
        GPIO_WriteOutputPin(start_release_button_red_gpio_handle.pGPIOx,
                            start_release_button_red_gpio_handle.GPIO_Config.GPIO_PinNumber, LOW);
    } else if (color == COLOR_BLUE) {
        GPIO_WriteOutputPin(start_release_button_blue_gpio_handle.pGPIOx,
                            start_release_button_blue_gpio_handle.GPIO_Config.GPIO_PinNumber, HIGH);
        GPIO_WriteOutputPin(start_release_button_green_gpio_handle.pGPIOx,
                            start_release_button_green_gpio_handle.GPIO_Config.GPIO_PinNumber, LOW);
        GPIO_WriteOutputPin(start_release_button_red_gpio_handle.pGPIOx,
                            start_release_button_red_gpio_handle.GPIO_Config.GPIO_PinNumber, LOW);
    }
    else if (color == COLOR_AMBER) {
        GPIO_WriteOutputPin(start_release_button_blue_gpio_handle.pGPIOx,
                            start_release_button_blue_gpio_handle.GPIO_Config.GPIO_PinNumber, LOW);
        GPIO_WriteOutputPin(start_release_button_green_gpio_handle.pGPIOx,
                            start_release_button_green_gpio_handle.GPIO_Config.GPIO_PinNumber, HIGH);
        GPIO_WriteOutputPin(start_release_button_red_gpio_handle.pGPIOx,
                            start_release_button_red_gpio_handle.GPIO_Config.GPIO_PinNumber, HIGH);
    }
    else if (color == COLOR_OFF) {
        GPIO_WriteOutputPin(start_release_button_blue_gpio_handle.pGPIOx,
                            start_release_button_blue_gpio_handle.GPIO_Config.GPIO_PinNumber, LOW);
        GPIO_WriteOutputPin(start_release_button_green_gpio_handle.pGPIOx,
                            start_release_button_green_gpio_handle.GPIO_Config.GPIO_PinNumber, LOW);
        GPIO_WriteOutputPin(start_release_button_red_gpio_handle.pGPIOx,
                            start_release_button_red_gpio_handle.GPIO_Config.GPIO_PinNumber, LOW);
    }
}
