#ifndef __DRIVER_MDR_ADC_H__
#define __DRIVER_MDR_ADC_H__

#include "MDR1986VE1T.h"
#include "stdint.h"

void MDR1986VE1T_SetPortPins ( MDR_PORT_TypeDef* pPORT, uint16_t PINMASK );

void MDR1986VE1T_ClearPortPins ( MDR_PORT_TypeDef* pPORT, uint16_t PINMASK );

void MDR1986VE1T_SetPortValue ( MDR_PORT_TypeDef* pPORT, uint16_t Value );

void MDR1986VE1T_SetPortPin ( MDR_PORT_TypeDef* pPORT, uint16_t PIN );
uint32_t MDR1986VE1T_GetPortPin (  MDR_PORT_TypeDef* pPORT, uint16_t PIN );
void MDR1986VE1T_ClearPortPin ( MDR_PORT_TypeDef* pPORT, uint16_t PIN );

void MDR1986VE1T_OutViaDAC1 ( uint32_t Value );
void MDR1986VE1T_OutViaDAC2 ( uint32_t Value );
void MDR1986VE1T_SendViaEthernet ( void* pData, uint16_t DataSize );

uint32_t MDR1986VE1T_GetTimerCounter ( MDR_TIMER_TypeDef* TIMER );

void MDR1986VE1T_SendViaCAN ( MDR_CAN_TypeDef* pCAN, void* pData, uint16_t DataSize );
void MDR1986VE1T_WaitForCANMessage ( MDR_CAN_TypeDef* pCAN, uint32_t MASK, uint32_t FILTER );

void MDR1986VE1T_AddADCChannelToSwitchList ( MDR_ADC_TypeDef* pADC, uint32_t ChannelsFlags );
void MDR1986VE1T_DeleteADCChannelFromSwitchList ( MDR_ADC_TypeDef* pADC, uint32_t ChannelsFlags );

#endif // __DRIVER_MDR_ADC_H__
