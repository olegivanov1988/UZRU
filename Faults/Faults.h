#ifndef __FAULTS_H__
#define __FAULTS_H__

#include "stdint.h"

typedef enum {
	OFF,
	ON
} FaultState_Typedef;

typedef enum {
	FAULT_LOW_FREQUENCY,
	FAULT_HIGH_FREQUENCY,
	FAULT_ANGLE_SENSOR_SHORTCIRQUIT,
	FAULT_ANGLE_SENSOR_DISCONNECTION,
	FAULT_ROTATION_SENSOR_SHORTCIRQUIT,
	FAULT_ROTATION_SENSOR_DISCONNECTION,
	FAULT_LOW_VOLTAGE,
	FAULT_HIGH_VOLTAGE,
	FAULT_NO_CAPACITORS_CHARGE,
	FAULT_SHORT_CIRQUIT,
	NumOfFaults
} FaultIndex_Typedef;

typedef enum {
	FAULTACTION_CANT_START_ENGINE,
	FAULTACTION_NEED_STOP_ENGINE,
	FAULTACTION_CANT_ON_SHUNT,
	FAULTACTION_NEED_OFF_SHUNT,
} FaultActions_Typedef;

void SetFault ( FaultIndex_Typedef FaultIndex );
void ClearFault ( FaultIndex_Typedef FaultIndex );
void ClearFaults ( );
uint32_t GetFaultsActions ();

#endif // __FAULTS_H__