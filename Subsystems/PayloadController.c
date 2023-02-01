#include "main.h"
#include "Board.h"
#include "VoltageControl.h"
#include "PayloadController.h"
#include "Subsystems.h"
#include "Measurements.h"
#include "Faults.h"
#include "Relays.h"
#include "Menu.h"

extern uint32_t MenuIndex;

uint32_t tmpPayloadControllerState;
uint32_t lastPayloadControllerState;

void InitPayloadController ( void ) {
  SetSubsystemState ( PAYLOAD_CONTROLLER, PAYLOAD_IS_FREE );
  tmpPayloadControllerState = lastPayloadControllerState = PAYLOAD_IS_FREE;
}

uint64_t PayloadControllerTimeStamp;

void AnalyzePayloadController ( void ) {
}

uint32_t PayloadContactorIsOn = 0;
uint32_t PayloadIsLocked = 0;

void DispatchPayloadController ( void ) {
  tmpPayloadControllerState = GetSubsystemState ( PAYLOAD_CONTROLLER );
  switch ( tmpPayloadControllerState ) {
    case PAYLOAD_IS_FREE:
      if ( Board_IsLoadLocked () ) { SetSubsystemState ( PAYLOAD_CONTROLLER, PAYLOAD_IS_LOCKED_BY_SOMEBODY ); }
      break;
   case PAYLOAD_IS_LOCKED_BY_SOMEBODY:
      if ( !Board_IsLoadLocked () ) { SetSubsystemState ( PAYLOAD_CONTROLLER, PAYLOAD_IS_FREE ); }
      break;
   case TRY_LOCK_PAYLOAD:
     if ( !PayloadIsLocked && Board_IsLoadLocked () ) {
       SetFault ( FAULT_PAYLOAD_IS_LOCKED );
       SetRelayState ( LOCK_PAYLOAD, OFF );
       SetRelayState ( LOAD_CONTACTOR, OFF );
       SetSubsystemState ( PAYLOAD_CONTROLLER, PAYLOAD_IS_LOCKED_BY_SOMEBODY );
     }
     else {
       GetTimeStamp ( &PayloadControllerTimeStamp );
       SetSubsystemState ( PAYLOAD_CONTROLLER, WAIT_FOR_PAYLOAD_LOCKING );
       SetRelayState ( LOCK_PAYLOAD, ON );
       PayloadIsLocked = 1;
       SetRelayState ( LOAD_CONTACTOR, ON );
     }
     break;
   case WAIT_FOR_PAYLOAD_LOCKING:
     if ( IsIntervalOverFrom ( (uint64_t)36000000, PayloadControllerTimeStamp ) ) {
      if ( Board_IsLoadLocked () ) {
        PayloadContactorIsOn = 1;
        SetSubsystemState ( PAYLOAD_CONTROLLER, PAYLOAD_IS_LOCKED_BY_ME );
      }
      else {
        PayloadContactorIsOn = 1;
        PayloadIsLocked = 0;
        SetRelayState ( LOCK_PAYLOAD, OFF );
        SetRelayState ( LOAD_CONTACTOR, OFF );
        SetFault ( PAYLOAD_LOCKING_FAULT );
        GetTimeStamp ( &PayloadControllerTimeStamp );
        SetSubsystemState ( PAYLOAD_CONTROLLER, WAIT_FOR_PAYLOAD_FREE );
      }
    }
     break;
   case PAUSE_USE_PAYLOAD:
     SetRelayState ( LOAD_CONTACTOR, OFF );
     PayloadContactorIsOn = 0;
     SetSubsystemState ( PAYLOAD_CONTROLLER, USE_PAYLOAD_PAUSED );
     break;
   case USE_PAYLOAD_PAUSED:
     break;
   case PAYLOAD_CONTROLLER_FREE_PAYLOAD:
     GetTimeStamp ( &PayloadControllerTimeStamp );
     SetRelayState ( LOCK_PAYLOAD, OFF );
     PayloadIsLocked = 0;
     SetRelayState ( LOAD_CONTACTOR, OFF );
     SetSubsystemState ( PAYLOAD_CONTROLLER, WAIT_FOR_PAYLOAD_FREE );
     break;
   case PAYLOAD_IS_LOCKED_BY_ME:
     break;
   case WAIT_FOR_PAYLOAD_FREE:
     if ( IsIntervalOverFrom ( (uint64_t)36000000, PayloadControllerTimeStamp ) ) {
      if ( !Board_IsLoadLocked () ) {
        PayloadContactorIsOn = 0;
        SetSubsystemState ( PAYLOAD_CONTROLLER, PAYLOAD_IS_FREE );
      }
      else {
        SetFault ( PAYLOAD_FREE_FAULT );
      }
    }
    break;
  }
}

void ReservePayload ( void ) {
}

void FreePayload ( void ) {
}
