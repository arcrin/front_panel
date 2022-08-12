//
// Created by wbai on 8/10/2022.
//

#include "adc_driver.h"

void ADC_Cal(pADC_RegDef_t pADCx){
    _vo uint16_t cal_var;
    pADCx->SC3 |= (1 << ADC_SC3_AVGE);
    pADCx->SC2 &= ~(1 << ADC_SC2_ADTRG);
    pADCx->SC3 |= (1 << ADC_SC3_CAL);

    while (!(pADCx->SC1A & (1 << ADC_SC1_COCO))); // wait for conversion to complete
    if (pADCx->SC3 & (1 << ADC_SC3_CALF)) { // Check for Calibration fail error
        pADCx->SC3 &= ~(1 << ADC_SC3_CALF); // Clear fail flag
    }

    // Calculate plus-side calibration
    cal_var = 0x00;
    cal_var += (pADCx->CLP0 & 0x3F) +
               (pADCx->CLP1 & 0x7F) +
               (pADCx->CLP2 & 0xFF) +
               (pADCx->CLP3 & 0x1FF) +
               (pADCx->CLP4 & 0x3FF) +
               (pADCx->CLPS & 0x3F);

    cal_var = cal_var / 2;

    cal_var += 0x8000; // Set MSB

    pADCx->PG |= cal_var;

    // Calculate minus-side calibration
    cal_var = 0x00;

    cal_var += (pADCx->CLM0 & 0x3F) +
               (pADCx->CLM1 & 0x7F) +
               (pADCx->CLM2 & 0xFF) +
               (pADCx->CLM3 & 0x1FF) +
               (pADCx->CLM4 & 0x3FF) +
               (pADCx->CLMS & 0x3F);

    cal_var = cal_var / 2;
    cal_var += 0x8000;

    pADCx->MG |= cal_var;

    pADCx->SC3 &= ~(1 << ADC_SC3_CAL);
    pADCx->SC3 &= ~(1 << ADC_SC3_AVGE); // Disable averaging
//    pADCx->SC2 |= (1 << ADC_SC2_ADTRG); // Enable Hardware Trigger
}

void ADC_Init(pADC_Handle_t pADCHandle){
    uint32_t temp = 0;

    ADC0_CLOCK_EN();


    // CFG1 Register
    temp = (pADCHandle->LowPowerConfig |
            pADCHandle->ClockDivideSelect |
            pADCHandle->SampleTimeConfig |
            pADCHandle->ConversionModeSelect |
            pADCHandle->InputClockSelect);

    pADCHandle->pADCx->CFG1 |= temp;

    // CFG2 Register
    temp = (pADCHandle->AsyncClockOutputEnable |
            pADCHandle->HighSpeedConfig |
            pADCHandle->LongSampleTimeSelect);

    pADCHandle->pADCx->CFG2 |= temp;

    // SC2 Register
    temp = (pADCHandle->ConversionTriggerSelect |
            pADCHandle->CompareFunctionEnable |
            pADCHandle->CompareFunctionGTEnable |
            pADCHandle->CompareFunctionRangeEnable |
            pADCHandle->DMAEnable |
            pADCHandle->VoltageRefSelect);

    pADCHandle->pADCx->SC2 |= temp;

    // SC3 Register
    temp = (pADCHandle->ContinuousConversionEnable |
            pADCHandle->HardwareAverageEnable |
            pADCHandle->HardwareAverageSelect);

    pADCHandle->pADCx->SC3 |= temp;


    // SC1 Register
    temp = (pADCHandle->InterruptControlA |
            pADCHandle->DifferentialModeA |
            0x1F);

    pADCHandle->pADCx->SC1A |= temp;

    // SC2 Register
    temp = (pADCHandle->InterruptControlB |
            pADCHandle->DifferentialModeB |
            pADCHandle->InputChannelB |
            0x1F);

    pADCHandle->pADCx->SC1B |= temp;


    // Calibration
    ADC_Cal(pADCHandle->pADCx);
}

