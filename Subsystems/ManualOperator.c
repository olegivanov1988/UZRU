#include "main.h"
#include "Subsystems.h"
#include "Board.h"
#include "ManualOperator.h"

void InitManualOperator ( void ) {
  SetSubsystemState ( MANUAL_OPERATOR, WANT_NOTHING );
}

void DispatchManualOperator ( void ) {
  uint32_t tmpManualOperatorState = GetSubsystemState ( MANUAL_OPERATOR );

}

void AnalyzeManualOperator ( void ) {
/*  if ( Board_GetStopButtonState () == PUSHED ) {
    SetSubsystemState ( MANUAL_OPERATOR, MANUAL_OPERATOR_WANT_STOP_ENGINE );
  } */
}
