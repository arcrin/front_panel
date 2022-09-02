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

/**********************************
 * JIG Status
 **********************************/
#define READY       0
#define BUSY        1

void FRONT_PANEL_TEST_STATUS_LED_INIT();

void FRONT_PANEL_START_RELEASE_BUTTON_INIT();

void FRONT_PANEL_RESEAT_BUTTON_INIT();

void FRONT_PANEL_LATCH_CONTROL_INIT();

void FRONT_PANEL_MASTER_RELAY_INIT();

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

void FRONT_PANEL_TEST_LED_GREEN();
void FRONT_PANEL_TEST_LED_RED();
void FRONT_PANEL_TEST_LED_AMBER();
void FRONT_PANEL_TEST_LED_BLINK_AMBER();
void FRONT_PANEL_TEST_LED_OFF();
void FRONT_PANEL_JIG_LED_GREEN();
void FRONT_PANEL_JIG_LED_RED();
void FRONT_PANEL_JIG_LED_AMBER();
void FRONT_PANEL_JIG_LED_BLINK_AMBER();
void FRONT_PANEL_JIG_LED_OFF();

void FRONT_PANEL_BUTTON_RGB_INIT();
void FRONT_PANEL_START_RELEASE_BUTTON_RGB_CONTROL(uint8_t state);



#endif //FRONT_PANEL_FRONT_PANEL_PERIPHERAL_H
