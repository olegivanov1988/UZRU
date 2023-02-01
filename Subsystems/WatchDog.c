#include "main.h"
#include "Board.h"
#include "VoltageControl.h"
#include "TimeControl.h"

uint64_t PowerOnTimeStamp;
uint32_t ChargeIsOver;

void InitVoltageControl ( ) {
  ChargeIsOver = 0;
  R4VBoard_BeginSmoothStart ();
  GetTimeStamp ( &PowerOnTimeStamp );
}

void DispatchVoltageControl ( ) {
  if ( IsIntervalOverFrom ( 96000000 - 4 * 9600000, PowerOnTimeStamp ) ) {
    R4VBoard_OverSmoothStart ();
  }
}

void AnalyzeVoltageControl ( ) {
  
}
