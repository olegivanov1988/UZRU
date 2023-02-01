#include "main.h"
#include "Board.h"
#include "VoltageControl.h"
#include "TimeControl.h"
#include "Subsystems.h"
#include "ChargeController.h"
#include "Measurements.h"
#include "Relays.h"
#include "Menu.h"
#include "Faults.h"
#include "Calibrations.h"
#include "PayloadController.h"
#include "PrechargeController.h"
#include "References.h"
#include "ConvContactorController.h"

extern uint32_t MenuIndex;

uint64_t TotalTime;      // Общее время режима
int64_t tmpGlobalVolume; // Буферная переменная для вычисления приращения заряда

int64_t tmpTotalVolume;  // Буферная переменная для вычисления переданного в АКБ заряда
int32_t TotalVolume;     // Переменная для хранения переданного в АКБ заряда

int32_t* pChargeVoltage; // Указатель на напряжение заряда.
int32_t* pChargeCurrent; // Указатель на ток заряда.
int32_t* pDischargeCurrent; // Указатель на ток разряда.

int32_t LastChargeVoltage; // Предыдущее значение напряжения заряда
int32_t LastChargeCurrent; // Предыдущее значение тока заряда
int32_t LastDischargeCurrent; // Предыдущее значение тока разряда

int32_t ChargeVoltageSignal; // Демпфированный сигнал управления напряжением заряда
int32_t ChargeCurrentSignal; // Демпфированный сигнал управления током заряда
int32_t DischargeCurrentSignal; // Демпфированный сигнал управления током разряда

int32_t TargetChargeCurrentSignal; // Целевой сигнал управления током заряда
int32_t TargetChargeVoltageSignal; // Целевой сигнал управления напряжением заряда
int32_t TargetDischargeCurrentSignal; // Целевой сигнал управления током разряда

int32_t* pMaxFaultBatteryVoltage;
int32_t* pMinFaultBatteryVoltage;

// Эта функция позволяет напрямую установить желаемый сигнал управления током заряда
// Этот сигнал постепенно будет выведен на нужный вывод микроконтроллера
void SetTargetChargeCurrentSignal ( int32_t NewTargetChargeCurrentSignal ) {
  TargetChargeCurrentSignal = NewTargetChargeCurrentSignal;
}

// Функция позволяет напрямую установить желаемый сигнал управления напряжением заряда
// Этот сигнал постепенно будет выведен на нужный вывод микроконтроллера
void SetTargetChargeVoltageSignal ( int32_t NewTargetChargeVoltageSignal ) {
  TargetChargeVoltageSignal = NewTargetChargeVoltageSignal;
}

// Демпфирующая переменная регулирования заряда
int32_t TimeToRegulateChargeParameters = 0;

// Эта функция постепенно подводит сигналы обратной связи датчиков тока и напряжения АКБ на выводах микроконтроллера к заданным пользователем уровням
void RegulateDirectChargeParameters ( void ) {
  
  //int32_t tmpFault = 100 * ( NormedPreValues[CHARGE_CURRENT] - TargetChargeCurrentSignal );
  
  if ( TimeToRegulateChargeParameters % 10 == 0 ) {
		
    if ( ( PreValuesSum[CHARGE_CURRENT] > (TargetChargeCurrentSignal * MAX_NUM_OF_MEASUREMENTS) ) || ( PreValuesSum[BATTERY_VOLTAGE] > (TargetChargeVoltageSignal * MAX_NUM_OF_MEASUREMENTS) ) ) {
      if ( ChargeCurrentSignal > 0 ) { ChargeCurrentSignal--; }
		}
    else if ( ( PreValuesSum[CHARGE_CURRENT] < (TargetChargeCurrentSignal * MAX_NUM_OF_MEASUREMENTS) ) && ( PreValuesSum[BATTERY_VOLTAGE] < (TargetChargeVoltageSignal * MAX_NUM_OF_MEASUREMENTS) ) ) {
      if ( ChargeCurrentSignal < 4095 ) { ChargeCurrentSignal++; }
    }
  
  }
  
  TimeToRegulateChargeParameters++;
  Board_SetChargeCurrent ( ChargeCurrentSignal );
  // Board_SetChargeVoltage ( ChargeVoltageSignal );

}

// Функция позволяет напрямую установить желаемый сигнал управления током разряда
// этот сигнал постепенно будет выведен на нужный вывод микроконтроллера
void SetTargetDischargeCurrentSignal ( int32_t NewTargetDischargeCurrentSignal ) {
  TargetDischargeCurrentSignal = NewTargetDischargeCurrentSignal;
}

// Эта функция постепенно подводит сигналы обратной связи датчика тока разряда на выводе микроконтроллера к заданному пользователем уровню
void RegulateDirectDischargeParameters ( void ) {
  
  if ( TimeToRegulateChargeParameters % 5 == 0 ) {
    //int32_t tmp = 0;
    if ( PreValuesSum[DISCHARGE_CURRENT] > (TargetDischargeCurrentSignal * MAX_NUM_OF_MEASUREMENTS) ) {
      if ( DischargeCurrentSignal > MIN_DISCHARGE_SIGNAL ) { DischargeCurrentSignal--; }
    }
    else if ( PreValuesSum[DISCHARGE_CURRENT] < (TargetDischargeCurrentSignal * MAX_NUM_OF_MEASUREMENTS) ) {
      if ( DischargeCurrentSignal < MAX_DISCHARGE_SIGNAL ) {
        DischargeCurrentSignal++;
      }
    }
  }
  
  Board_SetDischargeCurrent ( DischargeCurrentSignal );
  TimeToRegulateChargeParameters++;
  
}

int32_t RegulationDirection = 0;
int32_t RegulationOk = 0;
int32_t StatisticsNotInitialized = 1;
uint64_t LastRegulationOkTimeStamp;

// Эта функция очищает все сигналы
void ClearSignals ( void ) {
  RegulationDirection = 0;
  RegulationOk = 0;
  StatisticsNotInitialized = 1;
  TargetChargeVoltageSignal = 0;
  TargetChargeCurrentSignal = 0;
  TargetDischargeCurrentSignal = 0;
  ChargeVoltageSignal = 0;
  ChargeCurrentSignal = 0;
  DischargeCurrentSignal = 0;
	Board_SetChargeCurrent ( ChargeCurrentSignal );
	Board_SetDischargeCurrent ( DischargeCurrentSignal );
}

void RegulateChargeParameters ( void ) {
  
  RegulationOk = 0;
  
  if ( ( TimeToRegulateChargeParameters % 5 ) == 0 ) {
    
    if ( Values[CHARGE_CURRENT] > *pChargeCurrent ) {
      if ( RegulationDirection & 2 ) {
        RegulationOk = 1;
        RegulationDirection &= ~2;
      }
    }
    else if ( Values[CHARGE_CURRENT] < *pChargeCurrent ) {
      if ( !( RegulationDirection & 2 ) ) {
        RegulationOk = 1;
        RegulationDirection |= 2;
      }
    }
    else {
      RegulationOk = 1;
    }
    
    if ( Values[BATTERY_VOLTAGE] > *pChargeVoltage ) {
      if ( RegulationDirection & 1 ) {
        RegulationOk = 1;
        RegulationDirection &= ~1;
      }
    }
    else if ( Values[BATTERY_VOLTAGE] < *pChargeVoltage ) {
      if ( !( RegulationDirection & 1 ) ) {
        RegulationOk = 1;
        RegulationDirection |= 1;
      }
    }
    else {
      RegulationOk = 1;
    }
    
    if ( ( Values[CHARGE_CURRENT] < *pChargeCurrent ) && ( Values[BATTERY_VOLTAGE] < *pChargeVoltage ) ) {
      if ( ChargeCurrentSignal < 4095) { ChargeCurrentSignal++; }
    }
    else if ( ( Values[CHARGE_CURRENT] > *pChargeCurrent ) || ( Values[BATTERY_VOLTAGE] > *pChargeVoltage ) ) {
      if ( ChargeCurrentSignal > 0 ) { ChargeCurrentSignal--; }
    }
    else {
      RegulationOk = 1;
    }

  }
  
  TimeToRegulateChargeParameters++;
  
  Board_SetChargeCurrent ( ChargeCurrentSignal );
  Board_SetChargeVoltage ( 4095 );
  
}

int32_t DeltaDischargeCurrentSign = 1;

void RegulateDischargeParameters ( void ) {
  
  RegulationOk = 0;
  
  if ( TimeToRegulateChargeParameters % 2 == 0 ) {
  
    if ( Values[DISCHARGE_CURRENT] > *pDischargeCurrent ) {
      if ( RegulationDirection & 4 ) {
        RegulationDirection &= ~4;
        RegulationOk = 1;
      }
      if ( DischargeCurrentSignal > MIN_DISCHARGE_SIGNAL ) {
        DischargeCurrentSignal--;
      }
    }
    else if ( Values[DISCHARGE_CURRENT] < *pDischargeCurrent ) {
      if ( !( RegulationDirection & 4 ) ) {
        RegulationDirection |= 4;
        RegulationOk = 1;
      }
      if ( DischargeCurrentSignal < MAX_DISCHARGE_SIGNAL ) {
        DischargeCurrentSignal++;
      }
    }
    else {
      RegulationOk = 1;
    }
    
    if ( RegulationOk ) { GetTimeStamp ( &LastRegulationOkTimeStamp ); }
    else {
     // if ( IsIntervalOverFrom ( (uint64_t)480000000, LastRegulationOkTimeStamp ) ) { SetFault ( DISCHARGE_REGULATION_FAULT ); }
    }
    
  }
  
  Board_SetDischargeCurrent ( DischargeCurrentSignal );
  TimeToRegulateChargeParameters++;
  
}

uint64_t* pGlobalTime;
uint32_t* pGlobalVolume;
int32_t* pGlobalUmax;
int32_t* pGlobalUmin;
int32_t* pGlobalImax;
int32_t* pGlobalImin;

int32_t LocalUmax;
int32_t LocalUmin;
int32_t LocalImax;
int32_t LocalImin;

uint32_t MinuteIndex;

uint64_t StartLocalStatisticsTimeStamp;
uint64_t StartMinuteStatisticsTimeStamp;

void ChargeControllerSetSettings ( int32_t* pNewChargeVoltage, int32_t* pNewChargeCurrent, int32_t* pNewDischargeCurrent ) {
  pChargeVoltage = pNewChargeVoltage;
  pChargeCurrent = pNewChargeCurrent;
  pDischargeCurrent = pNewDischargeCurrent;
}

void ChargeControllerSetTelemetry ( uint32_t* pNewUmax, uint32_t* pNewUmin, uint32_t* pNewImax, uint32_t* pNewImin, uint64_t* pNewTime, uint32_t* pNewVolume ) {
  pGlobalTime = pNewTime;
  pGlobalVolume = pNewVolume;
  tmpGlobalVolume = (uint64_t)((uint64_t)96000000*(*pGlobalVolume)*(uint64_t)3600);
  pGlobalUmax = pNewUmax;
  pGlobalUmin = pNewUmin;
  pGlobalImax = pNewImax;
  pGlobalImin = pNewImin;
}

void StartNewRegulation (void ) {
  RegulationDirection = 0;
  StatisticsNotInitialized = 1;
  TimeToRegulateChargeParameters = 1;
  RegulationOk = 0;
  LocalUmin = 10000;
  LocalImin = 10000;
  LocalUmax = 0;
  LocalImax = 0;
  if ( Values[BATTERY_VOLTAGE] < *pChargeVoltage ) {
    RegulationDirection |= 1;
  }
  if ( Values[CHARGE_CURRENT] < *pChargeCurrent ) {
    RegulationDirection |= 2;
  }
  if ( Values[DISCHARGE_CURRENT] < *pDischargeCurrent ) {
    RegulationDirection |= 4;
  }
  LastChargeCurrent = 0;
  LastDischargeCurrent = 0;
}

uint64_t ChargeControllerTimeStamp;

uint32_t LastChargeControllerState;
uint64_t LastTimeStamp;
uint64_t NowTimeStamp;
uint64_t StartModeTimeStamp;

typedef struct {
  int32_t MinVoltage;
  int32_t MaxVoltage;
} VoltageIntervalControlStruct;

VoltageIntervalControlStruct VoltageMinMaxValues[60];

int32_t NumOfVoltageControls = 0;
int32_t VoltageControlIndex = 0;
int32_t VoltageIntervalPerHour = 101;

void StartVoltageIntervalPerHourControl ( void ) {
  NumOfVoltageControls = 0;
  VoltageControlIndex = 0;
  VoltageIntervalPerHour = 101;
}

void SaveVoltages ( int32_t MinVoltagePerMinute, int32_t MaxVoltagePerMinute ) {
  int32_t tmpIndex, tmpUmax, tmpUmin;
  tmpUmax = MaxVoltagePerMinute;
  tmpUmin = MinVoltagePerMinute;
  VoltageMinMaxValues[VoltageControlIndex].MinVoltage = MinVoltagePerMinute;
  VoltageMinMaxValues[VoltageControlIndex].MaxVoltage = MaxVoltagePerMinute;
  NumOfVoltageControls++;
  VoltageControlIndex = ( VoltageControlIndex + 1 ) % References[SIMPLE_CONST_I_CHARGE_2ND_STAGE_MIN_TIME];
  if ( NumOfVoltageControls < References[SIMPLE_CONST_I_CHARGE_2ND_STAGE_MIN_TIME] ) {
    VoltageIntervalPerHour = 101;
  }
  else {
    for ( tmpIndex = 0; tmpIndex < References[SIMPLE_CONST_I_CHARGE_2ND_STAGE_MIN_TIME]; tmpIndex++ ) {
      if ( tmpUmax < VoltageMinMaxValues[tmpIndex].MaxVoltage ) { tmpUmax = VoltageMinMaxValues[tmpIndex].MaxVoltage; }
      if ( tmpUmin > VoltageMinMaxValues[tmpIndex].MinVoltage ) { tmpUmin = VoltageMinMaxValues[tmpIndex].MinVoltage; }
    }
    VoltageIntervalPerHour = tmpUmax - tmpUmin;
  }
}

int32_t GetVoltageIntervalPerHour ( void ) {
  return VoltageIntervalPerHour;
}

int32_t MinuteUmax, MinuteUmin;

uint64_t LastSecondTimeStamp;
uint64_t LastMinuteTimeStamp;

void UpdateChargeStatistics ( void ) {
  uint64_t tmpDeltaTime;
  int64_t DoubleCurrent;
  if ( ( StatisticsNotInitialized ) && ( RegulationOk ) ) {
    StatisticsNotInitialized = 0;
    GetTimeStamp ( &LastSecondTimeStamp );
    GetTimeStamp ( &LastMinuteTimeStamp );
    LastChargeCurrent = Values[CHARGE_CURRENT];
    LocalUmax = LocalUmin = *pGlobalUmax = *pGlobalUmin = MinuteUmax = MinuteUmin = Values[BATTERY_VOLTAGE];
    LocalImax = LocalImin = *pGlobalImax = *pGlobalImin = Values[CHARGE_CURRENT];
  }
  else if ( !StatisticsNotInitialized ) {
    if ( *pGlobalUmax < Values[BATTERY_VOLTAGE] ) { *pGlobalUmax = Values[BATTERY_VOLTAGE]; }
    if ( *pGlobalImax < Values[CHARGE_CURRENT] ) { *pGlobalImax = Values[CHARGE_CURRENT]; }
    if ( *pGlobalUmin > Values[BATTERY_VOLTAGE] ) { *pGlobalUmin = Values[BATTERY_VOLTAGE]; }
    if ( *pGlobalImin > Values[CHARGE_CURRENT] ) { *pGlobalImin = Values[CHARGE_CURRENT]; }
    
    if ( IsIntervalOverFrom ( 96000000, LastSecondTimeStamp ) ) {
      LocalUmax = LocalUmin = Values[BATTERY_VOLTAGE];
      LocalImax = LocalImin = Values[CHARGE_CURRENT];
      GetTimeStamp ( &LastSecondTimeStamp );
    }
    else {
      if ( LocalUmax < Values[BATTERY_VOLTAGE] ) { LocalUmax = Values[BATTERY_VOLTAGE]; }
      if ( LocalImax < Values[CHARGE_CURRENT] ) { LocalImax = Values[CHARGE_CURRENT]; }
      if ( LocalUmin > Values[BATTERY_VOLTAGE] ) { LocalUmin = Values[BATTERY_VOLTAGE]; }
      if ( LocalImin > Values[CHARGE_CURRENT] ) { LocalImin = Values[CHARGE_CURRENT]; }      
    }
    
    if ( IsIntervalOverFrom ( (uint64_t)((uint64_t)96000000*(uint64_t)60), LastMinuteTimeStamp ) ) {
      SaveVoltages ( MinuteUmin, MinuteUmax );
      MinuteUmax = MinuteUmin = Values[BATTERY_VOLTAGE];
      GetTimeStamp ( &LastMinuteTimeStamp );
    }
    else {
      if ( MinuteUmax < Values[BATTERY_VOLTAGE] ) { MinuteUmax = Values[BATTERY_VOLTAGE]; }
      if ( MinuteUmin > Values[BATTERY_VOLTAGE] ) { MinuteUmin = Values[BATTERY_VOLTAGE]; }
    }
  }
  tmpDeltaTime = (uint64_t)((uint64_t)NowTimeStamp - (uint64_t)LastTimeStamp);
  (*pGlobalTime) = (uint64_t)(*pGlobalTime) + (uint64_t)tmpDeltaTime;
  TotalTime = (uint64_t)TotalTime + (uint64_t)tmpDeltaTime;
  DoubleCurrent = (int64_t)((int64_t)Values[CHARGE_CURRENT] + (int64_t)LastChargeCurrent);
  tmpGlobalVolume = (int64_t)tmpGlobalVolume + (int64_t)((int64_t)((int64_t)DoubleCurrent*(uint64_t)tmpDeltaTime))/(uint64_t)2;
  (*pGlobalVolume) = (int64_t)tmpGlobalVolume/(int64_t)96000000/(int64_t)3600;
  tmpTotalVolume = (int64_t)tmpTotalVolume + (int64_t)((int64_t)((int64_t)((int64_t)DoubleCurrent*(uint64_t)tmpDeltaTime))/(uint64_t)2);
  TotalVolume = (int64_t)tmpTotalVolume/(int64_t)96000000/(int64_t)3600;
  LastChargeCurrent = Values[CHARGE_CURRENT];
}

void UpdateDischargeStatistics ( void ) {
  if ( ( StatisticsNotInitialized ) && ( RegulationOk ) ) {
    StatisticsNotInitialized = 0;
    GetTimeStamp ( &LastSecondTimeStamp );
    LastDischargeCurrent = Values[DISCHARGE_CURRENT];
    LocalUmax = LocalUmin = *pGlobalUmax = *pGlobalUmin = Values[BATTERY_VOLTAGE];
    LocalImax = LocalImin = *pGlobalImax = *pGlobalImin = Values[DISCHARGE_CURRENT];
  }
  else if ( !StatisticsNotInitialized ) {
    if ( *pGlobalUmax < Values[BATTERY_VOLTAGE] ) { *pGlobalUmax = Values[BATTERY_VOLTAGE]; }
    if ( *pGlobalImax < Values[CHARGE_CURRENT] ) { *pGlobalImax = Values[DISCHARGE_CURRENT]; }
    if ( *pGlobalUmin > Values[BATTERY_VOLTAGE] ) { *pGlobalUmin = Values[BATTERY_VOLTAGE]; }
    if ( *pGlobalImin > Values[CHARGE_CURRENT] ) { *pGlobalImin = Values[DISCHARGE_CURRENT]; }
    
    if ( IsIntervalOverFrom ( 96000000, LastSecondTimeStamp ) ) {
      LocalUmax = LocalUmin = Values[BATTERY_VOLTAGE];
      LocalImax = LocalImin = Values[DISCHARGE_CURRENT];
      GetTimeStamp ( &LastSecondTimeStamp );
    }
    else {
      if ( LocalUmax < Values[BATTERY_VOLTAGE] ) { LocalUmax = Values[BATTERY_VOLTAGE]; }
      if ( LocalImax < Values[DISCHARGE_CURRENT] ) { LocalImax = Values[DISCHARGE_CURRENT]; }
      if ( LocalUmin > Values[BATTERY_VOLTAGE] ) { LocalUmin = Values[BATTERY_VOLTAGE]; }
      if ( LocalImin > Values[DISCHARGE_CURRENT] ) { LocalImin = Values[DISCHARGE_CURRENT]; }      
    }
    
  }
  
  (*pGlobalTime) = (uint64_t)(*pGlobalTime) + (uint64_t)NowTimeStamp - (uint64_t)LastTimeStamp;
  TotalTime = (uint64_t)TotalTime + (uint64_t)NowTimeStamp - (uint64_t)LastTimeStamp;
  tmpGlobalVolume = (uint64_t)tmpGlobalVolume + (uint64_t)((uint64_t)Values[DISCHARGE_CURRENT]+(uint64_t)LastDischargeCurrent)*(((uint64_t)(NowTimeStamp - (uint64_t)LastTimeStamp)))/(uint64_t)2;
  (*pGlobalVolume) = (uint64_t)tmpGlobalVolume/96000000/3600;
  tmpTotalVolume = (int64_t)tmpTotalVolume + (uint64_t)((uint64_t)Values[DISCHARGE_CURRENT]+(uint64_t)LastDischargeCurrent)*(((uint64_t)(NowTimeStamp - (uint64_t)LastTimeStamp)))/(uint64_t)2;
  TotalVolume = (int64_t)tmpTotalVolume/(int64_t)96000000/(int64_t)3600;
  LastDischargeCurrent = Values[DISCHARGE_CURRENT];
  
}

void DispatchChargeController () {
  int32_t tmpChargeControllerState = GetSubsystemState(CHARGE_CONTROLLER);
  LastTimeStamp = (uint64_t)NowTimeStamp;
  GetTimeStamp ( &NowTimeStamp );
  switch ( tmpChargeControllerState ) {
  case CHARGE_CONTROLLER_OFF:
    break;
  case CHARGE_CONTROLLER_START_CHARGE:
		Board_SetDischargeCurrent ( 0 );
    if ( ( GetSubsystemState ( PAYLOAD_CONTROLLER ) != PAYLOAD_IS_FREE ) && ( GetSubsystemState ( PAYLOAD_CONTROLLER ) != PAYLOAD_IS_LOCKED_BY_SOMEBODY ) ) {
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_GLOBAL_DISCHARGE_FOR_CHARGE );
    }
    else if ( GetRelayState ( START_DISCHARGE ) == ON ) {
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_LOCAL_DISCHARGE_FOR_CHARGE );
    }
    else if ( GetRelayState ( START_CHARGE ) == ON ) {
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_CHARGE );
    }
    else {
      if ( GetSubsystemState ( PRECHARGE_CONTROLLER ) == PRECHARGE_AUTO ) { SetSubsystemState ( PRECHARGE_CONTROLLER, PRECHARGE_SET_ON ); }
      SetSubsystemState ( CONV_CONTACTOR_CONTROLLER, CONV_CONTACTOR_SET_ON );
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_WAIT_START_CHARGE );
    }
    break;
  case CHARGE_CONTROLLER_WAIT_START_CHARGE:
    if ( GetSubsystemState ( CONV_CONTACTOR_CONTROLLER ) == CONV_CONTACTOR_IS_ON ) {
      ClearSignals ();
      StartNewRegulation ();
      SetRelayState ( START_CHARGE, ON );
      GetTimeStamp ( &StartModeTimeStamp );
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_CHARGE );
    }
    break;
  case CHARGE_CONTROLLER_CHARGE:
    RegulateChargeParameters ();
    if ( IsIntervalOverFrom ( (uint64_t)48000000, StartModeTimeStamp ) ) { UpdateChargeStatistics (); }
    break;
  case CHARGE_CONTROLLER_STOP_CHARGE:
    SetRelayState ( START_CHARGE, OFF );
	  ClearSignals ();
    SetSubsystemState ( CONV_CONTACTOR_CONTROLLER, CONV_CONTACTOR_SET_OFF );
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_WAIT_STOP_CHARGE );
    break;
  case CHARGE_CONTROLLER_WAIT_STOP_CHARGE:
    if ( GetSubsystemState ( CONV_CONTACTOR_CONTROLLER ) == CONV_CONTACTOR_IS_OFF ) {
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_OFF );
    }
    break;
    
  case CHARGE_CONTROLLER_START_DIRECT_CHARGE:
		Board_SetDischargeCurrent ( 0 );
    if ( ( GetSubsystemState ( PAYLOAD_CONTROLLER ) != PAYLOAD_IS_FREE ) && ( GetSubsystemState ( PAYLOAD_CONTROLLER ) != PAYLOAD_IS_LOCKED_BY_SOMEBODY ) ) {
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_OFF );
    }
    else if ( GetRelayState ( START_DISCHARGE ) == ON ) {
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_OFF );
    }
    else if ( GetRelayState ( START_CHARGE ) == ON ) {
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_DIRECT_CHARGE );
    }
    else {
      if ( GetSubsystemState ( PRECHARGE_CONTROLLER ) == PRECHARGE_AUTO ) { SetSubsystemState ( PRECHARGE_CONTROLLER, PRECHARGE_SET_ON ); }
      SetSubsystemState ( CONV_CONTACTOR_CONTROLLER, CONV_CONTACTOR_SET_ON );
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_WAIT_START_DIRECT_CHARGE );
    }
    break;
    
  case CHARGE_CONTROLLER_WAIT_START_DIRECT_CHARGE:
    if ( GetSubsystemState ( CONV_CONTACTOR_CONTROLLER ) == CONV_CONTACTOR_IS_ON ) {
      SetRelayState ( START_CHARGE, ON );
      GetTimeStamp ( &StartModeTimeStamp );
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_DIRECT_CHARGE );
    }
    break;
  case CHARGE_CONTROLLER_DIRECT_CHARGE:
    RegulateDirectChargeParameters ();
    break;
  case CHARGE_CONTROLLER_STOP_DIRECT_CHARGE:
    SetRelayState ( START_CHARGE, OFF );
    ClearSignals ();
    SetSubsystemState ( CONV_CONTACTOR_CONTROLLER, CONV_CONTACTOR_SET_OFF );
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_WAIT_STOP_DIRECT_CHARGE );
    break;
  case CHARGE_CONTROLLER_WAIT_STOP_DIRECT_CHARGE:
    if ( GetSubsystemState ( CONV_CONTACTOR_CONTROLLER ) == CONV_CONTACTOR_IS_OFF ) {
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_OFF );
    }
    break;
    
  case CHARGE_CONTROLLER_STOP_CHARGE_FOR_LOCAL_DISCHARGE:
    SetRelayState ( START_CHARGE, OFF );
    ClearSignals ();
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_WAIT_STOP_CHARGE_FOR_LOCAL_DISCHARGE );
    GetTimeStamp ( &ChargeControllerTimeStamp );
    break;
  case CHARGE_CONTROLLER_WAIT_STOP_CHARGE_FOR_LOCAL_DISCHARGE:
    if ( IsIntervalOverFrom ( (uint64_t)36000000, ChargeControllerTimeStamp ) ) {
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_LOCAL_DISCHARGE );
    }
    break;
  case CHARGE_CONTROLLER_START_LOCAL_DISCHARGE:
		Board_SetChargeCurrent ( 0 );
    if ( GetRelayState ( START_CHARGE ) == ON ) {
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE_FOR_LOCAL_DISCHARGE );
    }
    else {
      if ( GetSubsystemState ( PRECHARGE_CONTROLLER ) == PRECHARGE_AUTO ) { SetSubsystemState ( PRECHARGE_CONTROLLER, PRECHARGE_SET_ON ); }
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_WAIT_START_LOCAL_DISCHARGE );
    }
    break;
  case CHARGE_CONTROLLER_WAIT_START_LOCAL_DISCHARGE:
    if ( GetSubsystemState ( PRECHARGE_CONTROLLER ) == PRECHARGE_IS_ON ) {
      SetRelayState ( START_DISCHARGE, ON );
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_LOCAL_DISCHARGE );
      GetTimeStamp ( &StartModeTimeStamp );
    }
    break;
  case CHARGE_CONTROLLER_LOCAL_DISCHARGE:
    UpdateDischargeStatistics ();
    RegulateDischargeParameters ();
    break;
  case CHARGE_CONTROLLER_STOP_LOCAL_DISCHARGE:
    Board_SetDischargeCurrent ( MIN_DISCHARGE_SIGNAL );
    SetRelayState ( START_DISCHARGE, OFF );
	  ClearSignals ();
    SetSubsystemState ( CONV_CONTACTOR_CONTROLLER, CONV_CONTACTOR_SET_OFF );
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_WAIT_STOP_LOCAL_DISCHARGE );    
    break;
  case CHARGE_CONTROLLER_WAIT_STOP_LOCAL_DISCHARGE:
    if ( GetSubsystemState ( CONV_CONTACTOR_CONTROLLER ) == CONV_CONTACTOR_IS_OFF ) {
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_OFF );
    }
    break;
  case CHARGE_CONTROLLER_STOP_LOCAL_DISCHARGE_FOR_CHARGE:
    SetRelayState ( START_DISCHARGE, OFF );
	  ClearSignals ();
    Board_SetDischargeCurrent ( MIN_DISCHARGE_SIGNAL );
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_WAIT_STOP_LOCAL_DISCHARGE_FOR_CHARGE );
    break;
  case CHARGE_CONTROLLER_WAIT_STOP_LOCAL_DISCHARGE_FOR_CHARGE:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    break;
  case CHARGE_CONTROLLER_START_GLOBAL_DISCHARGE:
		Board_SetChargeCurrent ( 0 );
    if ( GetSubsystemState ( PRECHARGE_CONTROLLER ) == PRECHARGE_AUTO ) { SetSubsystemState ( PRECHARGE_CONTROLLER, PRECHARGE_SET_ON ); }
    SetSubsystemState ( PAYLOAD_CONTROLLER, TRY_LOCK_PAYLOAD );
    SetSubsystemState ( CONV_CONTACTOR_CONTROLLER, CONV_CONTACTOR_SET_ON );
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_WAIT_START_GLOBAL_DISCHARGE );    
    break;
  case CHARGE_CONTROLLER_WAIT_START_GLOBAL_DISCHARGE:
    if ( ( GetSubsystemState ( PAYLOAD_CONTROLLER ) == PAYLOAD_IS_LOCKED_BY_ME ) && ( GetSubsystemState ( CONV_CONTACTOR_CONTROLLER ) == CONV_CONTACTOR_IS_ON ) ) {
      SetRelayState ( START_DISCHARGE, ON );
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_GLOBAL_DISCHARGE );
    }
    break;
  case CHARGE_CONTROLLER_GLOBAL_DISCHARGE:
    UpdateDischargeStatistics ();
    RegulateDischargeParameters ();
    break;
  case CHARGE_CONTROLLER_PAUSE_GLOBAL_DISCHARGE:
    SetRelayState ( START_DISCHARGE, OFF );
    SetRelayState ( LOAD_CONTACTOR, OFF );
	  ClearSignals ();
	  Board_SetDischargeCurrent ( 0 );
    GetTimeStamp ( &ChargeControllerTimeStamp );
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_WAIT_PAUSE_GLOBAL_DISCHARGE );
    break;
  case CHARGE_CONTROLLER_WAIT_PAUSE_GLOBAL_DISCHARGE:
    if ( IsIntervalOverFrom ( (uint64_t)36000000, ChargeControllerTimeStamp ) ) {
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_GLOBAL_DISCHARGE_PAUSED );
    }
    break;
  case CHARGE_CONTROLLER_GLOBAL_DISCHARGE_PAUSED:
    break;
  case CHARGE_CONTROLLER_STOP_GLOBAL_DISCHARGE:
    SetRelayState ( START_DISCHARGE, OFF );
	  ClearSignals ();
	  Board_SetDischargeCurrent ( 0 );
    SetSubsystemState ( CONV_CONTACTOR_CONTROLLER, CONV_CONTACTOR_SET_OFF );
    SetSubsystemState ( PAYLOAD_CONTROLLER, PAYLOAD_CONTROLLER_FREE_PAYLOAD );
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_WAIT_STOP_GLOBAL_DISCHARGE );    
    break;
  case CHARGE_CONTROLLER_WAIT_STOP_GLOBAL_DISCHARGE:
    if ( ( GetSubsystemState ( CONV_CONTACTOR_CONTROLLER ) == CONV_CONTACTOR_IS_OFF ) && ( GetSubsystemState ( PAYLOAD_CONTROLLER ) == PAYLOAD_IS_FREE ) ) {
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_OFF );
    }
    break;
  case CHARGE_CONTROLLER_STOP_GLOBAL_DISCHARGE_FOR_CHARGE:
    SetRelayState ( START_DISCHARGE, OFF );
	  ClearSignals ();
	  Board_SetDischargeCurrent ( 0 );
    break;
  case CHARGE_CONTROLLER_WAIT_STOP_GLOBAL_DISCHARGE_FOR_CHARGE:
    break;
  
  case CHARGE_CONTROLLER_START_DIRECT_GLOBAL_DISCHARGE:
		Board_SetChargeCurrent ( 0 );
    if ( GetSubsystemState ( PRECHARGE_CONTROLLER ) == PRECHARGE_AUTO ) { SetSubsystemState ( PRECHARGE_CONTROLLER, PRECHARGE_SET_ON ); }
    SetSubsystemState ( PAYLOAD_CONTROLLER, TRY_LOCK_PAYLOAD );
    SetSubsystemState ( CONV_CONTACTOR_CONTROLLER, CONV_CONTACTOR_SET_ON );
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_WAIT_START_DIRECT_GLOBAL_DISCHARGE );    
    break;
  case CHARGE_CONTROLLER_WAIT_START_DIRECT_GLOBAL_DISCHARGE:
    if ( ( GetSubsystemState ( PAYLOAD_CONTROLLER ) == PAYLOAD_IS_LOCKED_BY_ME ) && ( GetSubsystemState ( CONV_CONTACTOR_CONTROLLER ) == CONV_CONTACTOR_IS_ON ) ) {
      SetRelayState ( START_DISCHARGE, ON );
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_DIRECT_GLOBAL_DISCHARGE );
    }
    break;
  case CHARGE_CONTROLLER_DIRECT_GLOBAL_DISCHARGE:
    RegulateDirectDischargeParameters ();
    break;
  case CHARGE_CONTROLLER_STOP_DIRECT_GLOBAL_DISCHARGE:
    SetRelayState ( START_DISCHARGE, OFF );
	  ClearSignals ();
    SetSubsystemState ( CONV_CONTACTOR_CONTROLLER, CONV_CONTACTOR_SET_OFF );
    SetSubsystemState ( PAYLOAD_CONTROLLER, PAYLOAD_CONTROLLER_FREE_PAYLOAD );
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_WAIT_STOP_DIRECT_GLOBAL_DISCHARGE );
    break;
  case CHARGE_CONTROLLER_WAIT_STOP_DIRECT_GLOBAL_DISCHARGE:
    if ( ( GetSubsystemState ( CONV_CONTACTOR_CONTROLLER ) == CONV_CONTACTOR_IS_OFF ) && ( GetSubsystemState ( PAYLOAD_CONTROLLER ) == PAYLOAD_IS_FREE ) ) {
      SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_OFF );
    }
    break;
  }
  
  LastChargeControllerState = GetSubsystemState ( CHARGE_CONTROLLER );
}

void ChargeControllerStopDischarge () {
  SetRelayState ( START_DISCHARGE, OFF );
}

void AnalyzeChargeController () {
}

void InitChargeController () {
  ClearSignals ();
  SetRelayState ( START_CHARGE, OFF );
  SetRelayState ( START_DISCHARGE, OFF );
  SetRelayState ( WORK_LED, OFF );
  SetSubsystemState ( PAYLOAD_CONTROLLER, PAYLOAD_CONTROLLER_FREE_PAYLOAD );
  SetSubsystemState ( CONV_CONTACTOR_CONTROLLER, CONV_CONTACTOR_SET_OFF );
  SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_OFF );
}

void ChargeControllerSetPointers ( void ) {
}
