#include "main.h"
#include "Board.h"
#include "VoltageControl.h"
#include "ConvContactorController.h"
#include "PrechargeController.h"
#include "ChargeController.h"
#include "Subsystems.h"
#include "Measurements.h"
#include "Faults.h"
#include "Relays.h"
#include "Keyboard.h"
#include "Menu.h"

uint32_t tmpConvContactorControllerState;

void InitConvContactorController ( void ) {
  SetSubsystemState ( CONV_CONTACTOR_CONTROLLER, CONV_CONTACTOR_SET_OFF );
}

void AnalyzeConvContactorController ( void ) {
}

uint64_t tmpConvContactorSetTimeStamp = 0;

void DispatchConvContactorController ( void ) {
  int32_t tmpConvContactorControllerState = GetSubsystemState ( CONV_CONTACTOR_CONTROLLER );
  switch ( tmpConvContactorControllerState ) {
    case CONV_CONTACTOR_SET_ON:
			if ( GetSubsystemState ( PRECHARGE_CONTROLLER ) == PRECHARGE_AUTO ) {
				SetSubsystemState ( PRECHARGE_CONTROLLER, PRECHARGE_SET_ON );
			}
			else if ( GetSubsystemState ( PRECHARGE_CONTROLLER ) == PRECHARGE_IS_ON ) {
        GetTimeStamp ( &tmpConvContactorSetTimeStamp );
        SetRelayState ( CONVERTER_CONTACTOR, ON );
        SetSubsystemState ( CONV_CONTACTOR_CONTROLLER, CONV_CONTACTOR_WAIT_ON );
			}
      break;
    case CONV_CONTACTOR_WAIT_ON:
      if ( IsIntervalOverFrom ( (uint64_t)24000000, tmpConvContactorSetTimeStamp ) ) { SetSubsystemState ( CONV_CONTACTOR_CONTROLLER, CONV_CONTACTOR_IS_ON ); }
      break;
    case CONV_CONTACTOR_IS_ON:
      break;
    case CONV_CONTACTOR_SET_OFF:
      GetTimeStamp ( &tmpConvContactorSetTimeStamp );
      SetRelayState ( CONVERTER_CONTACTOR, OFF );
      SetSubsystemState ( CONV_CONTACTOR_CONTROLLER, CONV_CONTACTOR_WAIT_OFF );
      break;
    case CONV_CONTACTOR_WAIT_OFF:
      if ( IsIntervalOverFrom ( (uint64_t)24000000, tmpConvContactorSetTimeStamp ) ) { SetSubsystemState ( CONV_CONTACTOR_CONTROLLER, CONV_CONTACTOR_IS_OFF ); }
      break;
    case CONV_CONTACTOR_IS_OFF:
      break;
  }
}
