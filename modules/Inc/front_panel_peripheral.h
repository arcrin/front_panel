//
// Created by andy- on 2022-08-17.
//

#ifndef FRONT_PANEL_FRONT_PANEL_PERIPHERAL_H
#define FRONT_PANEL_FRONT_PANEL_PERIPHERAL_H

#include "k32lb11.h"



/******************************
 * Actuator status
 ******************************/
#define STOP        0
#define FORWARD     1
#define REVERSE     2

void FRONT_PANEL_TEST_STATUS_LED_INIT();

void FRONT_PANEL_START_RELEASE_BUTTON_INIT();

void FRONT_PANEL_RESEAT_BUTTON_INIT();

void FRONT_PANEL_ACT1_CONTROL_INIT();

void FRONT_PANEL_ACT2_CONTROL_INIT();

void FRONT_PANEL_LPUART_INIT();

void FRONT_PANEL_ADC0_INIT();

void FRONT_PANEL_ACT1_FORWARD(uint8_t speed);

void FRONT_PANEL_ACT1_REVERSE(uint8_t speed);

void FRONT_PANEL_ACT1_STOP();

void FRONT_PANEL_ACT2_FORWARD(uint8_t speed);

void FRONT_PANEL_ACT2_REVERSE(uint8_t speed);

void FRONT_PANEL_ACT2_STOP();

#endif //FRONT_PANEL_FRONT_PANEL_PERIPHERAL_H
