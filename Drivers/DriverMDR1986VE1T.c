#include "MDR1986VE1T.h"
#include "DriverMDR32_ADC.h"
#include "DriverMDR32_DAC.h"
#include "DriverMDR32_EEPROM.h"
#include "DriverMDR32_PORT.h"
#include "DriverMDR32_RST_CLK.h"
#include "DriverMDR32_TIMER.h"

// Функции портов
void MDR1986VE1T_SetPortPins ( MDR_PORT_TypeDef* pPORT, uint16_t PINMASK ) {
  MDR32_PORTx_SETPINS ( pPORT, PINMASK );
}

void MDR1986VE1T_ClearPortPins ( MDR_PORT_TypeDef* pPORT, uint16_t PINMASK ) {
  MDR32_PORTx_CLEARPINS ( pPORT, PINMASK );
}

void MDR1986VE1T_SetPortValue ( MDR_PORT_TypeDef* pPORT, uint16_t Value ) {
  pPORT->RXTX = Value;
}

void MDR1986VE1T_SetPortPin ( MDR_PORT_TypeDef* pPORT, uint16_t PIN ) {
  MDR32_PORTx_SETPIN ( pPORT, PIN );
}

void MDR1986VE1T_ClearPortPin ( MDR_PORT_TypeDef* pPORT, uint16_t PIN ) {
  MDR32_PORTx_CLEARPIN ( pPORT, PIN );
}
// Функции ЦАП
void MDR1986VE1T_OutViaDAC1 ( uint32_t Value ) {
  MDR32_OutViaDAC1 ( Value );
}

void MDR1986VE1T_OutViaDAC2 ( uint32_t Value ){
}
/*// Функции контроллера Ethernet
void MDR1986VE1T_SendViaEthernet ( void* pData, uint16_t DataSize ) {                  // Помещает данные в буфер передатчика Ethernet и ставит их в очередь отправки
  MDR32_ETHERNET_SendViaETHERNET ( MDR1986VE1T_ETHERNET, pData, DataSize );
}
*/

// Функции АЦП
void MDR1986VE1T_ADC1Start ( void );                                                        // Запускает преобразование АЦП1
void MDR1986VE1T_ADC2Start ( void );                                                        // Запускает преобразование АЦП2
void OnADCConversion ( void );                                                              // Обработчик преобразования АЦП

uint32_t MDR1986VE1T_GetTimerCounter ( MDR_TIMER_TypeDef* TIMER ) {
  return TIMER->CNT;
}

uint32_t MDR1986VE1T_GetPortPin (  MDR_PORT_TypeDef* pPORT, uint16_t PIN ) {
  if ( pPORT->RXTX & ( 1 << PIN ) ) {
    return 1;
  }
  else {
    return 0;
  }
}
