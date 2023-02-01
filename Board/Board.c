#include "main.h"
#include "DriverMDR1986VE1T.h"
#include "TimeControl.h"
#include "References.h"
#include "Measurements.h"
#include "Board.h"
#include "Relays.h"

uint64_t SolenoidAngleTimeStamp;
int32_t CurrentAngle;

void Board_Init () {
}

void Board_SetChargeCurrent ( int32_t TargetValue ) {// ճ鿬 󱲠쩢Όῠ讠󥭨塶欥㯣﫠 믭򰮫즰塘Ɍ
  if ( TargetValue < 0 ) {
    MDR_DAC->DAC1_DATA = (-TargetValue) & 0x00000FFF;
  }
  else {
    MDR_DAC->DAC1_DATA = TargetValue & 0x00000FFF;
  }
}

void Board_SetChargeVoltage ( int32_t TargetValue ) {
  if ( TargetValue < 0 ) {
    MDR_DAC->DAC2_DATA = (-TargetValue) & 0x00000FFF;
  }
  else {
    MDR_DAC->DAC2_DATA = TargetValue & 0x00000FFF;
  }
}

void Board_StartCharge ( ) {      
  MDR1986VE1T_SetPortPin ( MDR_PORTD, 0 );
}

void Board_StopCharge ( ) {
  // MDR1986VE1T_ClearPortPin ( MDR_PORTD, 0 );
}

void Board_StartDischarge ( ) {      
  MDR1986VE1T_SetPortPin ( MDR_PORTD, 1 );
}

void Board_StopDischarge ( ) {
  MDR1986VE1T_ClearPortPin ( MDR_PORTD, 1 );
}

void Board_StopAll ( ) {
  Board_StopCharge ( );
  Board_StopDischarge ( );
}

uint32_t Board_IsLoadFree () {        // Ƞ䬳𪠮
  if ( MDR_PORTD->RXTX & ( 1 << 2 ) ) {
    return 1;
  }
  else {
    return 0;
  }
}

/*
void Board_LockLoad ( ) {
  MDR_PORTD->OE |= 1 << 2;
  MDR_PORTD->CLRTX = 1 << 2;
}

void Board_UnlockLoad ( ) {
  MDR_PORTD->OE &= ( 1 << 2 );
  MDR_PORTD->SETTX = 1 << 2;
}*/

uint32_t Board_IsLoadLocked ( ) {
  if ( MDR_PORTD->RXTX & ( 1 << 2 ) ) {
    return 0;
  }
  else {
    return 1;
  }
}
/*
uint32_t Board_ReserveLoad () {
  MDR_PORTD->CLRTX = ( 1 << 3 );
}

uint32_t Board_FreeLoad () {
  MDR_PORTD->SETTX = ( 1 << 3 );
}*/

uint32_t Board_GetSolenoidCurrent () {        // Ƞ䬳𪠮
  return 0;
}

int32_t Board_GetSolenoidAngle () {          // ճ鿬 㯧㱠񠾹ῠ󣮫 ௢ﱮ򠠱ﬥ堍
  return CurrentAngle; // = ( 2534 - ( MDR_ADC->ADC1_RESULT & 0x00000FFF ) );  // Š򷨪 󣫠 ௤묾󥭠ꡀ׏ 򪡺򻣰שׂ池, ﳱ󲱲㩥 ௢ﱮ򠠱ﯲ㦲񲢳沠讠󥭨򠲵34, 񠳢欨󥭨欠󣫠 讠󥭨塳񻒢𙚇􃐊}
}

void Board_GetSolenoidAngleTimeStamp ( uint64_t* pDst ) {
  *pDst = SolenoidAngleTimeStamp;
}

void Board_TurnOnGreenEthernetLed () {        // ճ鿬 衦餠򹠿 覫٭󩠱㦲泌鸞񮥤鮨򥫥 Ethernet
  MDR1986VE1T_SetPortPin ( MDR_PORTB, 14 );        // Ա򠭠㬨㡥졢󢮤 0 ௰򠠁, 򮪮 񢥲泌鸞衦餠泱󠮲 񲮣󢮤͊}
}

void Board_TurnOffGreenEthernetLed () {        // ճ鿬 䡱󹠿 覫٭󩠱㦲泌鸞񮥤鮨򥫥 Ethernet
  MDR1986VE1T_ClearPortPin ( MDR_PORTB, 14 );        // ҡ𠱻㡥졢󢮤 0 ௰򠠁, 򮪮 񢥲泌鸞衦餠泱󠮲 񲮣󢮤͊}
}

void Board_TurnOnYellowEthernetLed () {        // ճ鿬 衦餠򹠿 禫򻩠񢥲泌鸞񮥤鮨򥫥 Ethernet
  MDR1986VE1T_SetPortPin ( MDR_PORTB, 15 );        // Ա򠭠㬨㡥졢󢮤 1 ௰򠠁, 򮪮 񢥲泌鸞衦餠泱󠮲 񲮣󢮤͊}
}

void Board_TurnOffYellowEthernetLed () {        // ճ鿬 䡱󹠿 禫򻩠񢥲泌鸞񮥤鮨򥫥 Ethernet
  MDR1986VE1T_ClearPortPin ( MDR_PORTB, 15 );        // ҡ𠱻㡥졢󢮤 1 ௰򠠁, 򮪮 񢥲泌鸞衦餠泱󠮲 񲮣󢮤͊}
}

void Board_TurnOffWorkLed () {            // ճ鿬 㫫򷠾񠿠覫٭󩠱㦲泌鸞ଠ򥠐؂
  MDR_PORTD->OE &= ~( 1 << 6 );
  MDR1986VE1T_SetPortPin ( MDR_PORTD, 6 );        // Ա򠭠㬨㡥졢󢮤 14 ௰򠠁, 򮪮 񢥲泌鸞衦餠泱󠮲 񲮣󢮤͊}
}

void Board_TurnOnWorkLed () {            // ճ鿬 ﳪ쿷Όῠ覫٭󩠱㦲泌鸞ଠ򥠐؂
  MDR_PORTD->OE |= (1 << 6 );
  MDR1986VE1T_ClearPortPin ( MDR_PORTD, 6 );        // ҡ𠱻㡥졢󢮤 14 ௰򠠁, 򮪮 񢥲泌鸞衦餠泱󠮲 񲮣󢮤͊}
}

void Board_TurnOffFaultLed () {              // ճ鿬 㫫򷠾񠿠뱠񭻩 񢥲泌鸞ଠ򥠐؂
  MDR_PORTD->OE &= ~( 1 << 5 );
  MDR1986VE1T_SetPortPin ( MDR_PORTD, 5 );        // Ա򠭠㬨㡥졢󢮤 15 ௰򠠁, 򮪮 񢥲泌鸞衦餠泱󠮲 񲮣󢮤͊}
}

void Board_TurnOnFaultLed () {            // ճ鿬 ﳪ쿷Όῠ뱠񭻩 񢥲泌鸞ଠ򥠐؂
  MDR_PORTD->OE = MDR_PORTD->OE | (1 << 5 );
  MDR1986VE1T_ClearPortPin ( MDR_PORTD, 5 );        // ҡ𠱻㡥졢󢮤 15 ௰򠠁, 򮪮 񢥲泌鸞衦餠泱󠮲 񲮣󢮤͊}
}

void Board_TurnOnHighlight () {              // ճ鿬 㫫򷠾񠿠뱠񭻩 񢥲泌鸞ଠ򥠐؂
  MDR_PORTD->OE |= (1 << 4 );
  MDR1986VE1T_ClearPortPin ( MDR_PORTD, 4 );        // ҡ𠱻㡥졢󢮤 15 ௰򠠁, 򮪮 񢥲泌鸞衦餠泱󠮲 񲮣󢮤͊}
}

void Board_TurnOffHighlight () {            // ճ鿬 ﳪ쿷Όῠ뱠񭻩 񢥲泌鸞ଠ򥠐؂
  MDR_PORTD->OE &= ~( 1 << 4 );
  MDR1986VE1T_SetPortPin ( MDR_PORTD, 4 );        // Ա򠭠㬨㡥졢󢮤 15 ௰򠠁, 򮪮 񢥲泌鸞衦餠泱󠮲 񲮣󢮤͊}
}

uint32_t Board_GetButtonsState () {
  return ( ( ( MDR_PORTA->RXTX ) >> 12 ) | ( ( MDR_PORTB->RXTX ) << 4 ) );
}

void Board_OnADCConversion ( uint32_t Channel, uint32_t PreValue ) {
  if ( Channel > 1 ) {
    RegisterMeasure ( Channel - 2, PreValue );
  }
}

void DispatchBoard ( void ) {
}

void Board_SetDischargeCurrent ( int32_t NewValue ) {
  if ( NewValue > MAX_DISCHARGE_SIGNAL ) {
    MDR_TIMER1->CCR11 = MAX_DISCHARGE_SIGNAL;
		MDR_TIMER1->CCR21 = 2 * MAX_DISCHARGE_SIGNAL;
  }
  else if ( NewValue < 0 ) {
    MDR_TIMER1->CCR11 = 0;
		MDR_TIMER1->CCR21 = MAX_DISCHARGE_SIGNAL;
  }
  else {
    MDR_TIMER1->CCR11 = NewValue;
		MDR_TIMER1->CCR21 = MAX_DISCHARGE_SIGNAL + NewValue;
  }
}
