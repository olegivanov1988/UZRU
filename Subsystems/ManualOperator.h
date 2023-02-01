#ifndef __MANUAL_OPERATOR_H__
#define __MANUAL_OPERATOR_H__

#include "stdint.h"

typedef enum {
  MANUAL_OPERATOR_WORKS = 0
} eManualOperatorStates;

typedef enum {
  WANT_NOTHING,
// Команды общего назначения
  WANTS_START_CONST_U_CHARGE,
  WANTS_START_CONST_I_CHARGE,
  WANTS_START_COMBINE_CHARGE,
  WANTS_START_ASIMM_CHARGE,
  WANTS_START_CONTROL_DISCHARGE,
  WANTS_STOP_CHARGE,
  WANTS_PAUSE_CHARGE
} eManualOperatorWants;

void InitManualOperator ( void );
void DispatchManualOperator ( void );
void AnalyzeManualOperator ( void );

#endif // __MANUAL_OPERATOR_H__
