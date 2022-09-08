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
#define READY                 0
#define BUSY                    1
#define ACT_DISENGAGED          2
#define ACT_ENGAGED             3
#define ACT_DISENGAGING         4
#define ACT_ENGAGING            5
#define ERROR                   6
#define STANDBY_ACT_ENGAGED     7
#define STANDBY_ACT_DISENGAGED  8
#define LATCH_DISENGAGING       9
#define FULLY_DISENGAGED        10
#define ACT_ENGAGE_ERROR        11
#define FULLY_ENGAGED           12
#define LATCH_ENGAGING          13
#define LATCH_ENGAGE_ERROR      14
#define STARTUP                 15

/***********************************
 * TEST Status
 ***********************************/
#define TEST_STANDBY                 0
#define TEST_RESEAT                  1


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
