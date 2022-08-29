//
// Created by wbai on 8/10/2022.
//

#ifndef FRONT_PANEL_ADC_DRIVER_H
#define FRONT_PANEL_ADC_DRIVER_H

#include "k32lb11.h"

typedef struct {
    pADC_RegDef_t pADCx;
    // SC1A fields
    uint8_t InterruptControlA;
    uint8_t DifferentialModeA;
    uint8_t InputChannelA;
    // SC1B fields
    uint8_t InterruptControlB;
    uint8_t DifferentialModeB;
    uint8_t InputChannelB;
    // CFG1 fields
    uint8_t LowPowerConfig;
    uint8_t ClockDivideSelect;
    uint8_t SampleTimeConfig;
    uint8_t ConversionModeSelect;
    uint8_t InputClockSelect;
    // CFG2 fields
    uint8_t AsyncClockOutputEnable;
    uint8_t HighSpeedConfig;
    uint8_t LongSampleTimeSelect;
    // SC2 fields
//    uint8_t ConversionActive;
    uint8_t ConversionTriggerSelect;
    uint8_t CompareFunctionEnable;
    uint8_t CompareFunctionGTEnable;
    uint8_t CompareFunctionRangeEnable;
    uint8_t DMAEnable;
    uint8_t VoltageRefSelect;
    //SC3 fields
    uint8_t ContinuousConversionEnable;
    uint8_t HardwareAverageEnable;
    uint8_t HardwareAverageSelect;
    //CV register
    uint16_t CompareValue=0;
} ADC_Handle_t, *pADC_Handle_t;

/****************************
 * ADC_SC1 bit definition
 ****************************/
#define ADC_SC1_ADCH        0
#define ADC_SC1_DIFF        5
#define ADC_SC1_AIEN        6
#define ADC_SC1_COCO        7

#define COCO_COMPLETE       0x80
#define COCO_NOT            0x00

#define AIEN_ON             0x40
#define AIEN_OFF            0x00

#define DIFF_SINGLE         0x00
#define DIFF_DIFFERENTIAL   0x20

#define DAD0              0x00
#define DAD1              0x01
#define DAD2              0x02
#define DAD3              0x03
#define AD4               0x04
#define AD5               0x05
#define AD6               0x06
#define AD7               0x07
#define AD8               0x08
#define AD9               0x09
#define AD10              0x0A
#define AD11              0x0B
#define AD12              0x0C
#define AD13              0x0D
#define AD14              0x0E
#define AD15              0x0F
#define AD16              0x10
#define AD17              0x11
#define AD18              0x12
#define AD19              0x13
#define AD20              0x14
#define AD21              0x15
#define AD22              0x16
#define AD23              0x17
#define Vtemp             0x1A
#define Bandgap           0x1B
#define VREFO             0x1D
#define VLL1              0x1E
#define MODULE_DISABLE    0x1F

/********************************
 * ADC_CFG1 bit definition
 ********************************/
#define ADC_CFG1_ADICLK     0
#define ADC_CFG1_MODE       1
#define ADC_CFG1_ADLSMP     2
#define ADC_CFG1_ADIV       3
#define ADC_CFG1_ADLPC      7

#define ADLPC_NORMAL        0x00
#define ADLPC_LOW           0x80

#define ADIV_1              0x00
#define ADIV_2              0x20
#define ADIV_3              0x40
#define ADIV_4              0x60

#define ADLSMP_SHORT        0x00
#define ADLSMP_LONG         0x10

#define MODE_8              0x00
#define MODE_12             0x04
#define MODE_10             0x08
#define MODE_16             0x0C


#define ADICLK_BUS          0x00
#define ADICLK_BUS_2        0x01
#define ADICLK_ALTCLK       0x02
#define ADICLK_ADACK        0x03


/*******************************
 * ADC_CFG2 bit definition
 *******************************/
#define ADC_CFG2_ADLSTS     0
#define ADC_CFG2_ADHSC      2
#define ADC_CFG2_ADACKEN    3
#define ADC_CFG2_MUXSEL     4

#define ADACKEN_DISABLED    0x00
#define ADACKEN_ENABLED     0x08

#define ADHSC_NORMAL        0x00
#define ADHSC_HISPEED       0x04

#define ADLSTS_20           0x00
#define ADLSTS_12           0x01
#define ADLSTS_6            0x02
#define ADLSTS_2            0x03

/*****************************
 * ADC_SC2 bit definition
 *****************************/
#define ADC_SC2_REFSEL      0
#define ADC_SC2_DMAEN       2
#define ADC_SC2_ACREN       3
#define ADC_SC2_ACFGT       4
#define ADC_SC2_ACFE        5
#define ADC_SC2_ADTRG       6
#define ADC_SC2_ADACT       7

#define ADACT_INACTIVE      0x00
#define ADACT_ACTIVE        0x80

#define ADTRG_SW            0x00
#define ADTRG_HW            0x40

#define ACFE_DISABLED       0x00
#define ACFE_ENABLED        0x20

#define ACFGT_LESS          0x00
#define ACFGT_GREATER       0x10

#define ACREN_DISABLED      0x00
#define ACREN_ENABLED       0x08

#define REFSEL_EXT          0x00
#define REFSEL_ALT          0x01
#define REFSEL_BG           0x02
#define REFSEL_RES          0x03

/****************************
 * ADC_SC3 bit definition
 ****************************/
#define ADC_SC3_AVGS        0
#define ADC_SC3_AVGE        2
#define ADC_SC3_ADCO        3
#define ADC_SC3_CALF        6
#define ADC_SC3_CAL         7

#define CAL_BEGIN           0x80
#define CAL_OFF             0x00

#define CALF_NORMAL         0x00
#define CLAF_FAIL           0x40

#define ADCO_SINGLE         0x00
#define ADCO_CONTINUOUS     0x08

#define AVGE_DISABLED       0x00
#define AVGE_ENABLED        0x04

#define AVGS_4              0x00
#define AVGS_8              0x01
#define AVGS_16             0x02
#define AVGS_32             0x03

/*************************
 * Channel
 *************************/
#define CHANNEL_A        0
#define CHANNEL_B        1

/*********************
 * API
 *********************/
void ADC_Init(pADC_Handle_t pADCHandle);

void ADC_Cal(pADC_RegDef_t pADCx);

void ADC_Interrupt_Start(pADC_Handle_t pADCHandle, uint8_t channel);

uint16_t ADC_Read(pADC_Handle_t pADCHandle, uint8_t channel);

#endif //FRONT_PANEL_ADC_DRIVER_H

