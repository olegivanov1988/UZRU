#include "MDR1986VE1T.h"

void MDR32_PORTx_SETPINS ( MDR_PORT_TypeDef* pPORT, uint16_t PINMASK ) {
  pPORT->SETTX = PINMASK;
}

void MDR32_PORTx_CLEARPINS ( MDR_PORT_TypeDef* pPORT, uint16_t PINMASK ) {
  pPORT->CLRTX = PINMASK;
}

void MDR32_PORTx_SETVALUE ( MDR_PORT_TypeDef* pPORT, uint16_t Value ) {
  pPORT->RXTX = Value;
}

void MDR32_PORTx_SETPIN ( MDR_PORT_TypeDef* pPORT, uint16_t PIN ) {
  pPORT->SETTX = 1 << PIN;
}

void MDR32_PORTx_CLEARPIN ( MDR_PORT_TypeDef* pPORT, uint16_t PIN ) {
  pPORT->CLRTX = 1 << PIN;
}
