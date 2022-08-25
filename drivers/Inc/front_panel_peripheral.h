//
// Created by andy- on 2022-08-17.
//

#ifndef FRONT_PANEL_FRONT_PANEL_PERIPHERAL_H
#define FRONT_PANEL_FRONT_PANEL_PERIPHERAL_H

#include "k32lb11.h"
/*******************************
 * LED Color
 *******************************/
#define COLOR_OFF         0
#define COLOR_GREEN       1
#define COLOR_RED         2
#define COLOR_BLUE        3

void FRONT_PANEL_TEST_STATUS_LED_INIT();

void FRONT_PANEL_START_RELEASE_BUTTON_INIT();

void FRONT_PANEL_RESEAT_BUTTON_INIT();

void FRONT_PANEL_ACT1_CONTROL_INIT();

void FRONT_PANEL_ACT2_CONTROL_INIT();

void FRONT_PANEL_LPUART_INIT();

void FRONT_PANEL_ADC0_INIT();

void FRONT_PANEL_ACT1_FORWARD();

void FRONT_PANEL_ACT1_REVERSE();

void FRONT_PANEL_ACT1_STOP();


#endif //FRONT_PANEL_FRONT_PANEL_PERIPHERAL_H
