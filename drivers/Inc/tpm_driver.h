//
// Created by wbai on 8/24/2022.
//

#ifndef FRONT_PANEL_TPM_DRIVER_H
#define FRONT_PANEL_TPM_DRIVER_H

/**********************************************
 * Status and Control register (SC) bit config
 **********************************************/
#define TPM_SC_PS       0
#define TPM_SC_CMOD     3
#define TPM_SC_CPWMS    5
#define TPM_SC_TOIE     6
#define TPM_SC_TOF      7
#define TPM_SC_DMA      8


/********************************************************
 * Channel Status and Control register (CnSC) bit config
 ********************************************************/
#define TPM_CSC_DMA     0
#define TPM_CSC_ELSA    2
#define TPM_CSC_ELSB    3
#define TPM_CSC_MSA     4
#define TPM_CSC_MSB     5
#define TPM_CSC_CHIE    6
#define TPM_CSC_CHF     7


/**********************************************************
 * Capture and Compare Status register (STATUS) bit config
 **********************************************************/
#define TPM_STATUS_CH0F        0
#define TPM_STATUS_CH1F        1
#define TPM_STATUS_CH2F        2
#define TPM_STATUS_CH3F        3
#define TPM_STATUS_CH4F        4
#define TPM_STATUS_CH5F        5
#define TPM_STATUS_TOF         8

/****************************************************
 * Channel Polarity register (POL) bit config
 ****************************************************/
#define TPM_POL_POL0        0
#define TPM_POL_POL1        1
#define TPM_POL_POL2        2
#define TPM_POL_POL3        3
#define TPM_POL_POL4        4
#define TPM_POL_POL5        5


/********************************************
 * Configuration register (CONF) bit config
 ********************************************/
#define TPM_CONF_DOZEEN     5
#define TPM_CONF_DBGMODE    6
#define TPM_CONF_GTBSYNC    8
#define TPM_CONF_GTBEEN     9
#define TPM_CONF_CSOT       16
#define TPM_CONF_CSOO       17
#define TPM_CONF_CROT       18
#define TPM_CONF_CPOT       19
#define TPM_CONF_TRGPOL     22
#define TPM_CONF_TRGSRC     23
#define TPM_CONF_TRGSEL     24

/*************************************
 * TPM Handle
 *************************************/
// typedef struct{
//
// };

void TPM1_Init();

void TPM2_Init();



#endif //FRONT_PANEL_TPM_DRIVER_H
