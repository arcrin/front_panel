//
// Created by wbai on 5/2/2022.
//
#include "port_driver.h"
/*********************************************************************************************
 * @fn          - PORT_ClockControl
 *
 * @brief       - Initialize or De-initialize port clock for a give port
 *
 * @param[pPORT_RegDef_t]
 *              - PORT_RegDef_t pointer that contains register definitions for PORT module
 *
 * @param[uint8_t]
 *              - EN/DI value that we want to assign to the PORT
 * @return      - None
 *
 * @Note        - None
 *********************************************************************************************/
void PORT_ClockControl(pPORT_RegDef_t pPORTx, uint8_t ENorDI){
    if(ENorDI == ENABLE){
        if (pPORTx == PORTA) {
            PORTA_CLOCK_EN();
        }
        else if (pPORTx == PORTB) {
            PORTB_CLOCK_EN();
        }
        else if (pPORTx == PORTC) {
            PORTC_CLOCK_EN();
        }
        else if (pPORTx == PORTD) {
            PORTD_CLOCK_EN();
        }
        else if (pPORTx == PORTE) {
            PORTE_CLOCK_EN();
        }
    }
    else if(ENorDI == DISABLE){
        if (pPORTx == PORTA) {
            PORTA_CLOCK_DI();
        }
        else if (pPORTx == PORTB) {
            PORTB_CLOCK_DI();
        }
        else if (pPORTx == PORTC) {
            PORTC_CLOCK_DI();
        }
        else if (pPORTx == PORTD) {
            PORTD_CLOCK_DI();
        }
        else if (pPORTx == PORTE) {
            PORTE_CLOCK_DI();
        }
    }
}

/*********************************************************************************************
 * @fn          - PORT_ClockControl
 *
 * @brief       - Initialize or De-initialize port clock for a give port
 *
 * @param[pPORT_RegDef_t]
 *              - PORT_RegDef_t pointer that contains register definitions for PORT module
 *
 * @param[uint8_t]
 *              - EN/DI value that we want to assign to the PORT
 * @return      - None
 *
 * @Note        - None
 *********************************************************************************************/
void PORT_Init(PORT_Handle_t* pPortHandle){
    PORT_ClockControl(pPortHandle->pPORT, ENABLE);

    uint8_t pin_number = pPortHandle->PORT_Config.PORT_Pin_Number;

    // Interrupt configuration
    pPortHandle->pPORT->PORT_PCR[pin_number] &= ~(0xF << 16); // clear interrupt config bits
    pPortHandle->pPORT->PORT_PCR[pin_number] |= pPortHandle->PORT_Config.PORT_Pin_Interrupt_cfg << 16;

    // Port pin MUX config
    pPortHandle->pPORT->PORT_PCR[pin_number] &= ~(0x7 << 8);
    pPortHandle->pPORT->PORT_PCR[pin_number] |= (pPortHandle->PORT_Config.PORT_Pin_Function << 8);

    // Pull enable
    if (pPortHandle->PORT_Config.PORT_Pin_Pull_Enable == ENABLE){
        pPortHandle->pPORT->PORT_PCR[pin_number] |= 0x1 << 1;
    }
    else if (pPortHandle->PORT_Config.PORT_Pin_Pull_Enable == DISABLE){
        pPortHandle->pPORT->PORT_PCR[pin_number] &= ~(0x1 << 1);
    }

    // Pull select
    if(pPortHandle->PORT_Config.PORT_Pin_Pull_Select == PULL_UP){
        pPortHandle->pPORT->PORT_PCR[pin_number] |= 0x1 << 0;
    }
    else if (pPortHandle->PORT_Config.PORT_Pin_Pull_Select == PULL_DOWN){
        pPortHandle->pPORT->PORT_PCR[pin_number] &= ~(0x1 << 0);
    }
}

/*********************************************************************************************
 * @fn          - PORT_IRQHandling
 *
 * @brief       -
 *
 * @param[uint8_t]
 *              -
 *
 * @return      - None
 *
 * @Note        - None
 *********************************************************************************************/
void PORT_IRQHandling(pPORT_RegDef_t pPORT, uint8_t PinNumber){
    if (pPORT->PORT_PCR[PinNumber] & (1 << 24)){
        pPORT->PORT_PCR[PinNumber] |= (1 << 24);
    }
}