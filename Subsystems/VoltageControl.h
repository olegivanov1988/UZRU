#ifndef __VOLTAGE_CONTROL_H__
#define __VOLTAGE_CONTROL_H__

#include "stdint.h"

typedef enum {
  VOLTAGE_CONTROL_WORKS
} eVoltageControlStates;

typedef enum {
  VOLTAGE_CONTROL_WANT_NOTHING
} eVoltageControlWants;

void InitVoltageControl ( void );
void AnalyzeVoltageControl ( void );
void DispatchVoltageControl ( void );

#endif // __VOLTAGE_CONTROL_H__
