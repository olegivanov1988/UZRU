#include "Faults.h"
#include "Subsystems.h"
#include "Engine.h"
#include "R4V.h"
#include "Board.h"

typedef struct {
	uint32_t State;
	uint32_t Actions;
} FaultStruct;

uint32_t FaultsActions;

FaultStruct Faults[NumOfFaults] = {
	{ OFF, 0 }, 														// FAULT_LOW_FREQUENCY - обороты дизель-генератора ниже нормы. Решение о реакции принимает САУ СЭС
	{ OFF, FAULTACTION_NEED_STOP_ENGINE }, 	// FAULT_HIGH_FREQUENCY - обороты дизель-генератора экстремально выше нормы. Останов дизеля
	{ OFF, FAULTACTION_CANT_START_ENGINE | FAULTACTION_NEED_STOP_ENGINE }, // FAULT_ANGLE_SENSOR_SHORTCIRQUIT - КЗ датчика угла электромагнита - останов дизеля, запрет пуска дизеля
	{ OFF, FAULTACTION_CANT_START_ENGINE | FAULTACTION_NEED_STOP_ENGINE }, // FAULT_ANGLE_SENSOR_DISCONNECTION - обрыв датчика угла электромагнита - останов дизеля, запрет пуска дизеля
	{ OFF, FAULTACTION_CANT_START_ENGINE | FAULTACTION_NEED_STOP_ENGINE }, // FAULT_ROTATION_SENSOR_SHORTCIRQUIT - КЗ датчика оборотов - останов дизеля, запрет пуска дизеля
	{ OFF, FAULTACTION_CANT_START_ENGINE | FAULTACTION_NEED_STOP_ENGINE }, // FAULT_ROTATION_SENSOR_DISCONNECTION - обрыв датчика оборотов - останов дизеля, запрет пуска дизеля
	{ OFF, FAULTACTION_CANT_START_ENGINE | FAULTACTION_NEED_STOP_ENGINE }, // FAULT_LOW_VOLTAGE - низкое напряжение питания - запрет пуска дизеля, останов дизеля
	{ OFF, FAULTACTION_CANT_START_ENGINE | FAULTACTION_NEED_STOP_ENGINE | FAULTACTION_CANT_ON_SHUNT | FAULTACTION_NEED_OFF_SHUNT }, // FAULT_HIGH_VOLTAGE - высокое напряжение питания
	{ OFF, FAULTACTION_CANT_ON_SHUNT | FAULTACTION_CANT_START_ENGINE }, // FAULT_NO_CAPACITORS_CHARGE - нет подзаряда конденсаторов - запрет включения шунта, запрет пуска дизеля
	{ OFF, FAULTACTION_NEED_STOP_ENGINE | FAULTACTION_CANT_START_ENGINE | FAULTACTION_NEED_OFF_SHUNT | FAULTACTION_CANT_ON_SHUNT } // FAULT_SHORT_CIRQUIT - КЗ - запрет всего, останов всего
};

void CalculateFaultsActions ( void );

void CheckFaults ( void ) {
	// Проверка частоты вращения дизеля
	if ( GetEngineFreqx100 () > 170000 ) { SetFault ( FAULT_HIGH_FREQUENCY ); }
}

void DispatchFaults ( void ) {
	if ( ( GetFaultsActions () & FAULTACTION_NEED_STOP_ENGINE ) && ( GetSubsystemState ( Engine ) != ENGINE_NOT_ROTATES ) ) {
		if ( ( GetSubsystemState ( R4V ) != R4V_STOP_ENGINE ) && ( GetSubsystemState ( R4V ) != R4V_WAIT_FOR_STOP_ENGINE ) ) {
			SetSubsystemState ( R4V, R4V_STOP_ENGINE );
		}
	}
	else {
		if ( GetFaultsActions () & FAULTACTION_NEED_OFF_SHUNT ) {
			R4VBoard_BeginSmoothStart ();
		}
	}
}

void SetFault ( FaultIndex_Typedef FaultIndex ) {
	Faults[FaultIndex].State = ON;
	FaultsActions |= Faults[FaultIndex].Actions;
}

void ClearFault ( FaultIndex_Typedef FaultIndex ) {
	Faults[FaultIndex].State = OFF;
	CalculateFaultsActions ();
}

void ClearFaults ( void ) {
	uint32_t FaultIndex;
	FaultsActions = 0;
	for ( FaultIndex = 0; FaultIndex < NumOfFaults; FaultIndex++ ) {
		Faults[FaultIndex].State = OFF;
	}
}

void CalculateFaultsActions () {
	uint32_t FaultIndex;
	FaultsActions = 0;
	for ( FaultIndex = 0; FaultIndex < NumOfFaults; FaultIndex++ ) {
		if ( Faults[FaultIndex].State == ON ) FaultsActions |= Faults[FaultIndex].Actions;
	}
}

uint32_t GetFaultsActions () {
	return FaultsActions;
}

