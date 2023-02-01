#include "main.h"
#include "Board.h"
#include "VoltageControl.h"
#include "PrechargeController.h"
#include "ChargeController.h"
#include "Subsystems.h"
#include "ConvContactorController.h"
#include "Measurements.h"
#include "Faults.h"
#include "Relays.h"
#include "Keyboard.h"
#include "Menu.h"

uint32_t tmpPrechargeControllerState;

void InitPrechargeController ( void ) {
  SetSubsystemState ( PRECHARGE_CONTROLLER, PRECHARGE_AUTO );
}

void AnalyzePrechargeController ( void ) {
}

uint64_t tmpPrechargeRelaySetOnTimeStamp = 0;

void DispatchPrechargeController ( void ) {
  tmpPrechargeControllerState = GetSubsystemState ( PRECHARGE_CONTROLLER );
  switch ( tmpPrechargeControllerState ) {
   case PRECHARGE_AUTO:
     if ( ( GetSubsystemState ( KEYBOARD ) == KEYBOARD_USED ) && ( GetSubsystemState ( CONV_CONTACTOR_CONTROLLER ) != CONV_CONTACTOR_IS_ON ) ) {
       SetSubsystemState ( PRECHARGE_CONTROLLER, PRECHARGE_SET_ON );
     }
     else {
       SetRelayState ( PRECHARGE_RELAY, OFF );
     }
     break;
   case PRECHARGE_SET_ON:
     GetTimeStamp ( &tmpPrechargeRelaySetOnTimeStamp );
     SetSubsystemState ( PRECHARGE_CONTROLLER, PRECHARGE_WAIT_ON );
     SetRelayState ( PRECHARGE_RELAY, ON );
     break;
   case PRECHARGE_WAIT_ON:
     if ( IsIntervalOverFrom ( (uint64_t)192000000, tmpPrechargeRelaySetOnTimeStamp ) ) {
       SetSubsystemState ( PRECHARGE_CONTROLLER, PRECHARGE_IS_ON );
     }
     break;
   case PRECHARGE_IS_ON: // Если предзаряд включен
		 if ( IsIntervalOverFrom ( (uint64_t)480000000, &tmpPrechargeRelaySetOnTimeStamp ) ) { // Реле предзаряда удерживается во включенном состоянии не меньше 5 секунд, после которых...
		   // если включен контактор преобразователя, то предзаряд уже не нужен
		   if ( GetSubsystemState ( CONV_CONTACTOR_CONTROLLER ) == CONV_CONTACTOR_IS_ON ) { SetSubsystemState ( PRECHARGE_CONTROLLER, PRECHARGE_AUTO ); }
		   // если нет нажатий на клавиши, то предзаряд уже не нужен
		   if ( GetSubsystemState ( KEYBOARD ) != KEYBOARD_USED ) { SetSubsystemState ( PRECHARGE_CONTROLLER, PRECHARGE_AUTO ); }
		 }
     break;
  }
}
