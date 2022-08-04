//
// Created by wbai on 7/14/2022.
//
#include "k32lb11.h"
#include <cstdint>

_vo uint32_t sysTick_count;

void MCG_Init(){
    uint8_t temp;
    temp = 0x40;
    MCG->C1 = temp;

    temp = 0x1;
    MCG->C2 = temp;
}

void SysTick_Init(uint32_t load_value){
    SysTick->CSR = 0UL;
    SysTick->RVR = load_value;
    SysTick->CVR = 0UL;
    SysTick->CSR = 7;
}

uint32_t get_tick(){
    return sysTick_count;
}

void delay(uint32_t delay_in_ms){
    uint32_t start_tick = get_tick();
    uint32_t wait = delay_in_ms;

    while((get_tick() - start_tick) < wait);
}

void InterruptConfig(uint8_t IRQNumber, uint8_t ENorDI){
    if (ENorDI == ENABLE) {
        *NVIC_ISER |= 1 << IRQNumber;
    } else if (ENorDI == DISABLE) {
        *NVIC_ICER |= 1 << IRQNumber;
    }
}

void IRQPriorityConfig(uint8_t IRQNumber, uint32_t priority){
    uint32_t register_offset = IRQNumber / 4;
    uint32_t byte_offset = IRQNumber % 4;
    // addresses are incremented by 4 bytes
    *(NVIC_IPR_BASEADDR + register_offset) |= (priority << (8 - NVIC_PRIORITY_BITS_IMPLEMENTED)) << (byte_offset * 8);
}

extern "C"{
    void SysTick_Handler(){
        sysTick_count++;
    }
}