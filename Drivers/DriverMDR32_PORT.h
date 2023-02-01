#ifndef __DRIVER_MDR_PORTA_H__
#define __DRIVER_MDR_PORTA_H__

#include "stdint.h"

void MDR32_PORTx_SETPINS ( MDR_PORT_TypeDef* pPORT, uint16_t PINMASK );
void MDR32_PORTx_CLEARPINS ( MDR_PORT_TypeDef* pPORT, uint16_t PINMASK );
void MDR32_PORTx_SETVALUE ( MDR_PORT_TypeDef* pPORT, uint16_t Value );
void MDR32_PORTx_SETPIN ( MDR_PORT_TypeDef* pPORT, uint16_t PIN );
void MDR32_PORTx_CLEARPIN ( MDR_PORT_TypeDef* pPORT, uint16_t PIN );

#endif // __DRIVER_MDR_PORTA_H__
