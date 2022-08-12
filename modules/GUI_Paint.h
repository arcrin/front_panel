//
// Created by wbai on 8/8/2022.
//

#ifndef FRONT_PANEL_GUI_PAINT_H
#define FRONT_PANEL_GUI_PAINT_H

#include <cstdint>
#include <cstdlib>
#include "fonts.h"

/*
 * Image attributes
 */
typedef struct {
    uint8_t *Image;
    uint16_t Width;
    uint16_t Height;
    uint16_t WidthMemory;
    uint16_t HeightMemory;
    uint16_t Color;
    uint16_t Rotate;
    uint16_t Mirror;
    uint16_t WidthByte;
    uint16_t HeightByte;
    uint16_t Scale;
} PAINT;
extern PAINT Paint;

/*
 * Display rotate
 */
#define ROTATE_0    0
#define ROTATE_90   90
#define ROTATE_180  180
#define ROTATE_270  270

/*
 * Display Flip
 */
typedef enum{
    MIRROR_NONE = 0x00,
    MIRROR_HORIZONTAL = 0x01,
    MIRROR_VERTICAL = 0x02,
    MIRROR_ORIGIN = 0x03,
} MIRROR_IMAGE;
#define MIRROR_IMAGE_DFT    MIRROR_NONE

/*
 * image color
 */
#define WHITE       0xFF // turn on pixel
#define BLACK       0x00 // turn off pixel
#define RED         BLACK

#define IMAGE_BACKGROUND    WHITE
#define FONT_FOREGROUND     BLACK
#define FONT_BACKGROUND     WHITE

#define TRUE    1
#define FALSE   0

// 4 Gray level
#define GRAY1   0x03
#define GRAY2   0x02
#define GRAY3   0x01
#define GRAY4   0x00

// color
#define Black_4Color    0x00
#define White_4Color    0x01
#define Yellow_4Color   0x02
#define Red_4Color      0x03

/*
 * the size of the point
 */
typedef enum{
    DOT_PIXEL_1X1 = 1,
    DOT_PIXEL_2X2,
    DOT_PIXEL_3X3,
    DOT_PIXEL_4X4,
    DOT_PIXEL_5X5,
    DOT_PIXEL_6X6,
    DOT_PIXEL_7X7,
    DOT_PIXEL_8X8,
}DOT_PIXEL;
#define DOT_PIXEL_DFT   DOT_PIXEL_1X1

/*
 * point size fill style
 */
typedef enum{
    DOT_FILL_AROUND = 1,
    DOT_FILL_RIGHTUP,
}DOT_STYLE;
#define DOT_STYLE_DFT   DOT_FILL_AROUND

/*
 * Line style, solid or dashed
 */
typedef enum{
    LINE_STYLE_SOLID = 0,
    LINE_STYLE_DOTTED,
}LINE_STYLE;

/*
 *
 * whether the graphic is filled
 */
typedef enum{
    DRAW_FILL_EMPTY = 0,
    DRAW_FILL_FULL,
}DRAW_FILL;

/*
 * Custom structure of a time attribute
 */
typedef struct {
    uint16_t Year;
    uint8_t Day;
    uint8_t Hour;
    uint8_t Min;
    uint8_t Sec;
}PAINT_TIME;
extern PAINT_TIME sPaint_time;

// init and clear
void Paint_NewImage(uint8_t *image, uint16_t Width, uint16_t Height, uint16_t Rotate, uint16_t Color);

void Paint_SelectImage(uint8_t *image);

void Paint_SetRotate(uint16_t Rotate);

void Paint_SetMirroring(uint8_t mirror);

void Paint_SetPixel(uint16_t Xpoint, uint16_t Ypoint, uint16_t Color);

void Paint_SetScale(uint8_t scale);

void Paint_Clear(uint16_t Color);

void Paint_ClearWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color);


// Drawing
void Paint_DrawPoint(uint16_t Xpoint, uint16_t Ypoint, uint16_t Color, DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_Style);

void Paint_DrawLine(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color,
                    DOT_PIXEL Line_width, LINE_STYLE Line_Style);

void Paint_DrawRectangle(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color,
                         DOT_PIXEL Line_width, DRAW_FILL Draw_Fill);

void Paint_DrawCircle(uint16_t X_Center, uint16_t Y_Center, uint16_t Radius, uint16_t Color, DOT_PIXEL Line_width,
                      DRAW_FILL Draw_Fill);


// Display string
void Paint_DrawChar(uint16_t Xpoint, uint16_t Ypoint, const char Ascii_Char, sFONT *Font, uint16_t Color_Foreground,
                    uint16_t Color_Background);

void Paint_DrawString_EN(uint16_t Xstart, uint16_t Ystart, const char *pString, sFONT *Font, uint16_t Color_Foreground,
                         uint16_t Color_background);

void Paint_DrawNum(uint16_t Xpoint, uint16_t Ypoint, int32_t Number, sFONT *Font, uint16_t Color_Foreground,
                   uint16_t Color_Background);

void Paint_DrawTime(uint16_t Xstart, uint16_t Ystart, PAINT_TIME *pTime, sFONT *Font, uint16_t Color_Foreground,
                    uint16_t Color_Background);

// pic
void Paint_DrawBitMap(const unsigned char *image_buffer);

void Paint_DrawBitMap_Paste(const unsigned char *image_bugger, uint16_t Xstart, uint16_t Ystart,
                            uint16_t imageWidth, uint16_t imageHeight, uint8_t flipColor);

void Paint_DrawBitMap_Block(const unsigned char *image_buffer, uint8_t Region);


#endif //FRONT_PANEL_GUI_PAINT_H
