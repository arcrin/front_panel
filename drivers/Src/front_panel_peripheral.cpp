//
// Created by andy- on 2022-08-17.
//

#include "front_panel_peripheral.h"

PORT_Handle_t start_release_button_port_handle;
GPIO_Handle_t start_release_button_gpio_handle;

PORT_Handle_t reseat_button_port_handle;
GPIO_Handle_t reseat_button_gpio_handle;

PORT_Handle_t test_status_led_port_handle;
GPIO_Handle_t test_status_led_gpio_handle;

PORT_Handle_t act1_reverse_control_port_handle;
GPIO_Handle_t act1_reverse_control_gpio_handle;

PORT_Handle_t act1_forward_control_port_handle;
GPIO_Handle_t act1_forward_control_gpio_handle;

PORT_Handle_t act2_reverse_control_port_handle;
GPIO_Handle_t act2_reverse_control_gpio_handle;

PORT_Handle_t lpuart_port_handle;
LPUART_Handle_t lpuart_handle;

PORT_Handle_t act1_feedback_adc_port_handle;
ADC_Handle_t act1_feedback_adc_handle;


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
    start_release_button_port_handle.PORT_Config.PORT_Pin_Pull_Enable = ENABLE;
    start_release_button_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_UP;
    PORT_Init(&start_release_button_port_handle);

    start_release_button_gpio_handle.pGPIOx = GPIOC;
    start_release_button_gpio_handle.GPIO_Config.GPIO_PinNumber = 4;
    start_release_button_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_INPUT;
    GPIO_Init(&start_release_button_gpio_handle);

    IRQPriorityConfig(IRQ_NUMBER_PORTDC, 3);
    InterruptConfig(IRQ_NUMBER_PORTDC, ENABLE);
}

void FRONT_PANEL_RESEAT_BUTTON_INIT(){
    reseat_button_port_handle.pPORT = PORTC;
    reseat_button_port_handle.PORT_Config.PORT_Pin_Number = 21;
    reseat_button_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    reseat_button_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_PIN_ET;
    reseat_button_port_handle.PORT_Config.PORT_Pin_Pull_Enable = ENABLE;
    reseat_button_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_UP;
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
    act1_forward_control_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    act1_forward_control_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    act1_forward_control_port_handle.PORT_Config.PORT_Pin_Pull_Enable = DISABLE;
    PORT_Init(&act1_forward_control_port_handle);

    act1_forward_control_gpio_handle.pGPIOx = GPIOE;
    act1_forward_control_gpio_handle.GPIO_Config.GPIO_PinNumber = 20;
    act1_forward_control_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_OUTPUT;
    GPIO_Init(&act1_forward_control_gpio_handle);


    act1_reverse_control_port_handle.pPORT = PORTE;
    act1_reverse_control_port_handle.PORT_Config.PORT_Pin_Number = 21;
    act1_reverse_control_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    act1_reverse_control_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    act1_reverse_control_port_handle.PORT_Config.PORT_Pin_Pull_Enable = DISABLE;
    PORT_Init(&act1_reverse_control_port_handle);

    act1_reverse_control_gpio_handle.pGPIOx = GPIOE;
    act1_reverse_control_gpio_handle.GPIO_Config.GPIO_PinNumber = 21;
    act1_reverse_control_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_OUTPUT;
    GPIO_Init(&act1_reverse_control_gpio_handle);
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

    act1_feedback_adc_handle.pADCx = ADC0;
    // CFG1
    act1_feedback_adc_handle.LowPowerConfig = ADLPC_NORMAL;
    act1_feedback_adc_handle.ClockDivideSelect = ADIV_2;
    act1_feedback_adc_handle.SampleTimeConfig = ADLSMP_SHORT;
    act1_feedback_adc_handle.ConversionModeSelect = MODE_16;
    act1_feedback_adc_handle.InputClockSelect = ADICLK_BUS_2;

    // CFG2
    act1_feedback_adc_handle.AsyncClockOutputEnable = ADACKEN_DISABLED;
    act1_feedback_adc_handle.HighSpeedConfig = ADHSC_HISPEED;
    act1_feedback_adc_handle.LongSampleTimeSelect = ADLSTS_2;

    // SC2
    act1_feedback_adc_handle.ConversionTriggerSelect = ADTRG_SW;
    act1_feedback_adc_handle.CompareFunctionEnable = ACFE_DISABLED;
    act1_feedback_adc_handle.CompareFunctionRangeEnable = ACREN_DISABLED;
    act1_feedback_adc_handle.VoltageRefSelect = REFSEL_EXT;

    // SC3
    act1_feedback_adc_handle.ContinuousConversionEnable = ADCO_SINGLE;
    act1_feedback_adc_handle.HardwareAverageEnable = AVGE_DISABLED;
    act1_feedback_adc_handle.HardwareAverageSelect = AVGS_32;

    // SC1A
    act1_feedback_adc_handle.InterruptControlA = AIEN_OFF;
    act1_feedback_adc_handle.DifferentialModeA = DIFF_SINGLE;
    act1_feedback_adc_handle.InputChannelA = AD8;

    // SC1B
    act1_feedback_adc_handle.InterruptControlB = AIEN_OFF;
    act1_feedback_adc_handle.DifferentialModeB = DIFF_SINGLE;
    act1_feedback_adc_handle.InputChannelB = 0x1F;

    ADC_Init(&act1_feedback_adc_handle);
}

void FRONT_PANEL_ACT1_FORWARD(){
    GPIO_WriteOutputPin(act1_forward_control_gpio_handle.pGPIOx,
                        act1_forward_control_gpio_handle.GPIO_Config.GPIO_PinNumber, HIGH);
    GPIO_WriteOutputPin(act1_reverse_control_gpio_handle.pGPIOx,
                        act1_reverse_control_gpio_handle.GPIO_Config.GPIO_PinNumber, LOW);
}

void FRONT_PANEL_ACT1_REVERSE(){
    GPIO_WriteOutputPin(act1_forward_control_gpio_handle.pGPIOx,
                        act1_forward_control_gpio_handle.GPIO_Config.GPIO_PinNumber, LOW);
    GPIO_WriteOutputPin(act1_reverse_control_gpio_handle.pGPIOx,
                        act1_reverse_control_gpio_handle.GPIO_Config.GPIO_PinNumber, HIGH);
}

void FRONT_PANEL_ACT1_STOP(){

    GPIO_WriteOutputPin(act1_forward_control_gpio_handle.pGPIOx,
                        act1_forward_control_gpio_handle.GPIO_Config.GPIO_PinNumber, HIGH);
    GPIO_WriteOutputPin(act1_reverse_control_gpio_handle.pGPIOx,
                        act1_reverse_control_gpio_handle.GPIO_Config.GPIO_PinNumber, HIGH);
}