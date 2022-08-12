//
// Created by wbai on 8/8/2022.
//

#include "epaper.h"
#include "k32lb11.h"

//const unsigned char WF_PARTIAL[159] ={
//        0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//        0x00,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
//        0x00,0x00,0x00,0x00,0x40,0x40,0x00,0x00,0x00,0x00,
//        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,
//        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//        0x0A,0x00,0x00,0x00,0x00,0x00,0x02,0x01,0x00,0x00,
//        0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,
//        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//        0x00,0x00,0x00,0x00,0x22,0x22,0x22,0x22,0x22,0x22,
//        0x00,0x00,0x00,0x22,0x17,0x41,0xB0,0x32,0x36,
//};


void EPD_GPIO_Init(){
    PORT_Handle_t epd_port_handle;


    epd_port_handle.pPORT = PORTE;
    epd_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION1;
    epd_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    epd_port_handle.PORT_Config.PORT_Pin_Pull_Enable = DISABLE;

    GPIO_Handle_t epd_gpio_handle;
    epd_gpio_handle.pGPIOx = GPIOE;
    epd_gpio_handle.GPIO_Config.GPIO_PinDirection = GPIO_OUTPUT;

    // RST pin
    epd_port_handle.PORT_Config.PORT_Pin_Number = 20;
    epd_gpio_handle.GPIO_Config.GPIO_PinNumber = 20;
    PORT_Init(&epd_port_handle);
    GPIO_Init(&epd_gpio_handle);

    // DC Pin
    epd_port_handle.PORT_Config.PORT_Pin_Number = 21;
    epd_gpio_handle.GPIO_Config.GPIO_PinNumber = 21;
    PORT_Init(&epd_port_handle);
    GPIO_Init(&epd_gpio_handle);


    // CS Pin
    epd_port_handle.PORT_Config.PORT_Pin_Number = 23;
    epd_gpio_handle.GPIO_Config.GPIO_PinNumber = 23;
    PORT_Init(&epd_port_handle);
    GPIO_Init(&epd_gpio_handle);

    // BUSY Pin
    epd_port_handle.PORT_Config.PORT_Pin_Number = 22;
    epd_gpio_handle.GPIO_Config.GPIO_PinNumber = 22;
    PORT_Init(&epd_port_handle);
    GPIO_Init(&epd_gpio_handle);
}


void EPD_SPI_Init(){
    PORT_Handle_t epd_spi_port_handle;
    epd_spi_port_handle.pPORT = PORTC;
    epd_spi_port_handle.PORT_Config.PORT_Pin_Function = ALT_FUNCTION2;
    epd_spi_port_handle.PORT_Config.PORT_Pin_Interrupt_cfg = ISF_DISABLE;
    epd_spi_port_handle.PORT_Config.PORT_Pin_Pull_Enable = ENABLE;
    epd_spi_port_handle.PORT_Config.PORT_Pin_Pull_Select = PULL_UP;

    // SCK pin
    epd_spi_port_handle.PORT_Config.PORT_Pin_Number = 5;
    PORT_Init(&epd_spi_port_handle);

    // MOSI pin
    epd_spi_port_handle.PORT_Config.PORT_Pin_Number = 6;
    PORT_Init(&epd_spi_port_handle);

    SPI_Handle_t epd_spi_handle;
    epd_spi_handle.pSPIx = SPI0;
    epd_spi_handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MASTER_MODE;
    epd_spi_handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
    epd_spi_handle.SPIConfig.SPI_BR_PRE = SPI_SCLK_PRE_1;
    epd_spi_handle.SPIConfig.SPI_BR_DIV = SPI_SCLK_SPEED_DIV2;
    epd_spi_handle.SPIConfig.SPI_MODE = SPI_8BIT_MODE;
    epd_spi_handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
    epd_spi_handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
    SPI_Init(&epd_spi_handle);
}

static void EPD_2IN66_Reset(){
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(2);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
}

static void EPD_2IN66_SendCommand(uint8_t Reg){
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Reg);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

static void EPD_2IN66_SendData(uint8_t Data){
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

static void EPD_2IN66_ReadBusy(){
    DEV_Delay_ms(100);
    while (DEV_Digital_Read(EPD_BUSY_PIN) == 1) {
        DEV_Delay_ms(100);
    }
    DEV_Delay_ms(100);
}

static void EPD_2IN66_TurnOnDisplay(){
    EPD_2IN66_SendCommand(0x20);
    EPD_2IN66_ReadBusy();
}

//static void EPD_2IN66_SetLUA(){
//    uint16_t count;
//    EPD_2IN66_SendCommand(0x32);
//    for (count = 0; count < 153; count++) {
//        EPD_2IN66_SendData(WF_PARTIAL[count]);
//    }
//    EPD_2IN66_ReadBusy();
//}

void EPD_2IN66B_Init(){
    EPD_2IN66_Reset();
    EPD_2IN66_ReadBusy();
    EPD_2IN66_SendCommand(0x12); // soft reset
    EPD_2IN66_ReadBusy();

    EPD_2IN66_SendCommand(0x11); // data entry mode
    EPD_2IN66_SendData(0x03);
    /* Set RamX-address Start/End position*/
    EPD_2IN66_SendCommand(0x44);
    EPD_2IN66_SendData(0x1);
    EPD_2IN66_SendData((EPD_2IN66_WIDTH % 8 == 0) ? (EPD_2IN66_WIDTH / 8) : (EPD_2IN66_WIDTH / 8 + 1));
    /* Set RamY-address Start/End position*/
    EPD_2IN66_SendCommand(0x45);
    EPD_2IN66_SendData(0);
    EPD_2IN66_SendData(0);
    EPD_2IN66_SendData((EPD_2IN66_HEIGHT & 0xFF));
    EPD_2IN66_SendData((EPD_2IN66_HEIGHT & 0x100) >> 8);

    EPD_2IN66_ReadBusy();
}

//void EPD_2IN66_Init_Partial(){
//    EPD_2IN66_Reset();
//    EPD_2IN66_ReadBusy();
//    EPD_2IN66_SendCommand(0x12);
//    EPD_2IN66_ReadBusy();
//
//    EPD_2IN66_SetLUA();
//    EPD_2IN66_SendCommand(0x37);
//    EPD_2IN66_SendData(0x00);
//    EPD_2IN66_SendData(0x00);
//    EPD_2IN66_SendData(0x00);
//    EPD_2IN66_SendData(0x00);
//    EPD_2IN66_SendData(0x00);
//    EPD_2IN66_SendData(0x40);
//    EPD_2IN66_SendData(0x00);
//    EPD_2IN66_SendData(0x00);
//    EPD_2IN66_SendData(0x00);
//    EPD_2IN66_SendData(0x00);
//
//    /* Y increment, X increment */
//    EPD_2IN66_SendCommand(0x11);
//    EPD_2IN66_SendData(0x03);
//    /* Set RamX-address Start/End position */
//    EPD_2IN66_SendCommand(0x44);
//    EPD_2IN66_SendData(0x01);
//    EPD_2IN66_SendData((EPD_2IN66_WIDTH % 8 == 0)? (EPD_2IN66_WIDTH / 8 ): (EPD_2IN66_WIDTH / 8 + 1) );
//    /* Set RamY-address Start/End position */
//    EPD_2IN66_SendCommand(0x45);
//    EPD_2IN66_SendData(0);
//    EPD_2IN66_SendData(0);
//    EPD_2IN66_SendData((EPD_2IN66_HEIGHT&0xff));
//    EPD_2IN66_SendData((EPD_2IN66_HEIGHT&0x100)>>8);
//
//    EPD_2IN66_SendCommand(0x3C);
//    EPD_2IN66_SendData(0x80);
//
//    EPD_2IN66_SendCommand(0x22);
//    EPD_2IN66_SendData(0xcf);
//    EPD_2IN66_SendCommand(0x20);
//    EPD_2IN66_ReadBusy();
//}

void EPD_2IN66B_Clear(void)
{
    uint16_t Width, Height;
    Width = (EPD_2IN66_WIDTH % 8 == 0)? (EPD_2IN66_WIDTH / 8 ): (EPD_2IN66_WIDTH / 8 + 1);
    Height = EPD_2IN66_HEIGHT;
    EPD_2IN66_SendCommand(0x24);
    for (uint16_t j = 0; j <=Height; j++) {
        for (uint16_t i = 0; i < Width; i++) {
            EPD_2IN66_SendData(0xff);
        }
    }
    EPD_2IN66_TurnOnDisplay();
}

void EPD_2IN66_Display(uint8_t *Image)
{
    uint16_t Width, Height;
    Width = (EPD_2IN66_WIDTH % 8 == 0)? (EPD_2IN66_WIDTH / 8 ): (EPD_2IN66_WIDTH / 8 + 1);
    Height = EPD_2IN66_HEIGHT;

    uint32_t Addr = 0;

    // UDOUBLE Offset = ImageName;
    EPD_2IN66_SendCommand(0x24);
    for (uint16_t j = 0; j <Height; j++) {
        for (uint16_t i = 0; i <Width; i++) {
            Addr = i + j * Width;
            EPD_2IN66_SendData(Image[Addr]);
        }
    }
    EPD_2IN66_TurnOnDisplay();
}

void EPD_2IN66B_Sleep(){
    EPD_2IN66_SendCommand(0x10);
    EPD_2IN66_SendData(0x01);
}

void DEV_SPI_WriteByte(uint8_t value){
    SPI_PeriControl(SPI0, ENABLE);
    SPI_Send_Data(SPI0, &value, 1);
//    while(SPI_GetFlagStatus(SPI0, SPI_BUSY_FLAG));
    SPI_PeriControl(SPI0, DISABLE);
}

int DEV_Module_Init(){
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    return 0;
}

void DEV_Module_Exit(){
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_PIN, 0);

    // close 5v?
    DEV_Digital_Write(EPD_RST_PIN, 0);
}


