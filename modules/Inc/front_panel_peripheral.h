//
// Created by andy- on 2022-08-17.
//

#ifndef FRONT_PANEL_FRONT_PANEL_PERIPHERAL_H
#define FRONT_PANEL_FRONT_PANEL_PERIPHERAL_H

#include "k32lb11.h"

#define I2C_SLV_ADDRESS 0x68

/******************************
 * Actuator status
 ******************************/
#define STOP        0
#define FORWARD     1
#define REVERSE     2
#define EXTENDED    3
#define RETRACTED   4

/**********************************
 * JIG Status
 **********************************/
#define STANDBY             0
#define BUSY                1
#define DISENGAGED          2
#define ENGAGED             3
#define DISENGAGING         4
#define ENGAGING            5
#define ERROR               6
#define STANDBY_ENGAGED     7
#define STANDBY_DISENGAGED  8

void FRONT_PANEL_TEST_STATUS_LED_INIT();

void FRONT_PANEL_START_RELEASE_BUTTON_INIT();

void FRONT_PANEL_RESEAT_BUTTON_INIT();

void FRONT_PANEL_LATCH_CONTROL_INIT();

void FRONT_PANEL_MASTER_RELAY_INIT();

void FRONT_PANEL_LIMIT_SWITCH_INIT();

void FRONT_PANEL_DUT_POWER_CONTROL_INIT();

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
void FRONT_PANEL_TEST_LED_BLINK_RED();
void FRONT_PANEL_TEST_LED_OFF();
void FRONT_PANEL_JIG_LED_GREEN();
void FRONT_PANEL_JIG_LED_RED();
void FRONT_PANEL_JIG_LED_AMBER();
void FRONT_PANEL_JIG_LED_BLINK_AMBER();
void FRONT_PANEL_JIG_LED_BLINK_RED();
void FRONT_PANEL_JIG_LED_OFF();

void FRONT_PANEL_BUTTON_RGB_INIT();
void FRONT_PANEL_START_RELEASE_BUTTON_RGB_CONTROL(uint8_t state);

void FRONT_PANEL_I2C_INT();



#endif //FRONT_PANEL_FRONT_PANEL_PERIPHERAL_H
