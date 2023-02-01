#ifndef __PRECHARGE_CONTROLLER_H__
#define __PRECHARGE_CONTROLLER_H__

#include "stdint.h"

typedef enum {
  PRECHARGE_AUTO,
  PRECHARGE_SET_ON,
  PRECHARGE_WAIT_ON,
  PRECHARGE_IS_ON
} PrechargeControllerStates;

void CurrentRegulatorStartDischarge ();
void CurrentRegulatorStopDischarge ();
void CurrentRegulatorStartCharge ();
void CurrentRegulatorStopCharge ();

#endif // __PRECHARGE_CONTROLLER_H__
