//
// Created by wbai on 8/8/2022.
//


//
// Created by wbai on 8/8/2022.
//

#include "Inc/epaper_front_panel.h"
#include "Inc/GUI_Paint.h"
//#include "barcode.h"

extern const unsigned char label_image[];
//extern const unsigned char gImage_2in66bb[];
//extern const unsigned char gImage_2in66br[];

int main(){
    DISABLE_IRQ();
    MCG_Init();
    SysTick_Init(8000);// millisecond delay
    EPD_GPIO_Init();
    EPD_SPI_Init();
    RESET_INTERRUPT();
    ENABLE_IRQ();

    while(1){
        EPD_2IN66_Init();
        EPD_2IN66B_Clear();
        delay(15000);

        uint8_t *BlackImage, *RedImage;
        uint16_t Imagesize =
                ((EPD_2IN66B_WIDTH % 8 == 0) ? (EPD_2IN66B_WIDTH / 8) : (EPD_2IN66B_WIDTH / 8 + 1)) * EPD_2IN66B_HEIGHT;
        if ((BlackImage = (uint8_t *) malloc(Imagesize)) == NULL) {
            return -1;
        }
        if ((RedImage = (uint8_t *) malloc(Imagesize)) == NULL) {
            return -1;
        }
        Paint_NewImage(BlackImage, EPD_2IN66B_WIDTH, EPD_2IN66B_HEIGHT, 270, WHITE);
    //    Paint_Clear(WHITE);
        Paint_NewImage(RedImage, EPD_2IN66B_WIDTH, EPD_2IN66B_HEIGHT, 270, WHITE);
    //    Paint_Clear(WHITE);
    #if 0
        //show image for array
        Paint_SelectImage(BlackImage);
        Paint_Clear(WHITE);
    //        BlackImage[0] = 0x3f;
    //        BlackImage[19] = 0x3f;
    //        BlackImage[37] = 0x0;
    //        BlackImage[74] = 0x0;
    //        BlackImage[111] = 0x0;
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

        EPD_2IN66B_Display(BlackImage);
    #endif


    #if 1
        Paint_SelectImage(RedImage);
        Paint_Clear(WHITE);
        EPD_2IN66B_Display((uint8_t*)&label_image, RedImage);

    #endif

    #if 0
        Paint_SelectImage(BlackImage);
        Paint_Clear(WHITE);


        Paint_SelectImage(RedImage);
        Paint_Clear(RED);
        EPD_2IN66B_Display(BlackImage, RedImage);

    #endif
    #if 0
        Paint_SelectImage(BlackImage);
        Paint_Clear(WHITE);
        Paint_DrawBitMap(gImage_2in66bb);

        Paint_SelectImage(RedImage);
        Paint_Clear(WHITE);
        Paint_DrawBitMap(gImage_2in66br);

        EPD_2IN66B_Display(BlackImage, RedImage);

    #endif


        delay(15000);
        EPD_2IN66B_Sleep();
        free(BlackImage);
        free(RedImage);
        BlackImage = NULL;
        RedImage = NULL;
    }
}