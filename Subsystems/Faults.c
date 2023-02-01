#include "Faults.h"
#include "Subsystems.h"
#include "Board.h"
#include "Menu.h"
#include "Faults.h"
#include "Relays.h"
#include "TimeControl.h"
#include "ChargeController.h"
#include "Measurements.h"
#include "References.h"
#include "ConvContactorController.h"
#include "UZRU10A.h"
#include "PrechargeController.h"

typedef struct {
  uint32_t State;
  uint32_t Actions;
} FaultStruct;

uint32_t NeedActionsUpdate = FALSE;

uint32_t FaultsCount = 0;

uint32_t FaultsActions = 0;

uint32_t FaultsStates [NUM_OF_FAULTS];
uint32_t FaultActions [NUM_OF_FAULTS] = {  
/* FAULT_NO_BATTERY              */ FAULT_ACTION_NEED_STOP_CHARGE | FAULT_ACTION_NEED_STOP_DISCHARGE | FAULT_ACTION_NEED_OFF_CONV_CONTACTOR | FAULT_ACTION_NEED_OFF_PAYLOAD_CONTACTOR,
/* FAULT_BATTERY_SHORT_CIRQUIT   */ FAULT_ACTION_NEED_STOP_CHARGE | FAULT_ACTION_NEED_STOP_DISCHARGE | FAULT_ACTION_NEED_OFF_CONV_CONTACTOR | FAULT_ACTION_NEED_OFF_PAYLOAD_CONTACTOR,
/* PAYLOAD_LOCKING_FAULT         */ FAULT_ACTION_NEED_STOP_DISCHARGE | FAULT_ACTION_NEED_OFF_PAYLOAD_CONTACTOR,
/* FAULT_PAYLOAD_IS_LOCKED       */ FAULT_ACTION_NEED_STOP_DISCHARGE | FAULT_ACTION_NEED_OFF_PAYLOAD_CONTACTOR,
/* PAYLOAD_FREE_FAULT            */ FAULT_ACTION_NEED_STOP_CHARGE | FAULT_ACTION_NEED_STOP_DISCHARGE | FAULT_ACTION_NEED_OFF_CONV_CONTACTOR | FAULT_ACTION_NEED_OFF_PAYLOAD_CONTACTOR,
/* FAULT_PAYLOAD_SHORT_CIRQUIT   */ FAULT_ACTION_NEED_STOP_DISCHARGE | FAULT_ACTION_NEED_OFF_PAYLOAD_CONTACTOR,
/* FAULT_LOW_NETWORK_VOLTAGE     */ FAULT_ACTION_NEED_STOP_CHARGE | FAULT_ACTION_NEED_STOP_DISCHARGE | FAULT_ACTION_NEED_OFF_CONV_CONTACTOR | FAULT_ACTION_NEED_OFF_PAYLOAD_CONTACTOR, 
/* FAULT_HIGH_NETWORK_VOLTAGE    */ FAULT_ACTION_NEED_STOP_CHARGE | FAULT_ACTION_NEED_STOP_DISCHARGE | FAULT_ACTION_NEED_OFF_CONV_CONTACTOR | FAULT_ACTION_NEED_OFF_PAYLOAD_CONTACTOR, 
/* FAULT_LOW_CONVERTER_VOLTAGE   */ FAULT_ACTION_NEED_STOP_CHARGE | FAULT_ACTION_NEED_STOP_DISCHARGE | FAULT_ACTION_NEED_OFF_CONV_CONTACTOR | FAULT_ACTION_NEED_OFF_PAYLOAD_CONTACTOR, 
/* FAULT_HIGH_CONVERTER_VOLTAGE  */ FAULT_ACTION_NEED_STOP_CHARGE | FAULT_ACTION_NEED_STOP_DISCHARGE | FAULT_ACTION_NEED_OFF_CONV_CONTACTOR | FAULT_ACTION_NEED_OFF_PAYLOAD_CONTACTOR, 
/* FAULT_LOW_BATTERY_VOLTAGE     */ FAULT_ACTION_NEED_STOP_CHARGE | FAULT_ACTION_NEED_STOP_DISCHARGE | FAULT_ACTION_NEED_OFF_CONV_CONTACTOR | FAULT_ACTION_NEED_OFF_PAYLOAD_CONTACTOR, 
/* FAULT_HIGH_BATTERY_VOLTAGE    */ FAULT_ACTION_NEED_STOP_CHARGE | FAULT_ACTION_NEED_STOP_DISCHARGE | FAULT_ACTION_NEED_OFF_CONV_CONTACTOR | FAULT_ACTION_NEED_OFF_PAYLOAD_CONTACTOR, 
/* FAULT_NO_CHARGE_CURRENT       */ FAULT_ACTION_NEED_STOP_CHARGE | FAULT_ACTION_NEED_STOP_DISCHARGE | FAULT_ACTION_NEED_OFF_CONV_CONTACTOR | FAULT_ACTION_NEED_OFF_PAYLOAD_CONTACTOR, 
/* FAULT_NO_DISCHARGE_CURRENT    */ FAULT_ACTION_NEED_STOP_CHARGE | FAULT_ACTION_NEED_STOP_DISCHARGE | FAULT_ACTION_NEED_OFF_CONV_CONTACTOR | FAULT_ACTION_NEED_OFF_PAYLOAD_CONTACTOR, 
/* FAULT_PRECHARGE_FAULT         */ FAULT_ACTION_NEED_STOP_CHARGE | FAULT_ACTION_NEED_STOP_DISCHARGE | FAULT_ACTION_NEED_OFF_CONV_CONTACTOR | FAULT_ACTION_NEED_OFF_PAYLOAD_CONTACTOR, 
/* FAULT_MODE_CANCEL             */ FAULT_ACTION_NEED_STOP_CHARGE | FAULT_ACTION_NEED_STOP_DISCHARGE | FAULT_ACTION_NEED_OFF_CONV_CONTACTOR | FAULT_ACTION_NEED_OFF_PAYLOAD_CONTACTOR, 
/* CHARGE_REGULATION_FAULT       */ FAULT_ACTION_NEED_STOP_CHARGE | FAULT_ACTION_NEED_STOP_DISCHARGE | FAULT_ACTION_NEED_OFF_CONV_CONTACTOR | FAULT_ACTION_NEED_OFF_PAYLOAD_CONTACTOR, 
/* DISCHARGE_REGULATION_FAULT    */ FAULT_ACTION_NEED_STOP_CHARGE | FAULT_ACTION_NEED_STOP_DISCHARGE | FAULT_ACTION_NEED_OFF_CONV_CONTACTOR | FAULT_ACTION_NEED_OFF_PAYLOAD_CONTACTOR, 
/* FAULT_RADIATOR_OVERHEAT       */ FAULT_ACTION_NEED_PAUSE_CHARGE | FAULT_ACTION_NEED_PAUSE_DISCHARGE | FAULT_ACTION_NEED_OFF_CONV_CONTACTOR | FAULT_ACTION_NEED_OFF_PAYLOAD_CONTACTOR,
/* FAULT_TEMP_SENSOR_DISCONNECT  */ 0
};

uint64_t LastLowNetworkVoltageTimeStamp;
uint64_t LastHighNetworkVoltageTimeStamp;
uint64_t LastLowFilterVoltageTimeStamp;
uint64_t LastHighFilterVoltageTimeStamp;
uint64_t LastLowBatteryVoltageTimeStamp;
uint64_t LastHighBatteryVoltageTimeStamp;

void InitFaults ( void ) {
  ClearFaults ();
  GetTimeStamp (&LastLowNetworkVoltageTimeStamp);
  GetTimeStamp (&LastHighNetworkVoltageTimeStamp);
  GetTimeStamp (&LastLowFilterVoltageTimeStamp);
  GetTimeStamp (&LastHighFilterVoltageTimeStamp);
  GetTimeStamp (&LastLowBatteryVoltageTimeStamp);
  GetTimeStamp (&LastHighBatteryVoltageTimeStamp);
}

uint32_t GetFaultsCount ( void ) {
  return FaultsCount;
}

void CalculateFaultsActions ( void ) {
  uint32_t tmpFaultIndex = 0;
  uint32_t tmpNumOfFaults = 0;
  FaultsActions = 0;
  while ( ( tmpNumOfFaults < FaultsCount ) && ( tmpFaultIndex < NUM_OF_FAULTS ) ) {
    if ( FaultsStates[tmpFaultIndex] ) {
      FaultsActions |= FaultActions[tmpFaultIndex];
      tmpNumOfFaults++;
    }
    tmpFaultIndex++;
  }
}

uint32_t IsFaultsActionIs ( uint32_t FaultActionMask ) {
  if ( FaultsActions & FaultActionMask ) {
    return 1;
  }
  else {
    return 0;
  }
}

uint32_t GetFaultsActions ( void ) {
  return FaultsActions;
}

void SetPreFault (uint32_t FaultIndex);
void ClearPreFault (uint32_t FaultIndex);
void SetFaultConfirmationIterval (uint32_t FaultIndex, uint64_t* pConfirmationInterval);
void SetFaultActions (uint32_t FaultIndex, uint32_t ActionsFlags);

extern int32_t* pMinFaultBatteryVoltage;
extern int32_t* pMaxFaultBatteryVoltage;

void AnalyzeFaults ( void ) {
/* FAULT_NO_BATTERY             */ 
	if ( Values[BATTERY_VOLTAGE] < References[NO_BATTERY_VOLTAGE] ) { // Если измерение напряжения АКБ дало результат меньше напряжения отсутствия АКБ, то...
    if ( GetSubsystemState ( PRECHARGE_CONTROLLER ) == PRECHARGE_IS_ON ) { // Если реле предзаряда включено, то...
			SetFault ( FAULT_NO_BATTERY ); // Устанавливаем ошибку "АКБ не подкл"
		}
  }
  else { // В противном случае, то есть если измерение напряжения АКБ дало результат больше напряжения отсутствия АКБ, то
    if ( !GetFaultState ( FAULT_MODE_CANCEL ) ) { ClearFault ( FAULT_NO_BATTERY ); } // Если нет сброшенного режима, то сбрасываем аварию "Батарея не подкл".
  }
/* FAULT_BATTERY_SHORT_CIRQUIT  */ // Формируется процедурой управления зарядом DispatchChargeControl
/* PAYLOAD_LOCKING_FAULT        */ // Формируется процедурой управления нагрузкой DispatchPayload
/* FAULT_PAYLOAD_IS_LOCKED      */ // Формируется процедурой управления нагрузкой DispatchPayload
/* PAYLOAD_FREE_FAULT           */ // Формируется процедурой управления нагрузкой DispatchPayload
/* FAULT_PAYLOAD_SHORT_CIRQUIT  */ //
/* FAULT_LOW_NETWORK_VOLTAGE    */
	if ( Values[NETWORK_VOLTAGE] > References[MIN_NETWORK_VOLTAGE] ) { // Если напряжение сети больше минимального допустимого уровня, то...
    GetTimeStamp (&LastLowNetworkVoltageTimeStamp); // запоминаем это время как время нормального напряжения АКБ
    if ( !GetFaultState(FAULT_MODE_CANCEL) ) { ClearFault (FAULT_LOW_NETWORK_VOLTAGE); } // Если не было сброса режима по аварии, то сбрасываем эту неисправность
  }
  else { if ( IsIntervalOverFrom ( 96000000, LastLowNetworkVoltageTimeStamp ) ) { SetFault (FAULT_LOW_NETWORK_VOLTAGE); } } // Если ниже, то по истечении 1 секунды выставляем аварию
		
/* FAULT_HIGH_NETWORK_VOLTAGE   */
	if ( Values[NETWORK_VOLTAGE] < References[MAX_NETWORK_VOLTAGE] ) { 
    GetTimeStamp (&LastHighNetworkVoltageTimeStamp); 
    if ( !GetFaultState(FAULT_MODE_CANCEL) ) { ClearFault (FAULT_HIGH_NETWORK_VOLTAGE); }
  }
  else { if ( IsIntervalOverFrom ( 96000000, LastHighNetworkVoltageTimeStamp ) ) { SetFault (FAULT_HIGH_NETWORK_VOLTAGE); } }
/* FAULT_LOW_CONVERTER_VOLTAGE  */
	if ( Values[SOURCE_DC_CONVERTER_VOLTAGE] > References[MIN_SOURCE_DC_CONVERTER_VOLTAGE] ) {
    GetTimeStamp (&LastLowFilterVoltageTimeStamp);
    if ( !GetFaultState(FAULT_MODE_CANCEL) ) { ClearFault (FAULT_LOW_CONVERTER_VOLTAGE); }
  }
  else { if ( IsIntervalOverFrom ( 96000000, LastLowFilterVoltageTimeStamp ) ) { SetFault (FAULT_LOW_CONVERTER_VOLTAGE); } }
/* FAULT_HIGH_CONVERTER_VOLTAGE */ 
	if ( Values[SOURCE_DC_CONVERTER_VOLTAGE] < References[MAX_SOURCE_DC_CONVERTER_VOLTAGE] ) {
    GetTimeStamp (&LastHighFilterVoltageTimeStamp);
    if ( !GetFaultState(FAULT_MODE_CANCEL) ) { ClearFault (FAULT_HIGH_CONVERTER_VOLTAGE); }
  }
  else { if ( IsIntervalOverFrom ( 96000000, LastHighFilterVoltageTimeStamp ) ) { SetFault (FAULT_HIGH_CONVERTER_VOLTAGE); } }
/* FAULT_LOW_BATTERY_VOLTAGE    */              
  if ( Values[BATTERY_VOLTAGE] < *pMinFaultBatteryVoltage ) {                     
    if ( GetSubsystemState (CONV_CONTACTOR_CONTROLLER) == CONV_CONTACTOR_IS_ON ) {
      SetFault ( FAULT_LOW_BATTERY_VOLTAGE );
    }
	}
	else if ( !GetFaultState ( FAULT_MODE_CANCEL ) ) { ClearFault ( FAULT_LOW_BATTERY_VOLTAGE ); }
/* FAULT_HIGH_BATTERY_VOLTAGE   */
  if ( Values[BATTERY_VOLTAGE] > *pMaxFaultBatteryVoltage ) { SetFault(FAULT_HIGH_BATTERY_VOLTAGE); }
  else if ( !GetFaultState(FAULT_MODE_CANCEL) ) { ClearFault (FAULT_HIGH_BATTERY_VOLTAGE); }
/* FAULT_NO_CHARGE_CURRENT      */ // Нет тока заряда. Формируется процедурой управления зарядом DispatchChargeController или процедурой регулирования тока заряда RegulateChargeCurrent
/* FAULT_NO_DISCHARGE_CURRENT   */ // Нет тока разряда. Формируется процедурой управления зарядом DispatchChargeController или процедурой регулирования тока разряда RegulateDischargeCurrent
/* FAULT_PRECHARGE_FAULT        */ // Ошибка предзаряда. Формируется процедурой управления предзарядом DispatchPrechargeController
/* FAULT_CHARGE_CANCEL          */ // Сброс заряда.
/* FAULT_DISCHARGE_CANCEL       */ // Сброс разряда.
/* CHARGE_REGULATION_FAULT      */ // 
/* DISCHARGE_REGULATION_FAULT   */ // 
/* FAULT_RADIATOR_OVERHEAT      */ // 
  if ( Values[RADIATOR_TEMPERATURE] > References[MAX_RADIATOR_TEMP] ) { SetFault ( FAULT_RADIATOR_OVERHEAT ); }
  else if ( Values[RADIATOR_TEMPERATURE] < References[NORM_RADIATOR_TEMP] ) { ClearFault ( FAULT_RADIATOR_OVERHEAT ); }
/* FAULT_TEMP_SENSOR_DISCONNECT */ 
  if ( Values[RADIATOR_TEMPERATURE] < -15000 ) { SetFault ( FAULT_TEMP_SENSOR_DISCONNECT ); } else { ClearFault ( FAULT_TEMP_SENSOR_DISCONNECT ); }

  if ( FaultsCount ) { if ( GetRelayState ( FAULT_LED ) != PULSE ) { SetRelayState ( FAULT_LED, PULSE ); } } else { SetRelayState ( FAULT_LED, OFF ); }

}

void DispatchFaults ( void ) {
}

uint32_t GetFaultState ( FaultIndex_Typedef FaultIndex ) {
  if ( FaultIndex < NUM_OF_FAULTS ) { return FaultsStates[FaultIndex]; }
  else { return 0; }
}

extern uint32_t MenuIndex;

void SetFault ( FaultIndex_Typedef FaultIndex ) {
  if ( !( FaultsStates[FaultIndex] ) && ( FaultIndex < NUM_OF_FAULTS ) ) {
    FaultsStates[FaultIndex] = FAULT_ON;
    FaultsCount++;
    FaultsActions |= FaultActions[FaultIndex];
    if ( MenuIndex == FAULTS_MENU ) {
      LoadFaultsMenu ();
    }
  }
}

void ClearFault ( FaultIndex_Typedef FaultIndex ) {
  if ( ( FaultsStates[FaultIndex] ) && ( FaultIndex < NUM_OF_FAULTS ) ) {
    FaultsStates[FaultIndex] = FAULT_OFF;
    FaultsCount--;
    CalculateFaultsActions ();
    if ( MenuIndex == FAULTS_MENU ) {
      LoadFaultsMenu ();
    }
  }
}

void ClearFaults ( void ) {
  uint32_t FaultIndex;
  for ( FaultIndex = 0; FaultIndex < NUM_OF_FAULTS; FaultIndex++ ) {
    if ( FaultsStates[FaultIndex] ) {
      FaultsStates[FaultIndex] = FAULT_OFF;
    }
  }
  FaultsActions = 0;
  FaultsCount = 0;
  if ( MenuIndex == FAULTS_MENU ) {
    LoadFaultsMenu ();
  }
}

void FaultsMoveRight ( void ) {
}

void FaultsMoveLeft ( void ) {
}

void FaultsMoveUp ( void ) {
}

void FaultsMoveDown ( void ) {
}
