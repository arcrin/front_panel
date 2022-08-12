//
// Created by wbai on 8/11/2022.
//
#include "k32lb11.h"

ADC_Handle_t adc_handle;

PORT_Handle_t green_led_port_handle;
GPIO_Handle_t green_led_gpio_handle;


void TestADCPortInit(){
    PORT_Handle_t adc_port_handle;
    adc_port_handle.pPORT = PORTE;
    adc_port_handle.PORT_Config.PORT_Pin_Function = PIN_DISABLED;
    adc_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    adc_port_handle.PORT_Config.PORT_Pin_Pull_Enable = DISABLE;
    adc_port_handle.PORT_Config.PORT_Pin_Number = 21;
    PORT_Init(&adc_port_handle);
}

void TestADCInit(){
    adc_handle.pADCx = ADC0;
    // CFG1
    adc_handle.LowPowerConfig = ADLPC_NORMAL;
    adc_handle.ClockDivideSelect = ADIV_2;
    adc_handle.SampleTimeConfig = ADLSMP_SHORT;
    adc_handle.ConversionModeSelect = MODE_16;
    adc_handle.InputClockSelect = ADICLK_BUS_2;

    // CFG2
    adc_handle.AsyncClockOutputEnable = ADACKEN_DISABLED;
    adc_handle.HighSpeedConfig = ADHSC_HISPEED;
    adc_handle.LongSampleTimeSelect = ADLSTS_2;

    // SC2
    adc_handle.ConversionTriggerSelect = ADTRG_SW;
    adc_handle.CompareFunctionEnable = ACFE_DISABLED;
    adc_handle.CompareFunctionRangeEnable = ACREN_DISABLED;
    adc_handle.VoltageRefSelect = REFSEL_EXT;

    // SC3
    adc_handle.ContinuousConversionEnable = ADCO_SINGLE;
    adc_handle.HardwareAverageEnable = AVGE_DISABLED;
    adc_handle.HardwareAverageSelect = AVGS_32;

    // SC1A
    adc_handle.InterruptControlA = AIEN_OFF;
    adc_handle.DifferentialModeA = DIFF_SINGLE;
    adc_handle.InputChannelA = 0x1F;

    // SC1B
    adc_handle.InterruptControlB = AIEN_OFF;
    adc_handle.DifferentialModeB = DIFF_SINGLE;
    adc_handle.InputChannelB = 0x1F;

    ADC_Init(&adc_handle);
}

int main(){
    uint16_t dummy_read;
    uint32_t tempreg;
    DISABLE_IRQ();
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
    MCG_Init();
    SysTick_Init(8000);
    TestADCPortInit();
    TestADCInit();
    RESET_INTERRUPT();
    ENABLE_IRQ();
    while(1){
        delay(1000);
        tempreg = adc_handle.pADCx->SC1A;
        tempreg &= ~(0x1F);
        adc_handle.pADCx->SC1A = tempreg;
        adc_handle.pADCx->SC1A = (tempreg | AD4);
        while (!(adc_handle.pADCx->SC1A & (1 << 7)));
        dummy_read = adc_handle.pADCx->RA & 0xFFFF;
        if (dummy_read > 0xFFF0) {
            GPIO_WriteOutputPin(green_led_gpio_handle.pGPIOx, 5, HIGH);
        } else{
            GPIO_WriteOutputPin(green_led_gpio_handle.pGPIOx, 5, LOW);
        }
    }
}