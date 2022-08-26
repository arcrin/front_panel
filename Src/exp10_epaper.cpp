//
// Created by wbai on 8/8/2022.
//

#include "backup/epaper.h"
#include "Inc/GUI_Paint.h"
//#include "barcode.h"

extern const unsigned char label_image[];

int main(){
    DISABLE_IRQ();
    MCG_Init();
    SysTick_Init(8000);
    EPD_GPIO_Init();
    EPD_SPI_Init();
    RESET_INTERRUPT();
    ENABLE_IRQ();


//    if (DEV_Module_Init() != 0) {
//        continue;
//    }
    EPD_2IN66_Init();
    EPD_2IN66_Clear();
    delay(5000);

    uint8_t *BlankImage;
    uint16_t Imagesize =
            ((EPD_2IN66_WIDTH % 8 == 0) ? (EPD_2IN66_WIDTH / 8) : (EPD_2IN66_WIDTH / 8 + 1)) * EPD_2IN66_HEIGHT;
    if ((BlankImage = (uint8_t *) malloc(Imagesize)) == NULL) {
        return -1;
    }
    Paint_NewImage(BlankImage, EPD_2IN66_WIDTH, EPD_2IN66_HEIGHT, ROTATE_270, WHITE);
#if 0
    //show image for array
    Paint_SelectImage(BlankImage);
    Paint_Clear(WHITE);
//        BlankImage[0] = 0x3f;
//        BlankImage[19] = 0x3f;
//        BlankImage[37] = 0x0;
//        BlankImage[74] = 0x0;
//        BlankImage[111] = 0x0;
//    Paint_DrawPoint(20, 65, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
//        Paint_DrawPoint(10, 80, WHITE, DOT_PIXEL_1X1, DOT_STYLE_DFT);
//        Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
//
//        Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
//
//    Paint_DrawRectangle(20, 70, 20, 100, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
//        Paint_DrawRectangle(30, 70, 30, 100, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawRectangle(80, 70, 81, 71, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
//
//        Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
//
//        Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
//
    Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
//        Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
//
//        Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
//        Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);

    EPD_2IN66_Display(BlankImage);
#endif

#if 1
    EPD_2IN66_Display((uint8_t*)&label_image);
#endif

    delay(3000);
    EPD_2IN66_Sleep();
    free(BlankImage);
    BlankImage = NULL;

    while (1);
}