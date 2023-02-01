#include "MDR1986VE1T.h"
#include "Init1986VE1T.h"
#include "Subsystems.h"
#include "DriverMDR1986VE1T.h"
#include "Board.h"
#include "EEPROM.h"
#include "Calibrations.h"
#include "ChargeController.h"
#include "References.h"
#include "Relays.h"

uint64_t tmpTimeStamp2;
int32_t Value;
uint32_t TheFlag = 0;

int main () {
  InitMeasurements ();
  Init1986VE1T ();

  NVIC_EnableIRQ ( ADC_IRQn );
  
  InitSubsystems ();
  
  SetRelayState ( HIGHLIGHT, ON );
  
  while (1) {
    AnalyzeSubsystems ();
    DispatchSubsystems ();
  }
}
