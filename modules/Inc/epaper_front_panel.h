//
// Created by wbai on 8/8/2022.
//

#ifndef FRONT_PANEL_EPAPER_H
#define FRONT_PANEL_EPAPER_H

#include "k32lb11.h"

#define RST_Pin             7
#define RST_GPIO_Port       GPIOD
#define DC_Pin              3
#define DC_GPIO_Port        GPIOD
#define BUSY_Pin            31
#define BUSY_GPIO_Port      GPIOE
#define SPI_CS_Pin          4
#define SPI_CS_GPIO_Port    GPIOD

/*
 * e-Paper GPIO
 */
#define EPD_RST_PIN     RST_GPIO_Port, RST_Pin
#define EPD_DC_PIN      DC_GPIO_Port, DC_Pin
#define EPD_CS_PIN      SPI_CS_GPIO_Port, SPI_CS_Pin
#define EPD_BUSY_PIN    BUSY_GPIO_Port, BUSY_Pin

/*
 * GPIO read and write
 */
#define DEV_Digital_Write(_pin, _value) GPIO_WriteOutputPin(_pin, _value == 0? GPIO_PIN_RESET:GPIO_PIN_SET)
#define DEV_Digital_Read(_pin) GPIO_ReadFromInputPin(_pin)

/*
 * delay
 */
#define DEV_Delay_ms(__xms)     delay(__xms)

void DEV_SPI_WriteByte(uint8_t value);

int DEV_Module_Init();

void DEV_Module_Exit();


/*
 * 2.66 inch epaper display specific
 */
#define EPD_2IN66B_WIDTH        152
#define EPD_2IN66B_HEIGHT       296

void EPD_2IN66_Init();

void EPD_2IN66B_Clear();

void EPD_2IN66B_Display(uint8_t *ImageBlack, uint8_t* ImageRed);

void EPD_2IN66B_Sleep();

void EPD_GPIO_Init();

void EPD_SPI_Init();

#endif //FRONT_PANEL_EPAPER_H
