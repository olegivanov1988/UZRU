/** \file Subsystems.c
  \brief Данный программный модуль содержит определения API диспетчера подсистем.
*/

#include "main.h"      // Глобальные настройки проекта
#include "Subsystems.h"    // Прототипы функций данного модуля
#include "ManualOperator.h"  // Идентификаторы состояний подсистемы "Отслеживание команд оператора местного управления"
#include "Faults.h"
#include "TimeControl.h"  // Функции для работы с метками времени и отслеживания временных интервалов
#include "VoltageControl.h"
#include "Display.h"
#include "Keyboard.h"
#include "References.h"
#include "Measurements.h"
#include "Calibrations.h"
#include "EEPROM.h"
#include "Board.h"

//uint32_t SubsystemsTargetStates[NUM_OF_SUBSYSTEMS];
uint32_t SubsystemsStates[NUM_OF_SUBSYSTEMS];
//uint32_t SubsystemsSteps[NUM_OF_SUBSYSTEMS];
//uint32_t SubsystemsAbilities[NUM_OF_SUBSYSTEMS];

/*
void SetSubsystemTargetState ( uint32_t SubsystemIndex, uint32_t TargetState ) {
  SubsystemsTargetStates[SubsystemIndex] = TargetState;
  SendUDPResponse ( SET_VALUE, SUBSYSTEM_TARGET_STATE, SubsystemIndex, TargetState );
}

void SetSubsystemStep ( uint32_t SubsystemIndex, uint32_t Step ) {
  SubsystemsSteps[SubsystemIndex]=Step;
  SendUDPResponse ( SET_VALUE, SUBSYSTEM_STEP, SubsystemIndex, Step );
}

void SetSubsystemAbilities ( uint32_t SubsystemIndex, uint32_t AbilitiesFlags ) {
  SubsystemsAbilities[SubsystemIndex]=AbilitiesFlags;
  SendUDPResponse ( SET_VALUE, SUBSYSTEM_ABILITIES, SubsystemIndex, AbilitiesFlags );
}

void AddSubsystemAble ( uint32_t SubsystemIndex, uint32_t AbilitiesFlags ) {
  SubsystemsAbilities[SubsystemIndex] |= AbilitiesFlags;
  SendUDPResponse ( SET_VALUE, SUBSYSTEM_ABILITIES, SubsystemIndex, SubsystemsAbilities[SubsystemIndex] );
}

void ClearSubsystemAble ( uint32_t SubsystemIndex, uint32_t AbilitiesFlags ) {
  SubsystemsAbilities[SubsystemIndex] &= ~AbilitiesFlags;
  SendUDPResponse ( SET_VALUE, SUBSYSTEM_ABILITIES, SubsystemIndex, SubsystemsAbilities[SubsystemIndex] );
} */

sSubsystemInfo Subsystems[NUM_OF_SUBSYSTEMS];

void ExecuteSubsystemsPrograms ( void );

/** \fn InitSubsystems (void)
  \brief Инициализирует состояния всех подсистем в исходное состояние. Вызывается один раз, как правило после включения питания, перед запуском цикла управления подсистемами.
*/
void InitSubsystems ( void ) {

  InitEEPROM1 ();            ///< Подсистема ПЗУ
  InitRelays ();
  //InitVariables ();
  
  //InitCurrentReg ();        ///< Регулятор тока соленоида.
  InitManualOperator ();      ///< Оператор, управляющий РЧВ с клавиатуры
  InitDisplay ();             ///< Подсистема дисплея
  InitKeyboard ();            ///< Подсистема клавиатуры
  //InitMeasurements ();        ///< Подсистема параметров инициализируется в самом начале программы, перед инициализацией периферии
  // InitCurrentRegulator ();
  // InitCalibrations ();        ///< Подсистема калибровочных коэффициентов
  // InitCoefficients ();     ///< Подсистема коэффициентов регулирования
  // InitReferences ();       ///< Подсистема уставок
  InitChargeController ();
  InitPayloadController ();
  InitUzru10a ();
  InitMenu ();
  InitPrechargeController ();
  InitConvContactorController ();
  InitFaults ();            ///< Подсистема аварий
}

/** \fn AnalyzeSubsystems (void)
  \brief Анализирует состояния всех подсистем по значениям измеренных параметров, входным сигналам и состояниям других подсистем. Вызывается периодически.
*/
void AnalyzeSubsystems ( void ) {
  AnalyzeKeyboard ();       ///< Подсистема клавиатуры
  //AnalyzeDisplay ();        ///< Подсистема дисплея
  //AnalyzeReferences ();     ///< Подсистема уставок
  //AnalyzeMeasurements ();     ///< Подсистема параметров
  //AnalyzeCalibrations ();   ///< Подсистема калибровочных коэффициентов
  AnalyzeEEPROM ();         ///< Подсистема ПЗУ
  //AnalyzeUDPReports ();     ///< Служба отчётов по протоколу UDP
  AnalyzeRelays ();
  AnalyzeMenu ();
  // AnalyzeCurrentRegulator ();
  AnalyzeChargeController ();
  //AnalyzePayloadController ();
  AnalyzePrechargeController ();
  AnalyzePayloadController ();
  AnalyzeConvContactorController ();
  AnalyzeUzru10a ();
  
  AnalyzeFaults ();         ///< Подсистема аварий  
}

/** \fn AnalyzeSubsystems (void)
  \brief Выполняет один такт управления всех подсистем по значениям измеренных параметров, входным сигналам и состояниям других подсистем. Вызывается периодически.
*/
void DispatchSubsystems ( void ) {
  //DispatchBoard ();
  DispatchRelays ();
  DispatchFaults ();         ///< Подсистема аварий
  //DispatchUDPOperator ();    ///< Оператор, управляющий РЧВ через Ethernet
  //DispatchMainOperator ();   ///< Виртуальный главный оператор
  DispatchKeyboard ();       ///< Подсистема клавиатуры
  DispatchDisplay ();        ///< Подсистема дисплея
  //DispatchReferences ();     ///< Подсистема уставок
  //DispatchMeasurements ();     ///< Подсистема параметров
  //DispatchCalibrations ();   ///< Подсистема калибровочных коэффициентов
  DispatchEEPROM ();         ///< Подсистема ПЗУ
  //DispatchUDPReports ();     ///< Служба отчётов по протоколу UDP
  DispatchMenu ();
  DispatchChargeController ();
  //DispatchCurrentRegulator ();
  DispatchPayloadController ();
  DispatchPrechargeController ();
  DispatchConvContactorController ();
  DispatchUzru10a ();
}

void SetSubsystemState ( eSubsystemsID SubsystemID, uint32_t NewState ) {    // Функция установки состояния подсистемы
  if ( ( SubsystemID < NUM_OF_SUBSYSTEMS ) && ( SubsystemsStates[SubsystemID] != NewState ) ) {
    SubsystemsStates[SubsystemID] = NewState;
  }
}

void DropSubsystemWants ( eSubsystemsID SubsystemID, uint64_t WantsFlags ) {
  if ( ( SubsystemID < NUM_OF_SUBSYSTEMS ) && ( Subsystems[SubsystemID].Wants & WantsFlags ) ) {
    Subsystems[SubsystemID].Wants &= ~(WantsFlags);
  }
}

uint64_t GetSubsystemWants ( eSubsystemsID SubsystemID ) {
  return Subsystems[SubsystemID].Wants;
}

uint32_t IsSubsystemWants ( eSubsystemsID SubsystemID, uint64_t WantsFlags ) {
  if ( ( Subsystems[SubsystemID].Wants & WantsFlags ) == WantsFlags ) { return 1; }
  else { return 0; }
}

uint32_t IsSubsystemWantsOr ( eSubsystemsID SubsystemID, uint64_t WantsFlags ) {
  if ( Subsystems[SubsystemID].Wants & WantsFlags ) { return 1; }
  else { return 0; }
}

uint32_t GetSubsystemState ( eSubsystemsID SubsystemID ) {
  return SubsystemsStates[SubsystemID];                // Возвращаем значение поля "Текущее состояние устройства" для устройства с указанным ID
}
