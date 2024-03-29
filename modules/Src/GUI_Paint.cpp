//
// Created by wbai on 8/8/2022.
//

#include "Inc/GUI_Paint.h"
#include "cstdint"
#include "cstdlib"

PAINT Paint;
/******************************************************************************
function: Create Image
parameter:
    image   :   Pointer to the image cache
    width   :   The width of the picture
    Height  :   The height of the picture
    Color   :   Whether the picture is inverted
******************************************************************************/
void Paint_NewImage(uint8_t *image, uint16_t Width, uint16_t Height, uint16_t Rotate, uint16_t Color){
    Paint.Image = NULL;
    Paint.Image = image;

    Paint.WidthMemory = Width;
    Paint.HeightMemory = Height;
    Paint.Color = Color;
    Paint.Scale = 2;

    Paint.WidthByte = (Width % 8 == 0) ? (Width / 8) : (Width / 8 + 1);
    Paint.HeightByte = Height;

    Paint.Rotate = Rotate;
    Paint.Mirror = MIRROR_NONE;

    if (Rotate == ROTATE_0 || Rotate == ROTATE_180) {
        Paint.Width = Width;
        Paint.Height = Height;
    } else {
        Paint.Width = Height;
        Paint.Height = Width;
    }
}

/******************************************************************************
function: Select Image
parameter:
    image : Pointer to the image cache
******************************************************************************/
void Paint_SelectImage(uint8_t *image){
    Paint.Image = image;
}

/******************************************************************************
function: Select Image Rotate
parameter:
    Rotate : 0,90,180,270
******************************************************************************/
void Paint_SetRotate(uint16_t Rotate){
    if (Rotate == ROTATE_0 || Rotate == ROTATE_90 || Rotate == ROTATE_180 || Rotate == ROTATE_270) {
        Paint.Rotate = Rotate;
    } else{
        // debug
    }
}

void Paint_SetScale(uint8_t scale){
    if(scale == 2) {
        Paint.Scale = scale;
        Paint.WidthByte = (Paint.WidthMemory % 8 == 0) ? (Paint.WidthMemory / 8) : (Paint.WidthMemory / 8 + 1);
    } else if (scale == 4) {
        Paint.Scale = scale;
        Paint.WidthByte = (Paint.WidthMemory % 4 == 0) ? (Paint.WidthMemory / 4) : (Paint.WidthMemory / 4 + 1);
    } else if (scale == 7) {
        Paint.Scale = scale;
        Paint.WidthByte = (Paint.WidthMemory % 2 == 0) ? (Paint.WidthMemory / 2) : (Paint.WidthMemory / 2 + 1);
    }else{
        // debug
    }
}

/******************************************************************************
function:	Select Image mirror
parameter:
    mirror   :Not mirror,Horizontal mirror,Vertical mirror,Origin mirror
******************************************************************************/
void Paint_SetMirroring(uint8_t mirror){
    if (mirror == MIRROR_NONE || mirror == MIRROR_HORIZONTAL || mirror == MIRROR_VERTICAL || mirror == MIRROR_ORIGIN) {
        Paint.Mirror = mirror;
    }else{
        // debug
    }
}

/******************************************************************************
function: Draw Pixels
parameter:
    Xpoint : At point X
    Ypoint : At point Y
    Color  : Painted colors
******************************************************************************/
void Paint_SetPixel(uint16_t Xpoint, uint16_t Ypoint, uint16_t Color){
    if (Xpoint > Paint.Width || Ypoint > Paint.Height) {
        return;
    }
    uint16_t X, Y;
    switch (Paint.Rotate) {
        case 0:
            X = Xpoint;
            Y = Ypoint;
            break;
        case 90:
            X = Paint.WidthMemory - Ypoint - 1;
            Y = Xpoint;
            break;
        case 180:
            X = Paint.WidthMemory - Xpoint - 1;
            Y = Paint.HeightMemory - Ypoint - 1;
            break;
        case 270:
            X = Ypoint;
            Y = Paint.HeightMemory - Xpoint - 1;
            break;
        default:
            return;
    }

    switch (Paint.Mirror) {
        case MIRROR_NONE:
            break;
        case MIRROR_HORIZONTAL:
            X = Paint.WidthMemory - X - 1;
            break;
        case MIRROR_VERTICAL:
            Y = Paint.HeightMemory - Y - 1;
            break;
        case MIRROR_ORIGIN:
            X = Paint.WidthMemory - X - 1;
            Y = Paint.HeightMemory - Y - 1;
            break;
        default:
            return;
    }
    if (X > Paint.WidthMemory || Y > Paint.HeightMemory) {
        // debug: Exceeding display boundaries
        return;
    }

    if (Paint.Scale == 2) {
        uint16_t Addr = X / 8 + Y * Paint.WidthByte;
        uint8_t Rdata = Paint.Image[Addr];
        if (Color == BLACK) {
            Paint.Image[Addr] = Rdata & ~(0x80 >> (X % 8));
        }else {
            Paint.Image[Addr] = Rdata | (0x80 >> (X % 8));
        }
    }else if(Paint.Scale == 4){
        uint32_t Addr = X / 4 + Y * Paint.WidthByte;
        Color = Color % 4;
        uint8_t Rdata = Paint.Image[Addr];

        Rdata = Rdata & (~(0xC0 >> ((X % 4) * 2)));
        Paint.Image[Addr] = Rdata | ((Color << 6) >> ((X % 4) * 2));
    } else if (Paint.Scale == 7) {
        uint32_t Addr = X / 2 + Y * Paint.WidthByte;
        uint8_t Rdata = Paint.Image[Addr];
        Rdata = Rdata & (~(0XF0 >> ((X % 2) * 4)));
        Paint.Image[Addr] = Rdata | ((Color << 4) >> ((X % 2) * 4));
    }
}

/******************************************************************************
function: Clear the color of the picture
parameter:
    Color : Painted colors
******************************************************************************/
void Paint_Clear(uint16_t Color){
    if (Paint.Scale == 2 || Paint.Scale == 4) {
        for (uint16_t Y = 0; Y < Paint.HeightByte; Y++) {
            for (uint16_t X = 0; X < Paint.WidthByte; X++) {
                uint32_t Addr = X + Y * Paint.WidthByte;
                Paint.Image[Addr] = Color;
            }
        }
    } else if (Paint.Scale == 7) {
        for (uint16_t Y = 0; Y < Paint.HeightByte; Y++) {
            for (uint16_t X = 0; X < Paint.WidthByte; X++) {
                uint32_t Addr = X + Y * Paint.WidthByte;
                Paint.Image[Addr] = (Color << 4) | Color;
            }
        }
    }
}

/******************************************************************************
function: Clear the color of a window
parameter:
    Xstart : x starting point
    Ystart : Y starting point
    Xend   : x end point
    Yend   : y end point
    Color  : Painted colors
******************************************************************************/
void Paint_ClearWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color){
    uint16_t X, Y;
    for (Y = Ystart; Y < Yend; Y++) {
        for (X = Xstart; X < Xend; X++) {
            Paint_SetPixel(X, Y, Color);
        }
    }
}

/******************************************************************************
function: Draw Point(Xpoint, Ypoint) Fill the color
parameter:
    Xpoint		: The Xpoint coordinate of the point
    Ypoint		: The Ypoint coordinate of the point
    Color		: Painted color
    Dot_Pixel	: point size
    Dot_Style	: point Style
******************************************************************************/
void Paint_DrawPoint(uint16_t Xpoint, uint16_t Ypoint, uint16_t Color, DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_Style){
    if(Xpoint > Paint.Width || Ypoint > Paint.Height) {
        // debug
        return;
    }
    int16_t XDir_Num, YDir_Num;
    if (Dot_Style == DOT_FILL_AROUND) {
        for (XDir_Num = 0; XDir_Num < 2 * Dot_Pixel - 1; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num < 2 * Dot_Pixel - 1; YDir_Num++) {
                if (Xpoint + XDir_Num - Dot_Pixel < 0 || Ypoint + YDir_Num - Dot_Pixel < 0) {
                    break;
                }
                Paint_SetPixel(Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel, Color);
            }
        }
    }else {
        for (XDir_Num = 0; XDir_Num < Dot_Pixel; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num < Dot_Pixel; YDir_Num++) {
                Paint_SetPixel(Xpoint + XDir_Num - 1, Ypoint + YDir_Num - 1, Color);
            }
        }
    }
}

/******************************************************************************
function: Draw a line of arbitrary slope
parameter:
    Xstart ：Starting Xpoint point coordinates
    Ystart ：Starting Xpoint point coordinates
    Xend   ：End point Xpoint coordinate
    Yend   ：End point Ypoint coordinate
    Color  ：The color of the line segment
    Line_width : Line width
    Line_Style: Solid and dotted lines
******************************************************************************/
void Paint_DrawLine(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color,
                    DOT_PIXEL Line_width, LINE_STYLE Line_Style){
    if (Xstart > Paint.Width || Ystart > Paint.Height || Xend > Paint.Width || Yend > Paint.Height) {
        // debug
        return;
    }

    uint16_t Xpoint = Xstart;
    uint16_t Ypoint = Ystart;
    int dx = (int) Xend - (int) Xstart >= 0 ? Xend - Xstart : Xstart - Xend;
    int dy = (int) Yend - (int) Ystart <= 0 ? Yend - Ystart : Ystart - Yend;

    // Increment direction, 1 is positive, -1 is counter;
    int XAddway = Xstart < Xend ? 1 : -1;
    int YAddway = Ystart < Yend ? 1 : -1;

    // Cumulative error
    int Esp = dx + dy;
    char Dotted_Len = 0;

    for (;;) {
        Dotted_Len++;
        //Painted dotted line, 2 point is really virtual
        if (Line_Style == LINE_STYLE_DOTTED && Dotted_Len % 3 == 0) {
            Paint_DrawPoint(Xpoint, Ypoint, IMAGE_BACKGROUND, Line_width, DOT_STYLE_DFT);
            Dotted_Len = 0;
        }else {
            Paint_DrawPoint(Xpoint, Ypoint, Color, Line_width, DOT_STYLE_DFT);
        }
        if (2 * Esp >= dy) {
            if (Xpoint == Xend) {
                break;
            }
            Esp += dy;
            Xpoint += XAddway;
        }
        if (2 * Esp <= dy) {
            if (Ypoint == Yend) {
                break;
            }
            Esp += dx;
            Ypoint += YAddway;
        }
    }
}

/******************************************************************************
function: Draw a rectangle
parameter:
    Xstart ：Rectangular  Starting Xpoint point coordinates
    Ystart ：Rectangular  Starting Xpoint point coordinates
    Xend   ：Rectangular  End point Xpoint coordinate
    Yend   ：Rectangular  End point Ypoint coordinate
    Color  ：The color of the Rectangular segment
    Line_width: Line width
    Draw_Fill : Whether to fill the inside of the rectangle
******************************************************************************/
void Paint_DrawRectangle(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color,
                         DOT_PIXEL Line_width, DRAW_FILL Draw_Fill){
    if (Xstart > Paint.Width || Ystart > Paint.Height || Xend > Paint.Width || Yend > Paint.Height) {
        //debug
        return;
    }
    if (Draw_Fill) {
        uint16_t Ypoint;
        for (Ypoint = Ystart; Ypoint < Yend; Ypoint++) {
            Paint_DrawLine(Xstart, Ypoint, Xend, Ypoint, Color, Line_width, LINE_STYLE_SOLID);
        }
    }else{
        Paint_DrawLine(Xstart, Ystart, Xend, Ystart, Color, Line_width, LINE_STYLE_SOLID);
        Paint_DrawLine(Xstart, Ystart, Xstart, Yend, Color, Line_width, LINE_STYLE_SOLID);
        Paint_DrawLine(Xend, Yend, Xend, Ystart, Color, Line_width, LINE_STYLE_SOLID);
        Paint_DrawLine(Xend, Yend, Xstart, Yend, Color, Line_width, LINE_STYLE_SOLID);
    }
}

/******************************************************************************
function: Use the 8-point method to draw a circle of the
            specified size at the specified position->
parameter:
    X_Center  ：Center X coordinate
    Y_Center  ：Center Y coordinate
    Radius    ：circle Radius
    Color     ：The color of the ：circle segment
    Line_width: Line width
    Draw_Fill : Whether to fill the inside of the Circle
******************************************************************************/
void Paint_DrawCircle(uint16_t X_Center, uint16_t Y_Center, uint16_t Radius, uint16_t Color, DOT_PIXEL Line_width,
                      DRAW_FILL Draw_Fill){
    if (X_Center > Paint.Width || Y_Center >= Paint.Height) {
        // debug
        return;
    }

    //Draw a circle from(0, R) as a starting point
    int16_t XCurrent, YCurrent;
    XCurrent = 0;
    YCurrent = Radius;

    //Cumulative error, judge the next point of the logo
    int16_t Esp = 3 - (Radius << 1);

    int16_t sCountY;
    if (Draw_Fill == DRAW_FILL_FULL) {
        while (XCurrent <= YCurrent) {
            for (sCountY = XCurrent; sCountY < YCurrent; sCountY++) {
                Paint_DrawPoint(X_Center + XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT,
                                DOT_STYLE_DFT);
                Paint_DrawPoint(X_Center - XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT,
                                DOT_STYLE_DFT);//2
                Paint_DrawPoint(X_Center - sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT,
                                DOT_STYLE_DFT);//3
                Paint_DrawPoint(X_Center - sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT,
                                DOT_STYLE_DFT);//4
                Paint_DrawPoint(X_Center - XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT,
                                DOT_STYLE_DFT);//5
                Paint_DrawPoint(X_Center + XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT,
                                DOT_STYLE_DFT);//6
                Paint_DrawPoint(X_Center + sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT,
                                DOT_STYLE_DFT);//7
                Paint_DrawPoint(X_Center + sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT,
                                DOT_STYLE_DFT);
            }
            if (Esp < 0) {
                Esp += 4 * XCurrent + 6;
            }else {
                Esp += 10 + 4 * (XCurrent - YCurrent);
                YCurrent--;
            }
            XCurrent++;
        }
    } else{
        while (XCurrent <= YCurrent) {
            Paint_DrawPoint(X_Center + XCurrent, Y_Center + YCurrent, Color, Line_width,
                            DOT_STYLE_DFT);//1
            Paint_DrawPoint(X_Center - XCurrent, Y_Center + YCurrent, Color, Line_width,
                            DOT_STYLE_DFT);//2
            Paint_DrawPoint(X_Center - YCurrent, Y_Center + XCurrent, Color, Line_width,
                            DOT_STYLE_DFT);//3
            Paint_DrawPoint(X_Center - YCurrent, Y_Center - XCurrent, Color, Line_width,
                            DOT_STYLE_DFT);//4
            Paint_DrawPoint(X_Center - XCurrent, Y_Center - YCurrent, Color, Line_width,
                            DOT_STYLE_DFT);//5
            Paint_DrawPoint(X_Center + XCurrent, Y_Center - YCurrent, Color, Line_width,
                            DOT_STYLE_DFT);//6
            Paint_DrawPoint(X_Center + YCurrent, Y_Center - XCurrent, Color, Line_width,
                            DOT_STYLE_DFT);//7
            Paint_DrawPoint(X_Center + YCurrent, Y_Center + XCurrent, Color, Line_width,
                            DOT_STYLE_DFT);//0
            if (Esp < 0) {
                Esp += 4 * XCurrent + 6;
            }else {
                Esp += 10 + 4 * (XCurrent - YCurrent);
                YCurrent--;
            }
            XCurrent++;
        }
    }
}

/******************************************************************************
function: Show characters
parameter:
    Xpoint           ：X coordinate
    Ypoint           ：Y coordinate
    Acsii_Char       ：To display the English characters
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
//void Paint_DrawChar(uint16_t Xpoint, uint16_t Ypoint, const char Ascii_Char, sFONT *Font, uint16_t Color_Foreground,
//                    uint16_t Color_Background){
//    uint16_t Page, Column;
//    if (Xpoint > Paint.Width || Ypoint > Paint.Height) {
//        //debug: Input exceeds the normal display range
//        return;
//    }
//    uint32_t Char_Offset = (Ascii_Char - ' ') * Font->Height * (Font->Width / 8 + (Font->Width % 8 ? 1 : 0));
//    const unsigned char *ptr = &Font->table[Char_Offset];
//
//    for (Page = 0; Page < Font->Height; Page++) {
//        for (Column = 0; Column < Font->Width; Column++) {
//            //To determine whether the font background color and screen background color is consistent
//            if (FONT_BACKGROUND == Color_Background) {
//                if (*ptr & (0x80 >> (Column % 8))) {
//                    Paint_SetPixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
//                }
//            }else {
//                if (*ptr & (0x80 >> (Column % 8))) {
//                    Paint_SetPixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
//                } else {
//                    Paint_SetPixel(Xpoint + Column, Ypoint + Page, Color_Background);
//                }
//            }
//            //One pixel is 8 bits
//            if (Column % 8 == 7) {
//                ptr++;
//            }
//        } // Write a line
//        if (Font->Width % 8 != 0) {
//            ptr++;
//        }
//    }// Write all
//}

/******************************************************************************
function:	Display the string
parameter:
    Xstart           ：X coordinate
    Ystart           ：Y coordinate
    pString          ：The first address of the English string to be displayed
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
//void Paint_DrawString_EN(uint16_t Xstart, uint16_t Ystart, const char *pString, sFONT *Font, uint16_t Color_Foreground,
//                         uint16_t Color_Background){
//    uint16_t Xpoint = Xstart;
//    uint16_t Ypoint = Ystart;
//
//    if (Xstart > Paint.Width || Ystart > Paint.Height) {
//        // debug: Input exceeds the normal display range
//        return;
//    }
//
//    while (*pString != '\0') {
//        // if X direction filled, reposition to (Xstart, Ypoint), Ypoint is Y direction plus the Height of the charcter
//        if ((Xpoint + Font->Width) > Paint.Width) {
//            Xpoint = Xstart;
//            Ypoint += Font->Height;
//        }
//
//        // if Y direction is full, reposition to (Xstart, Ystart)
//        if ((Ypoint + Font->Height) > Paint.Height) {
//            Xpoint = Xstart;
//            Ypoint = Ystart;
//        }
//        Paint_DrawChar(Xpoint, Ypoint, *pString, Font, Color_Foreground, Color_Background);
//
//        // The next character of the address
//        pString++;
//
//        // The next word of the abscissa increases the font of the broadband
//        Xpoint += Font->Width;
//    }
//}

/******************************************************************************
function:	Display nummber
parameter:
    Xstart           ：X coordinate
    Ystart           : Y coordinate
    Nummber          : The number displayed
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
#define ARRAY_LEN 255
//void Paint_DrawNum(uint16_t Xpoint, uint16_t Ypoint, int32_t Number, sFONT *Font, uint16_t Color_Foreground,
//                   uint16_t Color_Background){
//    int16_t Num_Bit = 0, Str_Bit = 0;
//    uint8_t Str_Array[ARRAY_LEN] = {0}, Num_Array[ARRAY_LEN] = {0};
//    uint8_t *pStr = Str_Array;
//
//    if (Xpoint > Paint.Width || Ypoint > Paint.Height) {
//        // debug
//        return;
//    }
//
//    // Converts a number to a string
//    while(Number){
//        Num_Array[Num_Bit] = Number % 10 + '0';
//        Num_Bit++;
//        Number /= 10;
//    }
//    //THe string is inverted
//    while (Num_Bit > 0) {
//        Str_Array[Str_Bit] = Num_Array[Num_Bit - 1];
//        Str_Bit++;
//        Num_Bit--;
//    }
//
//    //show
//    Paint_DrawString_EN(Xpoint, Ypoint, (const char *) pStr, Font, Color_Foreground, Color_Background);
//}

/******************************************************************************
function:	Display time
parameter:
    Xstart           ：X coordinate
    Ystart           : Y coordinate
    pTime            : Time-related structures
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
//void Paint_DrawTime(uint16_t Xstart, uint16_t Ystart, PAINT_TIME *pTime, sFONT *Font, uint16_t Color_Foreground,
//                    uint16_t Color_Background){
//    uint8_t value[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
//
//    uint16_t Dx = Font->Width;
//
//    //Write data into the cache
//    Paint_DrawChar(Xstart                           , Ystart, value[pTime->Hour / 10], Font, Color_Foreground, Color_Background);
//    Paint_DrawChar(Xstart + Dx                      , Ystart, value[pTime->Hour % 10], Font, Color_Foreground, Color_Background);
//    Paint_DrawChar(Xstart + Dx  + Dx / 4 + Dx / 2   , Ystart, ':'                    , Font, Color_Foreground, Color_Background);
//    Paint_DrawChar(Xstart + Dx * 2 + Dx / 2         , Ystart, value[pTime->Min / 10] , Font, Color_Foreground, Color_Background);
//    Paint_DrawChar(Xstart + Dx * 3 + Dx / 2         , Ystart, value[pTime->Min % 10] , Font, Color_Foreground, Color_Background);
//    Paint_DrawChar(Xstart + Dx * 4 + Dx / 2 - Dx / 4, Ystart, ':'                    , Font, Color_Foreground, Color_Background);
//    Paint_DrawChar(Xstart + Dx * 5                  , Ystart, value[pTime->Sec / 10] , Font, Color_Foreground, Color_Background);
//    Paint_DrawChar(Xstart + Dx * 6                  , Ystart, value[pTime->Sec % 10] , Font, Color_Foreground, Color_Background);
//}

/******************************************************************************
function:	Display monochrome bitmap
parameter:
    image_buffer ：A picture data converted to a bitmap
info:
    Use a computer to convert the image into a corresponding array,
    and then embed the array directly into Imagedata.cpp as a .c file.
******************************************************************************/
void Paint_DrawBitMap(const unsigned char *image_buffer){
    uint16_t x, y;
    uint32_t Addr = 0;

    for (y = 0; y < Paint.HeightByte; y++) {
        for (x = 0; x < Paint.WidthByte; x++) {
            Addr = x + y * Paint.WidthByte;
            Paint.Image[Addr] = (unsigned char) image_buffer[Addr];
        }
    }
}
/******************************************************************************
function:	paste monochrome bitmap to a frame buff
parameter:
    image_buffer ：A picture data converted to a bitmap
    xStart: The starting x coordinate
    yStart: The starting y coordinate
    imageWidth: Original image width
    imageHeight: Original image height
    flipColor: Whether the color is reversed
info:
    Use this function to paste image data into a buffer
******************************************************************************/
void Paint_DrawBitMap_Paste(const unsigned char *image_buffer, uint16_t Xstart, uint16_t Ystart,
                            uint16_t imageWidth, uint16_t imageHeight, uint8_t flipColor){
    uint8_t color, srcImage;
    uint16_t x, y;
    uint16_t width = (imageWidth % 8 == 0 ? imageWidth / 8 : imageWidth / 8 + 1);

    for (y = 0; y < imageWidth; y++) {
        for (x = 0; x < imageWidth; x++) {
            srcImage = image_buffer[y * width + x / 8];
            if (flipColor) {
                color = (((srcImage << (x % 8) & 0x80) == 0) ? 1 : 0);
            }else {
                color = (((srcImage << (x % 8) & 0x80) == 0) ? 0 : 1);
            }
            Paint_SetPixel(x + Xstart, y + Ystart, color);
        }
    }
}

void Paint_DrawBitMap_Block(const unsigned char *image_buffer, uint8_t Region){
    uint16_t x, y;
    uint32_t Addr = 0;
    for (y = 0; y < Paint.HeightByte; y++) {
        for (x = 0; x < Paint.WidthByte; x++) {
            Addr = x + y * Paint.WidthByte;
            Paint.Image[Addr] = (unsigned char) image_buffer[Addr +
                                                             (Paint.HeightByte) * Paint.WidthByte * (Region - 1)];
        }
    }
}