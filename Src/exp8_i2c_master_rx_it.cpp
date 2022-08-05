//
// Created by wbai on 8/3/2022.
//
#include "k32lb11.h"

#define my_address      0x68

I2C_Handle_t i2c0_handle;

uint8_t commandCode;

//uint8_t tx_buffer[] = {0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,};

uint8_t tx_buffer[] = "HiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHi...123A";

uint32_t data_len = 0;

void I2C0_Port_Pin_Init(){
    PORT_Handle_t i2c0_port_handle;
    i2c0_port_handle.pPORT = PORTB;
    i2c0_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION2;
    i2c0_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    i2c0_port_handle.PORT_Config.PORT_Pin_Pull_Enable = ENABLE;
    i2c0_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_UP;

    // SCL
    i2c0_port_handle.PORT_Config.PORT_Pin_Number = 0;
    PORT_Init(&i2c0_port_handle);

    // SDA
    i2c0_port_handle.PORT_Config.PORT_Pin_Number = 1;
    PORT_Init(&i2c0_port_handle);
}

void I2C0_Init(){
    i2c0_handle.pI2Cx = I2C0;
    i2c0_handle.I2C_Config.I2C_DeviceAddress = my_address;
    i2c0_handle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
    I2C_Init(&i2c0_handle);
}

int main(){
    data_len = sizeof(tx_buffer);
    DISABLE_IRQ();
    MCG_Init();
    SysTick_Init(800); // 0.1 ms per count down cycle
    I2C0_Port_Pin_Init();
    I2C0_Init();
    I2C_PeripheralControl(i2c0_handle.pI2Cx, ENABLE);
    I2C_InterruptControl(i2c0_handle.pI2Cx, ENABLE);
//    I2C_StartStopDetectionControl(i2c0_handle.pI2Cx, ENABLE);

    IRQPriorityConfig(IRQ_NUMBER_I2C0, 3);
    InterruptConfig(IRQ_NUMBER_I2C0, ENABLE);

    RESET_INTERRUPT();
    ENABLE_IRQ();
    while(1);
}

extern "C" {
    void I2C0_Handler(){
        I2C_IRQHandling(&i2c0_handle);
    }
}

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv){
    static uint32_t count = 0;
    static uint32_t w_ptr = 0;
    if (AppEv == I2C_EV_DATA_RCV) {
        commandCode = I2C_SlaveReceiveData(pI2CHandle->pI2Cx);
    }
    if (AppEv == I2C_EV_DATA_SEND) {
        if (commandCode == 0x51) {
            I2C_SlaveSendData(pI2CHandle->pI2Cx, ((data_len >> (count % 2) * 8)) & 0xff);
            count++; // TODO: need to reset this to zero at some point
        } else if (commandCode == 0x52) {
            I2C_SlaveSendData(pI2CHandle->pI2Cx, tx_buffer[w_ptr++]);
        }
    }
}
