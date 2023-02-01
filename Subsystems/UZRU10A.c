#include "main.h"
#include "Board.h"
#include "VoltageControl.h"
#include "TimeControl.h"
#include "Subsystems.h"
#include "Uzru10a.h"
#include "Measurements.h"
#include "References.h"
#include "Relays.h"
#include "Menu.h"
#include "Faults.h"
#include "Calibrations.h"
#include "PayloadController.h"
#include "PrechargeController.h"
#include "ChargeController.h"
#include "ConvContactorController.h"

extern uint32_t MenuIndex;

uint32_t ModeStopOrPauseReason;

int32_t IDischarge;

extern uint64_t tmpTotalChargeVolume;
extern uint64_t tmpTotalDischargeVolume;
extern int64_t tmpTotalVolume;
extern int32_t TotalVolume;

const uint32_t TotalMaxVoltage = 4095;
const uint32_t ZeroValue = 0;

int32_t ConstUChargeChargeVoltage;
int32_t ConstUChargeMaxCurrent;
int32_t ConstUChargeMinCurrent;

int32_t ConstIChargeStage1MaxVoltage;
int32_t ConstIChargeStage1MaxCurrent;
int32_t ConstIChargeStage2MaxCurrent;
int32_t ConstIChargeStage2MaxVoltage;
uint64_t ConstIChargeStage2MaxTime;
int32_t ConstIChargeStage2MaxVoltageIntervalPerHour;

int32_t FastCombineChargeStage1MaxCurrent;
int32_t FastCombineChargeStage1MaxVoltage;
int32_t FastCombineChargeStage1MinCurrent;
int32_t FastCombineChargeStage2MaxVoltage;
int32_t FastCombineChargeStage2MaxCurrent;
int32_t FastCombineChargeStage3MaxVoltage;
int32_t FastCombineChargeStage3MaxCurrent;
int32_t FastCombineChargeStage3MaxVoltageIntervalPerHour;

int32_t SlowCombineChargeStage1MaxVoltage;
int32_t SlowCombineChargeStage1MaxCurrent;
int32_t SlowCombineChargeStage2MaxVoltage;
int32_t SlowCombineChargeStage2MaxCurrent;
int32_t SlowCombineChargeStage2MinCurrent;
int32_t SlowCombineChargeStage3MaxVoltage;
int32_t SlowCombineChargeStage3MaxCurrent;
uint64_t SlowCombineChargeStage3MaxTime;

int32_t ConstIWithLimitUChargeStage1MaxVoltage;
int32_t ConstIWithLimitUChargeStage1MaxCurrent;
int32_t ConstIWithLimitUChargeStage1MinCurrent;
int32_t ConstIWithLimitUChargeStage2MaxVoltage;
int32_t ConstIWithLimitUChargeStage2MaxCurrent;
int32_t ConstIWithLimitUChargeStage2MinCurrent;
int32_t ConstIWithLimitUChargeStage2MinVoltage;
int32_t ConstIWithLimitUChargeStage3MaxVoltage;
int32_t ConstIWithLimitUChargeStage3MaxCurrent;
uint64_t ConstIWithLimitUChargeStage3MaxTime;

int32_t AsimmChargeStage1MaxVoltage;
int32_t AsimmChargeStage1MaxCurrent;
int32_t AsimmChargeStage2MaxCurrent;
int32_t AsimmChargeStage2MinVoltage;
int32_t AsimmChargeStage3MaxVoltage;
int32_t AsimmChargeStage3MaxCurrent;
int32_t AsimmChargeStage3MinCurrent;
int32_t AsimmChargeStage4MaxVoltage;
int32_t AsimmChargeStage4MaxCurrent;
int32_t AsimmChargeStage4MaxVoltageIntervalPerHour;

int32_t ControlDischargeStage1MaxCurrent;
int32_t ControlDischargeStage1MinVoltage;

int32_t CTCStage1MaxVoltage;
int32_t CTCStage1MaxCurrent;
int32_t CTCStage2MaxCurrent;
int32_t CTCStage2MaxVoltage;
int32_t CTCStage2MaxVoltageIntervalPerHour;
uint64_t CTCStage4MaxTime;
int32_t CTCStage5MaxCurrent;
int32_t CTCStage5MinVoltage;
uint64_t CTCStage6MaxTime;
int32_t CTCStage7MaxVoltage;
int32_t CTCStage7MaxCurrent;
int32_t CTCStage8MaxCurrent;
int32_t CTCStage8MaxVoltage;
int32_t CTCStage8MaxVoltageIntervalPerHour;

int32_t SimpleCTCStage1MaxVoltage;
int32_t SimpleCTCStage1MaxCurrent;
int32_t SimpleCTCStage2MaxCurrent;
int32_t SimpleCTCStage2MaxVoltage;
int32_t SimpleCTCStage2MaxVoltageIntervalPerHour;
#define SimpleCTCStage3MaxTime (*((uint64_t*)(&(References[SIMPLE_CTC_PAUSE_TIME_HW]))))
// uint64_t SimpleCTCStage3MaxTime = (uint64_t)( (uint64_t)96000000 * (uint64_t)60 ); // (uint64_t)((uint64_t)96000000 * (uint64_t)60 * (uint64_t)60);
int32_t SimpleCTCStage4MaxCurrent;
int32_t SimpleCTCStage4MinVoltage;
#define SimpleCTCStage5MaxTime (*((uint64_t*)(&(References[SIMPLE_CTC_PAUSE_TIME_HW]))))
// uint64_t SimpleCTCStage5MaxTime = (uint64_t)( (uint64_t)96000000 * (uint64_t)60 ); // (uint64_t)((uint64_t)96000000 * (uint64_t)60 * (uint64_t)60);
int32_t SimpleCTCStage6MaxVoltage;
int32_t SimpleCTCStage6MaxCurrent;
int32_t SimpleCTCStage7MaxCurrent;
int32_t SimpleCTCStage7MaxVoltage;
int32_t SimpleCTCStage7MaxVoltageIntervalPerHour;


int32_t SimpleConstUChargeChargeVoltage;
int32_t SimpleConstUChargeMaxCurrent;
int32_t SimpleConstUChargeMinCurrent;

int32_t SimpleConstIChargeStage1MaxVoltage;
int32_t SimpleConstIChargeStage1MaxCurrent;
int32_t SimpleConstIChargeStage2MaxCurrent;
int32_t SimpleConstIChargeStage2MaxVoltage;
uint64_t SimpleConstIChargeStage3MaxTime;
int32_t SimpleConstIChargeStage2MaxVoltageIntervalPerHour;

int32_t SimpleFastCombineChargeStage1MaxCurrent;
int32_t SimpleFastCombineChargeStage1MaxVoltage;
int32_t SimpleFastCombineChargeStage1MinCurrent;
int32_t SimpleFastCombineChargeStage2MaxVoltage;
int32_t SimpleFastCombineChargeStage2MaxCurrent;
int32_t SimpleFastCombineChargeStage2MaxVoltageIntervalPerHour;

int32_t SimpleControlDischargeStage1MaxCurrent;
int32_t SimpleControlDischargeStage1MinVoltage;

int32_t SimpleCTCStage1MaxCurrent;
int32_t SimpleCTCStage1MaxVoltage;
int32_t SimpleCTCStage2MaxCurrent;
int32_t SimpleCTCStage2MaxVoltage;
int32_t SimpleCTCStage2MinVoltageIntervalPerHour;
int32_t SimpleCTCStage4MaxCurrent;

// References from menus:
uint32_t Stage1Reached;
int32_t Stage1Umax, Stage1Umin;
int32_t Stage1Imax, Stage1Imin;
uint64_t tmpStage1Volume;
int32_t Stage1Volume;
uint64_t Stage1Time;
uint64_t Stage1MaxTime;

uint32_t Stage2Reached;
int32_t Stage2Umax, Stage2Umin;
int32_t Stage2Imax, Stage2Imin;
uint64_t tmpStage2Volume;
int32_t Stage2Volume;
uint64_t Stage2Time;
uint64_t Stage2MaxTime;

uint32_t Stage3Reached;
int32_t Stage3Umax, Stage3Umin;
int32_t Stage3Imax, Stage3Imin;
uint64_t tmpStage3Volume;
int32_t Stage3Volume;
uint64_t Stage3Time;
uint64_t Stage3MaxTime;

uint32_t Stage4Reached;
int32_t Stage4Umax, Stage4Umin;
int32_t Stage4Imax, Stage4Imin;
uint64_t tmpStage4Volume;
int32_t Stage4Volume;
uint64_t Stage4Time;
uint64_t Stage4MaxTime;

uint32_t Stage5Reached;
int32_t Stage5Umax, Stage5Umin;
int32_t Stage5Imax, Stage5Imin;
uint64_t tmpStage5Volume;
int32_t Stage5Volume;
uint64_t Stage5Time;
uint64_t Stage5MaxTime;

uint32_t Stage6Reached;
int32_t Stage6Umax, Stage6Umin;
int32_t Stage6Imax, Stage6Imin;
uint64_t tmpStage6Volume;
int32_t Stage6Volume;
uint64_t Stage6Time;
uint64_t Stage6MaxTime;

uint32_t Stage7Reached;
int32_t Stage7Umax, Stage7Umin;
int32_t Stage7Imax, Stage7Imin;
uint64_t tmpStage7Volume;
int32_t Stage7Volume;
uint64_t Stage7Time;
uint64_t Stage7MaxTime;

uint32_t Stage8Reached;
int32_t Stage8Umax, Stage8Umin;
int32_t Stage8Imax, Stage8Imin;
uint64_t tmpStage8Volume;
int32_t Stage8Volume;
uint64_t Stage8Time;
uint64_t Stage8MaxTime;

uint32_t Stage9Reached;
int32_t Stage9Umax, Stage9Umin;
int32_t Stage9Imax, Stage9Imin;
uint64_t tmpStage9Volume;
int32_t Stage9Volume;
uint64_t Stage9Time;
uint64_t Stage9MaxTime;

uint32_t Stage10Reached;

uint32_t Stage6Over;
extern uint64_t TotalTime;
uint64_t ThisModeTimeStamp;

extern uint64_t FiveMinutes;
extern uint64_t FourtySeconds;

extern int32_t StatisticsNotInitialized;

extern int32_t* pMinFaultBatteryVoltage;
extern int32_t* pMaxFaultBatteryVoltage;

void ClearStage1Telemetry () {
  Stage1Reached = 0;
  Stage1Umax = 0; Stage1Umin = 10000;
  Stage1Imax = 0; Stage1Imin = 9990;
  tmpStage1Volume = (uint64_t)0;
  Stage1Volume = 0;
  Stage1Time = (uint64_t)0;
}

void ClearStage2Telemetry () {
  Stage2Reached = 0;
  Stage2Umax = 0; Stage2Umin = 10000;
  Stage2Imax = 0; Stage2Imin = 9990;
  tmpStage2Volume = (uint64_t)0;
  Stage2Volume = 0;
  Stage2Time = (uint64_t)0;
}

void ClearData () {
  
  ClearStage1Telemetry ();
  ClearStage2Telemetry ();

  Stage3Reached = 0;
  Stage3Umax = 0; Stage3Umin = 10000;
  Stage3Imax = 0; Stage3Imin = 10000;
  tmpStage3Volume = (uint64_t)0;
  Stage3Volume = 0;
  Stage3Time = (uint64_t)0;

  Stage4Reached = 0;
  Stage4Umax = 0; Stage4Umin = 10000;
  Stage4Imax = 0; Stage4Imin = 10000;
  tmpStage4Volume = (uint64_t)0;
  Stage4Volume = 0;
  Stage4Time = (uint64_t)0;

  Stage5Reached = 0;
  Stage5Umax = 0; Stage5Umin = 10000;
  Stage5Imax = 0; Stage5Imin = 10000;
  tmpStage5Volume = (uint64_t)0;
  Stage5Volume = 0;
  Stage5Time = (uint64_t)0;

  Stage6Reached = 0;
  Stage6Reached = 0;
  Stage6Umax = 0; Stage6Umin = 10000;
  Stage6Imax = 0; Stage6Imin = 10000;
  tmpStage6Volume = (uint64_t)0;
  Stage6Volume = 0;
  Stage6Time = (uint64_t)0;

  Stage7Reached = 0;
  Stage7Umax = 0; Stage7Umin = 10000;
  Stage7Imax = 0; Stage7Imin = 10000;
  tmpStage7Volume = 0;
  Stage7Volume = 0;
  Stage7Time = (uint64_t)0;

  Stage8Reached = 0;
  Stage8Umax = 0; Stage8Umin = 10000;
  Stage8Imax = 0; Stage8Imin = 10000;
  tmpStage8Volume = 0;
  Stage8Volume = 0;
  Stage8Time = (uint64_t)0;

  Stage9Reached = 0;
  Stage9Umax = 0; Stage9Umin = 10000;
  Stage9Imax = 0; Stage9Imin = 10000;
  tmpStage9Volume = 0;
  Stage9Volume = 0;
  Stage9Time = (uint64_t)0;

  tmpTotalVolume = (int64_t)0;
  TotalVolume = 0;
  //tmpTotalChargeVolume = (uint64_t)0;
  //tmpTotalDischargeVolume = (uint64_t)0;
  TotalTime = (uint64_t)0;
}

uint64_t Uzru10aTimeStamp;
uint32_t LastUzru10aState;

extern int32_t LocalUmax;
extern int32_t LocalUmin;
extern int32_t LocalImax;
extern int32_t LocalImin;

void DispatchUzru10a () {
  uint32_t tmpUzru10aState = GetSubsystemState(UZRU10A);
  switch ( tmpUzru10aState ) {
    
  case UZRU10A_OFF:
    break;
  
  case UZRU10A_CONST_U_CHARGE_START_STAGE1:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CONST_U_CHARGE_WAIT_START_STAGE1 );
    break;
  
  case UZRU10A_CONST_U_CHARGE_WAIT_START_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    else if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      GetTimeStamp ( &Uzru10aTimeStamp );
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_U_CHARGE_STAGE1 );
      // ClearData ();
      ChargeControllerSetSettings ( &ConstUChargeChargeVoltage, &ConstUChargeMaxCurrent, &ZeroValue ); // ChargeVoltage, ChargeCurrent, DischargeCurrent
      ChargeControllerSetTelemetry ( &Stage1Umax, &Stage1Umin, &Stage1Imax, &Stage1Imin, &Stage1Time, &Stage1Volume );
      StartNewRegulation ();
      Stage1Reached = 1;
      // SetRelayState ( WORK_LED, ON );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CONST_U_CHARGE_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    else if ( LocalImin <= ConstUChargeMinCurrent ) {
      Stage2Reached = 1;
      ModeStopOrPauseReason = OVER;
      StopMode ();
    }
    break;
    
  case UZRU10A_CONST_U_CHARGE_PAUSE_STAGE1:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CONST_U_CHARGE_WAIT_PAUSE_STAGE1 );
    break;
  
  case UZRU10A_CONST_U_CHARGE_WAIT_PAUSE_STAGE1:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_U_CHARGE_STAGE1_PAUSED );
      SetRelayState ( WORK_LED, PULSE );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CONST_U_CHARGE_STAGE1_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_U_CHARGE_START_STAGE1 );
    }
    break;
    
  case UZRU10A_STOP_CONST_U_CHARGE:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_WAIT_STOP_CONST_U_CHARGE );
    LoadCurrentMenu ();
    break;
  
  case UZRU10A_WAIT_STOP_CONST_U_CHARGE:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_U_CHARGE_STOPPED );
      SetRelayState ( WORK_LED, OFF );
      LoadCurrentMenu ();
    }
    break;
  
  case UZRU10A_CONST_U_CHARGE_STOPPED:
    break;
  
  
  
  case UZRU10A_CONST_I_CHARGE_START_STAGE1:
    StartNewRegulation ();
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_CHARGE_WAIT_START_STAGE1 );
    break;
  
  case UZRU10A_CONST_I_CHARGE_WAIT_START_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_CHARGE_STAGE1 );
      ChargeControllerSetSettings ( &ConstIChargeStage1MaxVoltage, &ConstIChargeStage1MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage1Umax, &Stage1Umin, &Stage1Imax, &Stage1Imin, &Stage1Time, &Stage1Volume );
      Stage1Reached = 1;
      SetRelayState ( WORK_LED, ON );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CONST_I_CHARGE_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( LocalUmax >= ConstIChargeStage1MaxVoltage ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_CHARGE_START_STAGE2 );
    }
    break;
    
  case UZRU10A_CONST_I_CHARGE_PAUSE_STAGE1:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_CHARGE_WAIT_PAUSE_STAGE1 );
    break;
  
  case UZRU10A_CONST_I_CHARGE_WAIT_PAUSE_STAGE1:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_CHARGE_STAGE1_PAUSED );
      SetRelayState ( WORK_LED, PULSE );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CONST_I_CHARGE_STAGE1_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_CHARGE_START_STAGE1 );
    }
    break;

    case UZRU10A_CONST_I_CHARGE_START_STAGE2:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_CHARGE_WAIT_START_STAGE2 );
    break;
  
  case UZRU10A_CONST_I_CHARGE_WAIT_START_STAGE2:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_CHARGE_STAGE2 );
      ChargeControllerSetSettings ( &ConstIChargeStage2MaxVoltage, &ConstIChargeStage2MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage2Umax, &Stage2Umin, &Stage2Imax, &Stage2Imin, &Stage2Time, &Stage2Volume );
      Stage2Reached = 1;
      StartVoltageIntervalPerHourControl ();
      SetRelayState ( WORK_LED, ON );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CONST_I_CHARGE_STAGE2:
    if ( Values[BATTERY_VOLTAGE] > ConstIChargeStage2MaxVoltage ) {
      SetFault ( FAULT_HIGH_BATTERY_VOLTAGE );
    }
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetVoltageIntervalPerHour () <= ConstIChargeStage2MaxVoltageIntervalPerHour ) {
      ModeStopOrPauseReason = OVER;
      StopMode ();
    }
    break;
    
  case UZRU10A_CONST_I_CHARGE_PAUSE_STAGE2:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_CHARGE_WAIT_PAUSE_STAGE2 );
    break;
  
  case UZRU10A_CONST_I_CHARGE_WAIT_PAUSE_STAGE2:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_CHARGE_STAGE2_PAUSED );
      SetRelayState ( WORK_LED, PULSE );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CONST_I_CHARGE_STAGE2_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_CHARGE_START_STAGE2 );
    }
    break;
  
  case UZRU10A_STOP_CONST_I_CHARGE:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_WAIT_STOP_CONST_I_CHARGE );
    LoadCurrentMenu ();
    break;
  
  case UZRU10A_WAIT_STOP_CONST_I_CHARGE:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_CHARGE_STOPPED );
      SetRelayState ( WORK_LED, OFF );
      LoadCurrentMenu ();
    }
    break;
  
  case UZRU10A_CONST_I_CHARGE_STOPPED:
    break;
  
  
  
  case UZRU10A_FAST_COMBINE_CHARGE_START_STAGE1:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_FAST_COMBINE_CHARGE_WAIT_START_STAGE1 );
    break;
  
  case UZRU10A_FAST_COMBINE_CHARGE_WAIT_START_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_FAST_COMBINE_CHARGE_STAGE1 );
      ChargeControllerSetSettings ( &FastCombineChargeStage1MaxVoltage, &FastCombineChargeStage1MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage1Umax, &Stage1Umin, &Stage1Imax, &Stage1Imin, &Stage1Time, &Stage1Volume );
      Stage1Reached = 1;
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_FAST_COMBINE_CHARGE_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( LocalImin <= FastCombineChargeStage1MinCurrent ) {
      SetSubsystemState ( UZRU10A, UZRU10A_FAST_COMBINE_CHARGE_WAIT_START_STAGE2 );
    }
    break;
    
  case UZRU10A_FAST_COMBINE_CHARGE_PAUSE_STAGE1:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_FAST_COMBINE_CHARGE_WAIT_PAUSE_STAGE1 );
    break;
  
  case UZRU10A_FAST_COMBINE_CHARGE_WAIT_PAUSE_STAGE1:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_FAST_COMBINE_CHARGE_STAGE1_PAUSED );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_FAST_COMBINE_CHARGE_STAGE1_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_FAST_COMBINE_CHARGE_START_STAGE1 );
    }
    break;

  case UZRU10A_FAST_COMBINE_CHARGE_START_STAGE2:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_FAST_COMBINE_CHARGE_WAIT_START_STAGE2 );
    break;
  
  case UZRU10A_FAST_COMBINE_CHARGE_WAIT_START_STAGE2:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_FAST_COMBINE_CHARGE_STAGE2 );
      ChargeControllerSetSettings ( &FastCombineChargeStage2MaxVoltage, &FastCombineChargeStage2MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage2Umax, &Stage2Umin, &Stage2Imax, &Stage2Imin, &Stage2Time, &Stage2Volume );
      Stage2Reached = 1;
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_FAST_COMBINE_CHARGE_STAGE2:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( LocalUmax >= FastCombineChargeStage2MaxVoltage ) {
      SetSubsystemState ( UZRU10A, UZRU10A_FAST_COMBINE_CHARGE_WAIT_START_STAGE3 );
    }
    break;
    
  case UZRU10A_FAST_COMBINE_CHARGE_PAUSE_STAGE2:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_FAST_COMBINE_CHARGE_WAIT_PAUSE_STAGE2 );
    break;
  
  case UZRU10A_FAST_COMBINE_CHARGE_WAIT_PAUSE_STAGE2:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_FAST_COMBINE_CHARGE_STAGE2_PAUSED );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_FAST_COMBINE_CHARGE_STAGE2_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_FAST_COMBINE_CHARGE_START_STAGE2 );
    }
    break;
  
  case UZRU10A_FAST_COMBINE_CHARGE_START_STAGE3:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_FAST_COMBINE_CHARGE_WAIT_START_STAGE3 );
    break;
  
  case UZRU10A_FAST_COMBINE_CHARGE_WAIT_START_STAGE3:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_FAST_COMBINE_CHARGE_STAGE3 );
      ChargeControllerSetSettings ( &FastCombineChargeStage3MaxVoltage, &FastCombineChargeStage3MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage3Umax, &Stage3Umin, &Stage3Imax, &Stage3Imin, &Stage3Time, &Stage3Volume );
      StartVoltageIntervalPerHourControl ();
      Stage3Reached = 1;
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_FAST_COMBINE_CHARGE_STAGE3:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetVoltageIntervalPerHour () <= FastCombineChargeStage3MaxVoltageIntervalPerHour ) {
      ModeStopOrPauseReason = OVER;
      StopMode ();
    }
    break;
    
  case UZRU10A_FAST_COMBINE_CHARGE_PAUSE_STAGE3:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_FAST_COMBINE_CHARGE_WAIT_PAUSE_STAGE3 );
    break;
  
  case UZRU10A_FAST_COMBINE_CHARGE_WAIT_PAUSE_STAGE3:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_FAST_COMBINE_CHARGE_STAGE3_PAUSED );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_STOP_FAST_COMBINE_CHARGE:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_WAIT_STOP_FAST_COMBINE_CHARGE );
    LoadCurrentMenu ();
    break;
  
  case UZRU10A_WAIT_STOP_FAST_COMBINE_CHARGE:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_FAST_COMBINE_CHARGE_STOPPED );
      LoadCurrentMenu ();
    }
    break;
  
  case UZRU10A_FAST_COMBINE_CHARGE_STOPPED:
    break;
  
  
    case UZRU10A_SLOW_COMBINE_CHARGE_START_STAGE1:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SLOW_COMBINE_CHARGE_WAIT_START_STAGE1 );
    break;
  
  case UZRU10A_SLOW_COMBINE_CHARGE_WAIT_START_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SLOW_COMBINE_CHARGE_STAGE1 );
      ChargeControllerSetSettings ( &SlowCombineChargeStage1MaxVoltage, &SlowCombineChargeStage1MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage1Umax, &Stage1Umin, &Stage1Imax, &Stage1Imin, &Stage1Time, &Stage1Volume );
      Stage1Reached = 1;
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SLOW_COMBINE_CHARGE_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( LocalUmax >= SlowCombineChargeStage1MaxVoltage ) {
      ModeStopOrPauseReason = OVER;
      SetSubsystemState ( UZRU10A, UZRU10A_SLOW_COMBINE_CHARGE_WAIT_START_STAGE2 );
    }
    break;
    
  case UZRU10A_SLOW_COMBINE_CHARGE_PAUSE_STAGE1:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SLOW_COMBINE_CHARGE_WAIT_PAUSE_STAGE1 );
    break;
  
  case UZRU10A_SLOW_COMBINE_CHARGE_WAIT_PAUSE_STAGE1:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SLOW_COMBINE_CHARGE_STAGE1_PAUSED );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SLOW_COMBINE_CHARGE_STAGE1_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SLOW_COMBINE_CHARGE_START_STAGE1 );
    }
    break;

    case UZRU10A_SLOW_COMBINE_CHARGE_START_STAGE2:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SLOW_COMBINE_CHARGE_WAIT_START_STAGE2 );
    break;
  
  case UZRU10A_SLOW_COMBINE_CHARGE_WAIT_START_STAGE2:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SLOW_COMBINE_CHARGE_STAGE2 );
      ChargeControllerSetSettings ( &SlowCombineChargeStage2MaxVoltage, &SlowCombineChargeStage2MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage2Umax, &Stage2Umin, &Stage2Imax, &Stage2Imin, &Stage2Time, &Stage2Volume );
      Stage2Reached = 1;
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SLOW_COMBINE_CHARGE_STAGE2:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( LocalImin <= SlowCombineChargeStage2MinCurrent ) {
      ModeStopOrPauseReason = OVER;
      SetSubsystemState ( UZRU10A, UZRU10A_SLOW_COMBINE_CHARGE_WAIT_START_STAGE3 );
    }
    break;
    
  case UZRU10A_SLOW_COMBINE_CHARGE_PAUSE_STAGE2:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SLOW_COMBINE_CHARGE_WAIT_PAUSE_STAGE2 );
    break;
  
  case UZRU10A_SLOW_COMBINE_CHARGE_WAIT_PAUSE_STAGE2:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SLOW_COMBINE_CHARGE_STAGE2_PAUSED );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SLOW_COMBINE_CHARGE_STAGE2_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SLOW_COMBINE_CHARGE_START_STAGE2 );
    }
    break;
  
    case UZRU10A_SLOW_COMBINE_CHARGE_START_STAGE3:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SLOW_COMBINE_CHARGE_WAIT_START_STAGE3 );
    break;
  
  case UZRU10A_SLOW_COMBINE_CHARGE_WAIT_START_STAGE3:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SLOW_COMBINE_CHARGE_STAGE3 );
      ChargeControllerSetSettings ( &SlowCombineChargeStage3MaxVoltage, &SlowCombineChargeStage3MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage3Umax, &Stage3Umin, &Stage3Imax, &Stage3Imin, &Stage3Time, &Stage3Volume );
      Stage3Reached = 1;
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SLOW_COMBINE_CHARGE_STAGE3:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( (uint64_t)Stage3Time >= (uint64_t)SlowCombineChargeStage3MaxTime ) {
      ModeStopOrPauseReason = OVER;
      Stage4Reached = 1;
      StopMode ();
    }
    break;
    
  case UZRU10A_SLOW_COMBINE_CHARGE_PAUSE_STAGE3:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SLOW_COMBINE_CHARGE_WAIT_PAUSE_STAGE3 );
    break;
  
  case UZRU10A_SLOW_COMBINE_CHARGE_WAIT_PAUSE_STAGE3:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SLOW_COMBINE_CHARGE_STAGE3_PAUSED );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SLOW_COMBINE_CHARGE_STAGE3_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SLOW_COMBINE_CHARGE_START_STAGE3 );
    }
    break;
    
  case UZRU10A_STOP_SLOW_COMBINE_CHARGE:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_WAIT_STOP_SLOW_COMBINE_CHARGE );
    LoadCurrentMenu ();
    break;
  
  case UZRU10A_WAIT_STOP_SLOW_COMBINE_CHARGE:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SLOW_COMBINE_CHARGE_STOPPED );
      LoadCurrentMenu ();
    }
    break;
  
  case UZRU10A_SLOW_COMBINE_CHARGE_STOPPED:
    break;


  case UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_START_STAGE1:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_WAIT_START_STAGE1 );
    break;
  
  case UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_WAIT_START_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_STAGE1 );
      ChargeControllerSetSettings ( &ConstIWithLimitUChargeStage1MaxVoltage, &ConstIWithLimitUChargeStage1MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage1Umax, &Stage1Umin, &Stage1Imax, &Stage1Imin, &Stage1Time, &Stage1Volume );
      Stage1Reached = 1;
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( LocalImin <= ConstIWithLimitUChargeStage1MinCurrent ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_WAIT_START_STAGE2 );
    }
    break;
    
  case UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_PAUSE_STAGE1:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_WAIT_PAUSE_STAGE1 );
    break;
  
  case UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_WAIT_PAUSE_STAGE1:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_STAGE1_PAUSED );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_STAGE1_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_START_STAGE1 );
    }
    break;

    case UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_START_STAGE2:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_WAIT_START_STAGE2 );
    break;
  
  case UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_WAIT_START_STAGE2:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_STAGE2 );
      ChargeControllerSetSettings ( &ConstIWithLimitUChargeStage2MaxVoltage, &ConstIWithLimitUChargeStage2MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage2Umax, &Stage2Umin, &Stage2Imax, &Stage2Imin, &Stage2Time, &Stage2Volume );
      Stage2Reached = 1;
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_STAGE2:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( LocalImin <= ConstIWithLimitUChargeStage2MinCurrent ) {
      ModeStopOrPauseReason = OVER;
      SetSubsystemState ( UZRU10A, UZRU10A_STOP_CONST_I_WITH_LIMIT_U_CHARGE );
    }
    break;
    
  case UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_PAUSE_STAGE2:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_WAIT_PAUSE_STAGE2 );
    break;
  
  case UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_WAIT_PAUSE_STAGE2:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_STAGE2_PAUSED );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_STAGE2_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_START_STAGE2 );
    }
    break;
  
  case UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_START_STAGE3:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_WAIT_START_STAGE3 );
    break;
  
  case UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_WAIT_START_STAGE3:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_STAGE3 );
      ChargeControllerSetSettings ( &ConstIWithLimitUChargeStage3MaxVoltage, &ConstIWithLimitUChargeStage3MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage3Umax, &Stage3Umin, &Stage3Imax, &Stage3Imin, &Stage3Time, &Stage3Volume );
      Stage3Reached = 1;
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_STAGE3:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( (uint64_t)Stage3Time >= (uint64_t)ConstIWithLimitUChargeStage3MaxTime ) {
      ModeStopOrPauseReason = OVER;
      Stage4Reached = 1;
      StopMode ();
    }
    break;
    
  case UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_PAUSE_STAGE3:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_WAIT_PAUSE_STAGE3 );
    break;
  
  case UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_WAIT_PAUSE_STAGE3:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_STAGE3_PAUSED );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_STAGE3_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_START_STAGE3 );
    }
    break;
    
  case UZRU10A_STOP_CONST_I_WITH_LIMIT_U_CHARGE:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_WAIT_STOP_CONST_I_WITH_LIMIT_U_CHARGE );
    LoadCurrentMenu ();
    break;
  
  case UZRU10A_WAIT_STOP_CONST_I_WITH_LIMIT_U_CHARGE:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_STOPPED );
      LoadCurrentMenu ();
    }
    break;
  
  case UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_STOPPED:
    break;
  

    case UZRU10A_ASIMM_CHARGE_START_STAGE1:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_LOCAL_DISCHARGE_FOR_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_WAIT_START_STAGE1 );
    break;
  
  case UZRU10A_ASIMM_CHARGE_WAIT_START_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_STAGE1 );
      ClearSignals ();
      ChargeControllerSetSettings ( &AsimmChargeStage1MaxVoltage, &AsimmChargeStage1MaxCurrent, &ZeroValue );
      ClearStage1Telemetry ();
      ChargeControllerSetTelemetry ( &Stage1Umax, &Stage1Umin, &Stage1Imax, &Stage1Imin, &Stage1Time, &Stage1Volume );
      Stage1Reached = 1;
      Stage1Time = (uint64_t)0;
      Stage8Reached = 0;
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_ASIMM_CHARGE_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( (uint64_t)Stage1Time >= (uint64_t)FiveMinutes ) {
      SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_START_STAGE2 );
    }
    else if ( LocalUmax >= AsimmChargeStage1MaxVoltage ) {
      SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_WAIT_START_STAGE3 );
    }
    break;
    
  case UZRU10A_ASIMM_CHARGE_PAUSE_STAGE1:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_WAIT_PAUSE_STAGE1 );
    break;
  
  case UZRU10A_ASIMM_CHARGE_WAIT_PAUSE_STAGE1:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_STAGE1_PAUSED );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_ASIMM_CHARGE_STAGE1_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_START_STAGE1 );
    }
    break;

    case UZRU10A_ASIMM_CHARGE_START_STAGE2:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE_FOR_LOCAL_DISCHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_WAIT_START_STAGE2 );
    break;
  
  case UZRU10A_ASIMM_CHARGE_WAIT_START_STAGE2:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_LOCAL_DISCHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_STAGE2 );
      ClearStage2Telemetry ();
      ChargeControllerSetSettings ( &ZeroValue, &ZeroValue, &AsimmChargeStage2MaxCurrent );
      ChargeControllerSetTelemetry ( &Stage2Umax, &Stage2Umin, &Stage2Imax, &Stage2Imin, &Stage2Time, &Stage2Volume );
      Stage2Reached = 1;
      Stage8Reached = 1;
      Stage2Time = (uint64_t)0;
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_ASIMM_CHARGE_STAGE2:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( ( LocalUmin <= AsimmChargeStage2MinVoltage ) || ( (uint64_t)Stage2Time >= (uint64_t)FourtySeconds ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_START_STAGE1 );
    }
    break;
    
  case UZRU10A_ASIMM_CHARGE_PAUSE_STAGE2:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_WAIT_PAUSE_STAGE2 );
    break;
  
  case UZRU10A_ASIMM_CHARGE_WAIT_PAUSE_STAGE2:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_STAGE2_PAUSED );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_ASIMM_CHARGE_STAGE2_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_START_STAGE2 );
    }
    break;
  
    case UZRU10A_ASIMM_CHARGE_START_STAGE3:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_WAIT_START_STAGE3 );
    break;
  
  case UZRU10A_ASIMM_CHARGE_WAIT_START_STAGE3:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_STAGE3 );
      ChargeControllerSetSettings ( &AsimmChargeStage3MaxVoltage, &AsimmChargeStage3MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage3Umax, &Stage3Umin, &Stage3Imax, &Stage3Imin, &Stage3Time, &Stage3Volume );
      Stage3Reached = 1;
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_ASIMM_CHARGE_STAGE3:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( LocalImin <= AsimmChargeStage3MinCurrent ) {
      SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_WAIT_START_STAGE4 );
    }
    break;
    
  case UZRU10A_ASIMM_CHARGE_PAUSE_STAGE3:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_WAIT_PAUSE_STAGE3 );
    break;
  
  case UZRU10A_ASIMM_CHARGE_WAIT_PAUSE_STAGE3:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_STAGE3_PAUSED );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_ASIMM_CHARGE_STAGE3_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_START_STAGE3 );
    }
    break;
    
  case UZRU10A_ASIMM_CHARGE_START_STAGE4:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_WAIT_START_STAGE4 );
    break;
  
  case UZRU10A_ASIMM_CHARGE_WAIT_START_STAGE4:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_STAGE4 );
      ChargeControllerSetSettings ( &AsimmChargeStage4MaxVoltage, &AsimmChargeStage4MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage4Umax, &Stage4Umin, &Stage4Imax, &Stage4Imin, &Stage4Time, &Stage4Volume );
      StartVoltageIntervalPerHourControl ();
      Stage4Reached = 1;
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_ASIMM_CHARGE_STAGE4:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetVoltageIntervalPerHour () <= AsimmChargeStage4MaxVoltageIntervalPerHour ) {
      Stage5Reached = 1;
      ModeStopOrPauseReason = OVER;
      StopMode ();
    }
    break;
    
  case UZRU10A_ASIMM_CHARGE_PAUSE_STAGE4:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_WAIT_PAUSE_STAGE4 );
    break;
  
  case UZRU10A_ASIMM_CHARGE_WAIT_PAUSE_STAGE4:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_STAGE4_PAUSED );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_ASIMM_CHARGE_STAGE4_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_START_STAGE4 );
    }
    break;
    
  case UZRU10A_STOP_ASIMM_CHARGE:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_WAIT_STOP_ASIMM_CHARGE );
    LoadCurrentMenu ();
    break;
  
  case UZRU10A_WAIT_STOP_ASIMM_CHARGE:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_STOPPED );
      LoadCurrentMenu ();
    }
    break;
  
  case UZRU10A_ASIMM_CHARGE_STOPPED:
    break;
  


  
  case UZRU10A_CONTROL_DISCHARGE_START_DISCHARGE:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_GLOBAL_DISCHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CONTROL_DISCHARGE_WAIT_START_DISCHARGE );
    break;
  
  case UZRU10A_CONTROL_DISCHARGE_WAIT_START_DISCHARGE:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_DISCHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_GLOBAL_DISCHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONTROL_DISCHARGE_DISCHARGE );
      ChargeControllerSetSettings ( &ZeroValue, &ZeroValue, &ControlDischargeStage1MaxCurrent );
      ChargeControllerSetTelemetry ( &Stage1Umax, &Stage1Umin, &Stage1Imax, &Stage1Imin, &Stage1Time, &Stage1Volume );
      Stage1Reached = 1;
      SetRelayState ( WORK_LED, ON );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CONTROL_DISCHARGE_DISCHARGE:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_DISCHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( LocalUmin <= ControlDischargeStage1MinVoltage ) {
      ModeStopOrPauseReason = OVER;
      StopMode ();
    }
    break;
    
  case UZRU10A_CONTROL_DISCHARGE_PAUSE_DISCHARGE:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_GLOBAL_DISCHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CONTROL_DISCHARGE_WAIT_PAUSE_DISCHARGE );
    break;
  
  case UZRU10A_CONTROL_DISCHARGE_WAIT_PAUSE_DISCHARGE:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONTROL_DISCHARGE_DISCHARGE_PAUSED );
      SetRelayState ( WORK_LED, PULSE );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CONTROL_DISCHARGE_DISCHARGE_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONTROL_DISCHARGE_START_DISCHARGE );
    }
    break;
  
  case UZRU10A_STOP_CONTROL_DISCHARGE:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_GLOBAL_DISCHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_WAIT_STOP_CONTROL_DISCHARGE );
    LoadCurrentMenu ();
    break;
  
  case UZRU10A_WAIT_STOP_CONTROL_DISCHARGE:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CONTROL_DISCHARGE_STOPPED );
      SetRelayState ( WORK_LED, OFF );
      LoadCurrentMenu ();
    }
    break;
  
  case UZRU10A_CONTROL_DISCHARGE_STOPPED:
    break;
  
  
  
  
  case UZRU10A_CTC_START_STAGE1:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CTC_WAIT_START_STAGE1 );
    break;
  
  case UZRU10A_CTC_WAIT_START_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_STAGE1 );
      ChargeControllerSetSettings ( &CTCStage1MaxVoltage, &CTCStage1MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage1Umax, &Stage1Umin, &Stage1Imax, &Stage1Imin, &Stage1Time, &Stage1Volume );
      Stage1Reached = 1;
      SetRelayState ( WORK_LED, ON );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CTC_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( LocalUmax >= CTCStage1MaxVoltage ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_START_STAGE2 );
    }
    break;
    
  case UZRU10A_CTC_PAUSE_STAGE1:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CTC_WAIT_PAUSE_STAGE1 );
    break;
  
  case UZRU10A_CTC_WAIT_PAUSE_STAGE1:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_STAGE1_PAUSED );
      SetRelayState ( WORK_LED, PULSE );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CTC_STAGE1_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_START_STAGE1 );
    }
    break;

    case UZRU10A_CTC_START_STAGE2:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CTC_WAIT_START_STAGE2 );
    break;
  
  case UZRU10A_CTC_WAIT_START_STAGE2:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_STAGE2 );
      ChargeControllerSetSettings ( &TotalMaxVoltage, &CTCStage2MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage2Umax, &Stage2Umin, &Stage2Imax, &Stage2Imin, &Stage2Time, &Stage2Volume );
      StartVoltageIntervalPerHourControl ();
      Stage2Reached = 1;
      SetRelayState ( WORK_LED, ON );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CTC_STAGE2:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetVoltageIntervalPerHour () <= CTCStage2MaxVoltageIntervalPerHour ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_START_STAGE4 );
    }
    break;
    
  case UZRU10A_CTC_PAUSE_STAGE2:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CTC_WAIT_PAUSE_STAGE2 );
    break;
  
  case UZRU10A_CTC_WAIT_PAUSE_STAGE2:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_STAGE2_PAUSED );
      SetRelayState ( WORK_LED, PULSE );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CTC_STAGE2_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_START_STAGE2 );
    }
    break;
  
  case UZRU10A_CTC_START_STAGE4:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CTC_WAIT_START_STAGE4 );
    break;
  
  case UZRU10A_CTC_WAIT_START_STAGE4:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_STAGE4 );
      ChargeControllerSetTelemetry ( &Stage4Umax, &Stage4Umin, &Stage4Imax, &Stage4Imin, &Stage4Time, &Stage4Volume );
      GetTimeStamp ( &ThisModeTimeStamp );
      Stage4Reached = 1;
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CTC_STAGE4:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    GetIntervalFrom ( (uint64_t)ThisModeTimeStamp, &Stage4Time );
    if ( (uint64_t)Stage4Time >= (uint64_t)CTCStage4MaxTime ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_START_STAGE5 );
    }
    break;
    
  case UZRU10A_CTC_PAUSE_STAGE4:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CTC_WAIT_PAUSE_STAGE4 );
    break;
  
  case UZRU10A_CTC_WAIT_PAUSE_STAGE4:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_STAGE4_PAUSED );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CTC_STAGE4_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_START_STAGE4 );
    }
    break;  

  case UZRU10A_CTC_START_STAGE5:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_GLOBAL_DISCHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CTC_WAIT_START_STAGE5 );
    break;
  
  case UZRU10A_CTC_WAIT_START_STAGE5:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_DISCHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_GLOBAL_DISCHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_STAGE5 );
      ChargeControllerSetSettings ( &ZeroValue, &ZeroValue, &CTCStage5MaxCurrent );
      ChargeControllerSetTelemetry ( &Stage5Umax, &Stage5Umin, &Stage5Imax, &Stage5Imin, &Stage5Time, &Stage5Volume );
      Stage5Reached = 1;
      SetRelayState ( WORK_LED, ON );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CTC_STAGE5:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_DISCHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( LocalUmin <= CTCStage5MinVoltage ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_START_STAGE6 );
    }
    break;
    
  case UZRU10A_CTC_PAUSE_STAGE5:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_GLOBAL_DISCHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CTC_WAIT_PAUSE_STAGE5 );
    break;
  
  case UZRU10A_CTC_WAIT_PAUSE_STAGE5:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_STAGE5_PAUSED );
      SetRelayState ( WORK_LED, PULSE );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CTC_STAGE5_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_START_STAGE5 );
    }
    break;
  
  case UZRU10A_CTC_START_STAGE6:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_GLOBAL_DISCHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CTC_WAIT_START_STAGE6 );
    break;
  
  case UZRU10A_CTC_WAIT_START_STAGE6:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_STAGE6 );
      ChargeControllerSetTelemetry ( &Stage6Umax, &Stage6Umin, &Stage6Imax, &Stage6Imin, &Stage6Time, &Stage6Volume );
      GetTimeStamp ( &ThisModeTimeStamp );
      Stage6Reached = 1;
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CTC_STAGE6:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    GetIntervalFrom ( (uint64_t)ThisModeTimeStamp, &Stage6Time );
    if ( (uint64_t)Stage6Time >= (uint64_t)CTCStage6MaxTime ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_START_STAGE7 );
    }
    break;
    
  case UZRU10A_CTC_PAUSE_STAGE6:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CTC_WAIT_PAUSE_STAGE6 );
    break;
  
  case UZRU10A_CTC_WAIT_PAUSE_STAGE6:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_STAGE6_PAUSED );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CTC_STAGE6_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_START_STAGE6 );
    }
    break;  
  
  case UZRU10A_CTC_START_STAGE7:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CTC_WAIT_START_STAGE7 );
    break;
  
  case UZRU10A_CTC_WAIT_START_STAGE7:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_STAGE7 );
      ChargeControllerSetSettings ( &CTCStage7MaxVoltage, &CTCStage7MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage7Umax, &Stage7Umin, &Stage7Imax, &Stage7Imin, &Stage7Time, &Stage7Volume );
      Stage7Reached = 1;
      SetRelayState ( WORK_LED, ON );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CTC_STAGE7:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( LocalUmax >= CTCStage7MaxVoltage ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_START_STAGE8 );
    }
    break;
    
  case UZRU10A_CTC_PAUSE_STAGE7:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CTC_WAIT_PAUSE_STAGE7 );
    break;
  
  case UZRU10A_CTC_WAIT_PAUSE_STAGE7:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_STAGE7_PAUSED );
      SetRelayState ( WORK_LED, PULSE );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CTC_STAGE7_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_START_STAGE7 );
    }
    break;

    case UZRU10A_CTC_START_STAGE8:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CTC_WAIT_START_STAGE8 );
    break;
  
  case UZRU10A_CTC_WAIT_START_STAGE8:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_STAGE8 );
      ChargeControllerSetSettings ( &TotalMaxVoltage, &CTCStage8MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage8Umax, &Stage8Umin, &Stage8Imax, &Stage8Imin, &Stage8Time, &Stage8Volume );
      StartVoltageIntervalPerHourControl ();
      Stage8Reached = 1;
      SetRelayState ( WORK_LED, ON );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CTC_STAGE8:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetVoltageIntervalPerHour () <= CTCStage8MaxVoltageIntervalPerHour ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_START_STAGE9 );
    }
    break;
    
  case UZRU10A_CTC_PAUSE_STAGE8:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_CTC_WAIT_PAUSE_STAGE8 );
    break;
  
  case UZRU10A_CTC_WAIT_PAUSE_STAGE8:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_STAGE8_PAUSED );
      SetRelayState ( WORK_LED, PULSE );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_CTC_STAGE8_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      ModeStopOrPauseReason = OVER;
      StopMode ();
    }
    break;
 
  case UZRU10A_STOP_CTC:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_WAIT_STOP_CTC );
    LoadCurrentMenu ();
    break;
  
  case UZRU10A_WAIT_STOP_CTC:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_STOPPED );
      LoadCurrentMenu ();
    }
    break;
  
  case UZRU10A_CTC_STOPPED:
    break;

  
  
  
  case UZRU10A_SIMPLE_CTC_START_STAGE1:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_WAIT_START_STAGE1 );
    break;
  
  case UZRU10A_SIMPLE_CTC_WAIT_START_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_STAGE1 );
      ChargeControllerSetSettings ( &SimpleCTCStage1MaxVoltage, &SimpleCTCStage1MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage1Umax, &Stage1Umin, &Stage1Imax, &Stage1Imin, &Stage1Time, &Stage1Volume );
      Stage1Reached = 1;
      StartNewRegulation ();
      SetRelayState ( WORK_LED, ON );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_CTC_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( ( !StatisticsNotInitialized ) && ( LocalUmin >= SimpleCTCStage1MaxVoltage ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_START_STAGE2 );
    }
    break;
    
  case UZRU10A_SIMPLE_CTC_PAUSE_STAGE1:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_WAIT_PAUSE_STAGE1 );
    break;
  
  case UZRU10A_SIMPLE_CTC_WAIT_PAUSE_STAGE1:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_STAGE1_PAUSED );
      SetRelayState ( WORK_LED, PULSE );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_CTC_STAGE1_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_START_STAGE1 );
    }
    break;

    case UZRU10A_SIMPLE_CTC_START_STAGE2:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_WAIT_START_STAGE2 );
    break;
  
  case UZRU10A_SIMPLE_CTC_WAIT_START_STAGE2:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_STAGE2 );
      ChargeControllerSetSettings ( &SimpleCTCStage2MaxVoltage, &SimpleCTCStage2MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage2Umax, &Stage2Umin, &Stage2Imax, &Stage2Imin, &Stage1Time, &Stage1Volume );
      StartVoltageIntervalPerHourControl ();
      StartNewRegulation ();
      Stage2Reached = 1;
      SetRelayState ( WORK_LED, ON );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_CTC_STAGE2:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetVoltageIntervalPerHour () <= SimpleCTCStage2MinVoltageIntervalPerHour ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_START_STAGE3 );
    }
    break;
    
  case UZRU10A_SIMPLE_CTC_PAUSE_STAGE2:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_WAIT_PAUSE_STAGE2 );
    break;
  
  case UZRU10A_SIMPLE_CTC_WAIT_PAUSE_STAGE2:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_STAGE2_PAUSED );
      SetRelayState ( WORK_LED, PULSE );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_CTC_STAGE2_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_START_STAGE2 );
    }
    break;
  
  case UZRU10A_SIMPLE_CTC_START_STAGE3:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_WAIT_START_STAGE3 );
    break;
  
  case UZRU10A_SIMPLE_CTC_WAIT_START_STAGE3:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_STAGE3 );
      ChargeControllerSetTelemetry ( &Stage3Umax, &Stage3Umin, &Stage3Imax, &Stage3Imin, &Stage3Time, &Stage3Volume );
      GetTimeStamp ( &ThisModeTimeStamp );
      Stage3Reached = 1;
      LoadCurrentMenu ();
      SetSubsystemState ( PRECHARGE_CONTROLLER, PRECHARGE_SET_ON );
    }
    break;
    
  case UZRU10A_SIMPLE_CTC_STAGE3:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    GetIntervalFrom ( (uint64_t)ThisModeTimeStamp, &Stage3Time );
    if ( (uint64_t)Stage3Time >= (uint64_t)SimpleCTCStage3MaxTime ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_START_STAGE4 );
    }
    break;
    
  case UZRU10A_SIMPLE_CTC_PAUSE_STAGE3:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_WAIT_PAUSE_STAGE3 );
    break;
  
  case UZRU10A_SIMPLE_CTC_WAIT_PAUSE_STAGE3:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_STAGE3_PAUSED );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_CTC_STAGE3_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_START_STAGE3 );
    }
    break;  

  case UZRU10A_SIMPLE_CTC_START_STAGE4:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_GLOBAL_DISCHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_WAIT_START_STAGE4 );
    break;
  
  case UZRU10A_SIMPLE_CTC_WAIT_START_STAGE4:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_DISCHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_GLOBAL_DISCHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_STAGE4 );
      ChargeControllerSetSettings ( &ZeroValue, &ZeroValue, &SimpleCTCStage4MaxCurrent );
      ChargeControllerSetTelemetry ( &Stage4Umax, &Stage4Umin, &Stage4Imax, &Stage4Imin, &Stage4Time, &Stage4Volume );
      Stage4Reached = 1;
      SetRelayState ( WORK_LED, ON );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_CTC_STAGE4:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_DISCHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( ( !StatisticsNotInitialized ) && ( LocalUmax <= SimpleCTCStage4MinVoltage ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_START_STAGE5 );
    }
    break;
    
  case UZRU10A_SIMPLE_CTC_PAUSE_STAGE4:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_GLOBAL_DISCHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_WAIT_PAUSE_STAGE4 );
    break;
  
  case UZRU10A_SIMPLE_CTC_WAIT_PAUSE_STAGE4:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_STAGE4_PAUSED );
      SetRelayState ( WORK_LED, PULSE );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_CTC_STAGE4_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_START_STAGE4 );
    }
    break;
  
  case UZRU10A_SIMPLE_CTC_START_STAGE5:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_GLOBAL_DISCHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_WAIT_START_STAGE5 );
    break;
  
  case UZRU10A_SIMPLE_CTC_WAIT_START_STAGE5:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_STAGE5 );
      ChargeControllerSetTelemetry ( &Stage5Umax, &Stage5Umin, &Stage5Imax, &Stage5Imin, &Stage5Time, &Stage5Volume );
      GetTimeStamp ( &ThisModeTimeStamp );
      Stage5Reached = 1;
      LoadCurrentMenu ();
      SetSubsystemState ( PRECHARGE_CONTROLLER, PRECHARGE_SET_ON );
    }
    break;
    
  case UZRU10A_SIMPLE_CTC_STAGE5:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    GetIntervalFrom ( (uint64_t)ThisModeTimeStamp, &Stage5Time );
    if ( (uint64_t)Stage5Time >= (uint64_t)SimpleCTCStage5MaxTime ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_START_STAGE6 );
    }
    break;
    
  case UZRU10A_SIMPLE_CTC_PAUSE_STAGE5:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_WAIT_PAUSE_STAGE5 );
    break;
  
  case UZRU10A_SIMPLE_CTC_WAIT_PAUSE_STAGE5:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_STAGE5_PAUSED );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_CTC_STAGE5_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_START_STAGE5 );
    }
    break;  
  
  case UZRU10A_SIMPLE_CTC_START_STAGE6:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_WAIT_START_STAGE6 );
    break;
  
  case UZRU10A_SIMPLE_CTC_WAIT_START_STAGE6:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_STAGE6 );
      ChargeControllerSetSettings ( &SimpleCTCStage1MaxVoltage, &SimpleCTCStage1MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage6Umax, &Stage6Umin, &Stage6Imax, &Stage6Imin, &Stage6Time, &Stage6Volume );
      StartNewRegulation ();
      Stage6Reached = 1;
      SetRelayState ( WORK_LED, ON );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_CTC_STAGE6:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( ( !StatisticsNotInitialized ) && ( LocalUmin >= SimpleCTCStage1MaxVoltage ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_START_STAGE7 );
    }
    break;
    
  case UZRU10A_SIMPLE_CTC_PAUSE_STAGE6:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_WAIT_PAUSE_STAGE6 );
    break;
  
  case UZRU10A_SIMPLE_CTC_WAIT_PAUSE_STAGE6:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_STAGE6_PAUSED );
      SetRelayState ( WORK_LED, PULSE );
      LoadCurrentMenu ();
    }
    break;

  case UZRU10A_SIMPLE_CTC_STAGE6_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_START_STAGE6 );
    }
    break; 
    
  case UZRU10A_SIMPLE_CTC_START_STAGE7:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_WAIT_START_STAGE7 );
    break;
  
  case UZRU10A_SIMPLE_CTC_WAIT_START_STAGE7:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_STAGE7 );
      ChargeControllerSetSettings ( &SimpleCTCStage2MaxVoltage, &SimpleCTCStage2MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage7Umax, &Stage7Umin, &Stage7Imax, &Stage7Imin, &Stage6Time, &Stage6Volume );
      StartVoltageIntervalPerHourControl ();
      StartNewRegulation ();
      Stage2Reached = 1;
      SetRelayState ( WORK_LED, ON );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_CTC_STAGE7:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetVoltageIntervalPerHour () <= SimpleCTCStage2MinVoltageIntervalPerHour ) {
      ModeStopOrPauseReason = OVER;
      StopMode ();
    }
    break;
    
  case UZRU10A_SIMPLE_CTC_PAUSE_STAGE7:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_WAIT_PAUSE_STAGE7 );
    break;
  
  case UZRU10A_SIMPLE_CTC_WAIT_PAUSE_STAGE7:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_STAGE7_PAUSED );
      SetRelayState ( WORK_LED, PULSE );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_CTC_STAGE7_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_START_STAGE7 );
    }
    break;  
  case UZRU10A_STOP_SIMPLE_CTC:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_WAIT_STOP_SIMPLE_CTC );
    LoadCurrentMenu ();
    break;
  
  case UZRU10A_WAIT_STOP_SIMPLE_CTC:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_STOPPED );
      LoadCurrentMenu ();
    }
    break;
  
  case UZRU10A_SIMPLE_CTC_STOPPED:
    break;


  
  

  case UZRU10A_CLEAR:
    ClearData ();
    SetSubsystemState ( UZRU10A, UZRU10A_OFF );
    LoadCurrentMenu ();
    break;
  
  case UZRU10A_START_BATTERY_VOLTAGE_MEASURE_CALIBRATION:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_DIRECT_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_WAIT_START_BATTERY_VOLTAGE_MEASURE_CALIBRATION );
    break;
  
  case UZRU10A_WAIT_START_BATTERY_VOLTAGE_MEASURE_CALIBRATION:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_DIRECT_CHARGE ) {
      GetTimeStamp ( &Uzru10aTimeStamp );
      SetSubsystemState ( UZRU10A, UZRU10A_BATTERY_VOLTAGE_MEASURE_CALIBRATION );
      // ClearData ();
      SetRelayState ( WORK_LED, ON );
    }
    break;
    
  case UZRU10A_BATTERY_VOLTAGE_MEASURE_CALIBRATION:
    break;
    
  case UZRU10A_STOP_BATTERY_VOLTAGE_MEASURE_CALIBRATION:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_DIRECT_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_WAIT_STOP_BATTERY_VOLTAGE_MEASURE_CALIBRATION );
    break;
  
  case UZRU10A_WAIT_STOP_BATTERY_VOLTAGE_MEASURE_CALIBRATION:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_OFF );
      SetRelayState ( WORK_LED, OFF );
    }
    break;
    
  case UZRU10A_START_CHARGE_CURRENT_MEASURE_CALIBRATION:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_DIRECT_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_WAIT_START_CHARGE_CURRENT_MEASURE_CALIBRATION );
    break;
  
  case UZRU10A_WAIT_START_CHARGE_CURRENT_MEASURE_CALIBRATION:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_DIRECT_CHARGE ) {
      GetTimeStamp ( &Uzru10aTimeStamp );
      SetSubsystemState ( UZRU10A, UZRU10A_CHARGE_CURRENT_MEASURE_CALIBRATION );
      // ClearData ();
      SetRelayState ( WORK_LED, ON );
    }
    break;
    
  case UZRU10A_CHARGE_CURRENT_MEASURE_CALIBRATION:
    break;
    
  case UZRU10A_STOP_CHARGE_CURRENT_MEASURE_CALIBRATION:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_DIRECT_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_WAIT_STOP_CHARGE_CURRENT_MEASURE_CALIBRATION );
    break;
  
  case UZRU10A_WAIT_STOP_CHARGE_CURRENT_MEASURE_CALIBRATION:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_OFF );
      SetRelayState ( WORK_LED, OFF );
    }
    break;
    
  case UZRU10A_START_DISCHARGE_CURRENT_MEASURE_CALIBRATION:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_DIRECT_GLOBAL_DISCHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_WAIT_START_DISCHARGE_CURRENT_MEASURE_CALIBRATION );
    break;
  
  case UZRU10A_WAIT_START_DISCHARGE_CURRENT_MEASURE_CALIBRATION:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_DIRECT_GLOBAL_DISCHARGE ) {
      GetTimeStamp ( &Uzru10aTimeStamp );
      SetSubsystemState ( UZRU10A, UZRU10A_DISCHARGE_CURRENT_MEASURE_CALIBRATION );
      // ClearData ();
      SetRelayState ( WORK_LED, ON );
    }
    break;
    
  case UZRU10A_DISCHARGE_CURRENT_MEASURE_CALIBRATION:
    break;
    
  case UZRU10A_STOP_DISCHARGE_CURRENT_MEASURE_CALIBRATION:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_DIRECT_GLOBAL_DISCHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_WAIT_STOP_DISCHARGE_CURRENT_MEASURE_CALIBRATION );
    break;
  
  case UZRU10A_WAIT_STOP_DISCHARGE_CURRENT_MEASURE_CALIBRATION:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_OFF );
      SetRelayState ( WORK_LED, OFF );
    }
    break;
    
  case UZRU10A_SIMPLE_CONST_U_CHARGE_START_STAGE1:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONST_U_CHARGE_WAIT_START_STAGE1 );
    break;
  
  case UZRU10A_SIMPLE_CONST_U_CHARGE_WAIT_START_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    else if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      GetTimeStamp ( &Uzru10aTimeStamp );
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONST_U_CHARGE_STAGE1 );
      ChargeControllerSetSettings ( &SimpleConstUChargeChargeVoltage, &SimpleConstUChargeMaxCurrent, &ZeroValue ); // ChargeVoltage, ChargeCurrent, DischargeCurrent
      ChargeControllerSetTelemetry ( &Stage1Umax, &Stage1Umin, &Stage1Imax, &Stage1Imin, &Stage1Time, &Stage1Volume );
      StartNewRegulation ();
      // ClearData ();
      Stage1Reached = 1;
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_CONST_U_CHARGE_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    else if ( ( !StatisticsNotInitialized ) && ( LocalImax <= SimpleConstUChargeMinCurrent ) ) {
      Stage2Reached = 1;
      ModeStopOrPauseReason = OVER;
      StopMode ();
    }
    break;
    
  case UZRU10A_SIMPLE_CONST_U_CHARGE_PAUSE_STAGE1:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONST_U_CHARGE_WAIT_PAUSE_STAGE1 );
    break;
  
  case UZRU10A_SIMPLE_CONST_U_CHARGE_WAIT_PAUSE_STAGE1:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONST_U_CHARGE_STAGE1_PAUSED );
      SetRelayState ( WORK_LED, PULSE );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_CONST_U_CHARGE_STAGE1_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONST_U_CHARGE_START_STAGE1 );
    }
    break;
    
  case UZRU10A_STOP_SIMPLE_CONST_U_CHARGE:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_WAIT_STOP_SIMPLE_CONST_U_CHARGE );
    LoadCurrentMenu ();
    break;
  
  case UZRU10A_WAIT_STOP_SIMPLE_CONST_U_CHARGE:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONST_U_CHARGE_STOPPED );
      SetRelayState ( WORK_LED, OFF );
      LoadCurrentMenu ();
    }
    break;
  
  case UZRU10A_SIMPLE_CONST_U_CHARGE_STOPPED:
    break;

  case UZRU10A_SIMPLE_CONST_I_CHARGE_START_STAGE1:
    StartNewRegulation ();
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONST_I_CHARGE_WAIT_START_STAGE1 );
    break;
  
  case UZRU10A_SIMPLE_CONST_I_CHARGE_WAIT_START_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONST_I_CHARGE_STAGE1 );
      ChargeControllerSetSettings ( &SimpleConstIChargeStage1MaxVoltage, &SimpleConstIChargeStage1MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage1Umax, &Stage1Umin, &Stage1Imax, &Stage1Imin, &Stage1Time, &Stage1Volume );
      Stage1Reached = 1;
      SetRelayState ( WORK_LED, ON );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_CONST_I_CHARGE_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( ( !StatisticsNotInitialized ) && ( LocalUmin >= SimpleConstIChargeStage1MaxVoltage ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONST_I_CHARGE_START_STAGE2 );
    }
    break;
    
  case UZRU10A_SIMPLE_CONST_I_CHARGE_PAUSE_STAGE1:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONST_I_CHARGE_WAIT_PAUSE_STAGE1 );
    break;
  
  case UZRU10A_SIMPLE_CONST_I_CHARGE_WAIT_PAUSE_STAGE1:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONST_I_CHARGE_STAGE1_PAUSED );
      SetRelayState ( WORK_LED, PULSE );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_CONST_I_CHARGE_STAGE1_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONST_I_CHARGE_START_STAGE1 );
    }
    break;

    case UZRU10A_SIMPLE_CONST_I_CHARGE_START_STAGE2:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONST_I_CHARGE_WAIT_START_STAGE2 );
    break;
  
  case UZRU10A_SIMPLE_CONST_I_CHARGE_WAIT_START_STAGE2:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONST_I_CHARGE_STAGE2 );
      ChargeControllerSetSettings ( &SimpleConstIChargeStage2MaxVoltage, &SimpleConstIChargeStage2MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage2Umax, &Stage2Umin, &Stage2Imax, &Stage2Imin, &Stage1Time, &Stage1Volume );
      Stage2Reached = 1;
      StartVoltageIntervalPerHourControl ();
      SetRelayState ( WORK_LED, ON );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_CONST_I_CHARGE_STAGE2:
    if ( Values[BATTERY_VOLTAGE] > SimpleConstIChargeStage2MaxVoltage ) {
      SetFault ( FAULT_HIGH_BATTERY_VOLTAGE );
    }
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetVoltageIntervalPerHour () <= SimpleConstIChargeStage2MaxVoltageIntervalPerHour ) {
      ModeStopOrPauseReason = OVER;
      StopMode (); // SetSubsystemState ( UZRU10A, UZRU10A_STOP_SIMPLE_CONST_I_CHARGE );
    }
    break;
    
  case UZRU10A_SIMPLE_CONST_I_CHARGE_PAUSE_STAGE2:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONST_I_CHARGE_WAIT_PAUSE_STAGE2 );
    break;
  
  case UZRU10A_SIMPLE_CONST_I_CHARGE_WAIT_PAUSE_STAGE2:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONST_I_CHARGE_STAGE2_PAUSED );
      SetRelayState ( WORK_LED, PULSE );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_CONST_I_CHARGE_STAGE2_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONST_I_CHARGE_START_STAGE2 );
    }
    break;
  
  case UZRU10A_STOP_SIMPLE_CONST_I_CHARGE:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_WAIT_STOP_SIMPLE_CONST_I_CHARGE );
    LoadCurrentMenu ();
    break;
  
  case UZRU10A_WAIT_STOP_SIMPLE_CONST_I_CHARGE:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONST_I_CHARGE_STOPPED );
      SetRelayState ( WORK_LED, OFF );
      LoadCurrentMenu ();
    }
    break;
  
  case UZRU10A_SIMPLE_CONST_I_CHARGE_STOPPED:
    break;
  
  
  
  case UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_START_STAGE1:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_WAIT_START_STAGE1 );
    break;
  
  case UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_WAIT_START_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_STAGE1 );
      ChargeControllerSetSettings ( &SimpleFastCombineChargeStage1MaxVoltage, &SimpleFastCombineChargeStage1MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage1Umax, &Stage1Umin, &Stage1Imax, &Stage1Imin, &Stage1Time, &Stage1Volume );
      Stage1Reached = 1;
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_STAGE1:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( ( !StatisticsNotInitialized ) && ( LocalImax <= SimpleFastCombineChargeStage2MaxCurrent ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_WAIT_START_STAGE2 );
    }
    break;
    
  case UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_PAUSE_STAGE1:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_WAIT_PAUSE_STAGE1 );
    break;
  
  case UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_WAIT_PAUSE_STAGE1:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_STAGE1_PAUSED );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_STAGE1_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_START_STAGE1 );
    }
    break;

  case UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_START_STAGE2:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_WAIT_START_STAGE2 );
    break;
  
  case UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_WAIT_START_STAGE2:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_CHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_STAGE2 );
      ChargeControllerSetSettings ( &SimpleFastCombineChargeStage2MaxVoltage, &SimpleFastCombineChargeStage2MaxCurrent, &ZeroValue );
      ChargeControllerSetTelemetry ( &Stage2Umax, &Stage2Umin, &Stage2Imax, &Stage2Imin, &Stage1Time, &Stage1Volume );
      StartVoltageIntervalPerHourControl ();
      Stage2Reached = 1;
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_STAGE2:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetVoltageIntervalPerHour () <= SimpleFastCombineChargeStage2MaxVoltageIntervalPerHour ) {
      ModeStopOrPauseReason = OVER;
      StopMode ();
    }
    break;
    
  case UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_PAUSE_STAGE2:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_WAIT_PAUSE_STAGE2 );
    break;
  
  case UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_WAIT_PAUSE_STAGE2:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_STAGE2_PAUSED );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_STOP_SIMPLE_FAST_COMBINE_CHARGE:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_CHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_WAIT_STOP_SIMPLE_FAST_COMBINE_CHARGE );
    LoadCurrentMenu ();
    break;
  
  case UZRU10A_WAIT_STOP_SIMPLE_FAST_COMBINE_CHARGE:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_STOPPED );
      LoadCurrentMenu ();
    }
    break;
  
  case UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_STOPPED:
    break;
  
    case UZRU10A_SIMPLE_CONTROL_DISCHARGE_START_DISCHARGE:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_START_GLOBAL_DISCHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONTROL_DISCHARGE_WAIT_START_DISCHARGE );
    break;
  
  case UZRU10A_SIMPLE_CONTROL_DISCHARGE_WAIT_START_DISCHARGE:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_DISCHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_GLOBAL_DISCHARGE ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONTROL_DISCHARGE_DISCHARGE );
      ChargeControllerSetSettings ( &ZeroValue, &ZeroValue, &SimpleControlDischargeStage1MaxCurrent );
      ChargeControllerSetTelemetry ( &Stage1Umax, &Stage1Umin, &Stage1Imax, &Stage1Imin, &Stage1Time, &Stage1Volume );
      Stage1Reached = 1;
      SetRelayState ( WORK_LED, ON );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_CONTROL_DISCHARGE_DISCHARGE:
    if ( GetFaultState ( FAULT_RADIATOR_OVERHEAT ) ) {
      ModeStopOrPauseReason = OVERHEATING;
      PauseMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_STOP_DISCHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      StopMode ();
    }
    else if ( IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) {
      ModeStopOrPauseReason = FAULT;
      PauseMode ();
    }
    if ( ( !StatisticsNotInitialized ) && ( LocalUmax <= SimpleControlDischargeStage1MinVoltage ) ) {
      ModeStopOrPauseReason = OVER;
      StopMode ();
    }
    break;
    
  case UZRU10A_SIMPLE_CONTROL_DISCHARGE_PAUSE_DISCHARGE:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_GLOBAL_DISCHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONTROL_DISCHARGE_WAIT_PAUSE_DISCHARGE );
    break;
  
  case UZRU10A_SIMPLE_CONTROL_DISCHARGE_WAIT_PAUSE_DISCHARGE:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONTROL_DISCHARGE_PAUSED );
      SetRelayState ( WORK_LED, PULSE );
      LoadCurrentMenu ();
    }
    break;
    
  case UZRU10A_SIMPLE_CONTROL_DISCHARGE_PAUSED:
    if ( ( ( ModeStopOrPauseReason == FAULT ) || ( ModeStopOrPauseReason == OVERHEATING ) ) && ( !IsFaultsActionIs ( FAULT_ACTION_NEED_PAUSE_CHARGE ) ) ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONTROL_DISCHARGE_START_DISCHARGE );
    }
    break;
  
  case UZRU10A_STOP_SIMPLE_CONTROL_DISCHARGE:
    SetSubsystemState ( CHARGE_CONTROLLER, CHARGE_CONTROLLER_STOP_GLOBAL_DISCHARGE );
    SetSubsystemState ( UZRU10A, UZRU10A_WAIT_STOP_SIMPLE_CONTROL_DISCHARGE );
    LoadCurrentMenu ();
    break;
  
  case UZRU10A_WAIT_STOP_SIMPLE_CONTROL_DISCHARGE:
    if ( GetSubsystemState ( CHARGE_CONTROLLER ) == CHARGE_CONTROLLER_OFF ) {
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONTROL_DISCHARGE_STOPPED );
      SetRelayState ( WORK_LED, OFF );
      LoadCurrentMenu ();
    }
    break;
  
  case UZRU10A_SIMPLE_CONTROL_DISCHARGE_STOPPED:
    break;
  
  }
  LastUzru10aState = tmpUzru10aState;
}

void Uzru10aStopDischarge () {
  
}

void AnalyzeUzru10a () {
}

void ClearLocalStatistics () {
}

void InitUzru10a () {
  ClearData ();
  SetSubsystemState ( UZRU10A, UZRU10A_OFF );
}