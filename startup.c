//
// Created by wbai on 4/25/2022.
//

#include <stdint.h>

#define SRAM_START      0x1FFFE000
#define SRAM_SIZE       (32 * 1024)
#define SRAM_END        ((SRAM_START) + (SRAM_SIZE))

#define STACK_START     SRAM_END

extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _la_data;
extern uint32_t _sbss;
extern uint32_t _ebss;

int main(void);

void __libc_init_array(void);

/*
 * ARM core system handler
 */
void Init_Program_Counter(void); // reset handler?
void NMI_Handler                    (void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler(void);
void SVC_Handler                    (void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler                (void) __attribute__ ((weak, alias("Default_Handler")));

/*
 * Non-core vectors
 */
void DMA0_Handler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Handler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Handler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA3_Handler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void FTFA_Handler                   (void) __attribute__ ((weak, alias("Default_Handler"))); // flash memory module
void PMC_LowVoltage_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void LLWU_Handler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void I2C0_Handler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void I2C1_Handler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void SPI0_Handler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void SPI1_Handler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void LPUART0_Handler                (void) __attribute__ ((weak, alias("Default_Handler")));
void LPUART1_Handler                (void) __attribute__ ((weak, alias("Default_Handler")));
void UART2_FlexIO_Handler           (void) __attribute__ ((weak, alias("Default_Handler")));
void ADC0_Hanlder                   (void) __attribute__ ((weak, alias("Default_Handler")));
void CMP0_Handler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void TPM0_Handler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void TPM1_Handler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void TPM2_Handler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void RTC_Alarm_Handler              (void) __attribute__ ((weak, alias("Default_Handler")));
void RTC_Sec_Handler                (void) __attribute__ ((weak, alias("Default_Handler")));
void PIT_Handler                    (void) __attribute__ ((weak, alias("Default_Handler")));
void USB_Handler                    (void) __attribute__ ((weak, alias("Default_Handler")));
void DAC0_Handler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void LPTMR0_Handler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void LCD_Handler                    (void) __attribute__ ((weak, alias("Default_Handler")));
void PortA_PinDetect_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void PortCD_SingleInterrupt_Handler (void) __attribute__ ((weak, alias("Default_Handler")));


uint32_t vectors[] __attribute__((section(".isr_vector"))) = {
        STACK_START,
        (uint32_t)Init_Program_Counter,
        (uint32_t)NMI_Handler,
        (uint32_t)HardFault_Handler,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        (uint32_t)SVC_Handler,
        0,
        0,
        (uint32_t)PendSV_Handler,
        (uint32_t)SysTick_Handler,
        (uint32_t)DMA0_Handler,
        (uint32_t)DMA1_Handler,
        (uint32_t)DMA2_Handler,
        (uint32_t)DMA3_Handler,
        0,
        (uint32_t)FTFA_Handler,
        (uint32_t)PMC_LowVoltage_Handler,
        (uint32_t)LLWU_Handler,
        (uint32_t)I2C0_Handler,
        (uint32_t)I2C1_Handler,
        (uint32_t)SPI0_Handler,
        (uint32_t)SPI1_Handler,
        (uint32_t)LPUART0_Handler,
        (uint32_t)LPUART1_Handler,
        (uint32_t)UART2_FlexIO_Handler,
        (uint32_t)ADC0_Hanlder,
        (uint32_t)CMP0_Handler,
        (uint32_t)TPM0_Handler,
        (uint32_t)TPM1_Handler,
        (uint32_t)TPM2_Handler,
        (uint32_t)RTC_Alarm_Handler,
        (uint32_t)RTC_Sec_Handler,
        (uint32_t)PIT_Handler,
        0,
        (uint32_t)USB_Handler,
        (uint32_t)DAC0_Handler,
        0,
        0,
        (uint32_t)LPTMR0_Handler,
        (uint32_t)LCD_Handler,
        (uint32_t)PortA_PinDetect_Handler,
        (uint32_t)PortCD_SingleInterrupt_Handler
};

void Default_Handler(void)
{
    while(1);
}

void HardFault_Handler(void)
{
    while(1);
}

void Init_Program_Counter(void){
    //copy .data section to SRAM
    uint32_t size = (uint32_t)&_edata - (uint32_t)&_sdata;

    uint8_t *pDst = (uint8_t*)&_sdata; //sram
    uint8_t *pSrc = (uint8_t*)&_la_data; //flash

    for(uint32_t i = 0; i < size; i++)
    {
        *pDst++ = *pSrc++;
    }

    //Init the .bss section to zero in SRAM
    size = (uint32_t)&_ebss - (uint32_t)&_sbss;
    pDst = (uint8_t*)&_sbss;
    for(uint32_t i = 0; i < size; i++)
    {
        *pDst++ = 0;
    }

    __libc_init_array();

    //call main()
    main();
}
