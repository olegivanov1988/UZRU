#ifndef __PAYLOAD_CONTROLLER_H__
#define __PAYLOAD_CONTROLLER_H__

#include "stdint.h"

typedef enum {
   PAYLOAD_IS_LOCKED_BY_SOMEBODY,
   PAYLOAD_IS_FREE,
   TRY_LOCK_PAYLOAD,
   WAIT_FOR_PAYLOAD_LOCKING,
   PAYLOAD_IS_LOCKED_BY_ME,
   PAUSE_USE_PAYLOAD,
   USE_PAYLOAD_PAUSED,
   PAYLOAD_CONTROLLER_FREE_PAYLOAD,
   WAIT_FOR_PAYLOAD_FREE
} PayloadControllerStates;

void CurrentRegulatorStartDischarge ();
void CurrentRegulatorStopDischarge ();
void CurrentRegulatorStartCharge ();
void CurrentRegulatorStopCharge ();

#endif // __PAYLOAD_CONTROLLER_H__
