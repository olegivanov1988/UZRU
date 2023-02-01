#ifndef __FAULTS_H__
#define __FAULTS_H__

#include "stdint.h"
#include "Display.h"

typedef enum {
  FAULT_OFF = 0,
  FAULT_ON = 1
} FaultState_Typedef;

typedef enum {
  FAULT_NO_BATTERY,           // Батарея не подключена
  FAULT_BATTERY_SHORT_CIRQUIT,// КЗ на выходе УЗРУ. Пока авария отключена, т.к. при этой аварии напряжение АКБ провалится ниже допустимого уровня, и сработает защита. Но вообще для диагностики неплохо
  PAYLOAD_LOCKING_FAULT,      // Неисправность блокировки нагрузки
  FAULT_PAYLOAD_IS_LOCKED,    // Нагрузка занята
  PAYLOAD_FREE_FAULT,         // Неисправность освобождения нагрузки
  FAULT_PAYLOAD_SHORT_CIRQUIT,// КЗ нагрузки. Пока авария отключена, т.к. при этой аварии напряжение АКБ провалится ниже допустимого уровня, и сработает защита. Но вообще для диагностики неплохо
  FAULT_LOW_NETWORK_VOLTAGE,  // Низкое напряжение сети
  FAULT_HIGH_NETWORK_VOLTAGE, // Напряжение сети больше нормы
  FAULT_LOW_CONVERTER_VOLTAGE,// Низкое напряжение понижающего преобразователя
  FAULT_HIGH_CONVERTER_VOLTAGE, // Напряжение фильтра больше нормы
  FAULT_LOW_BATTERY_VOLTAGE,  // Напряжение батареи ниже нормы
  FAULT_HIGH_BATTERY_VOLTAGE, // Напряжение батареи больше нормы
  FAULT_NO_CHARGE_CURRENT,    // Нет тока заряда
  FAULT_NO_DISCHARGE_CURRENT, // Нет тока разряда
  FAULT_PRECHARGE_FAULT,      // Нет предзаряда
  FAULT_MODE_CANCEL,          // Сброс режима заряда
  CHARGE_REGULATION_FAULT,    // Неисправность регулирования параметров заряда
  DISCHARGE_REGULATION_FAULT, // Неисправность регулирования параметров разряда
  FAULT_RADIATOR_OVERHEAT,
  FAULT_TEMP_SENSOR_DISCONNECT,
  
  NUM_OF_FAULTS
} FaultIndex_Typedef;

typedef enum {
  FAULT_ACTION_NEED_STOP_CHARGE = 1,            // Необходимо завершить заряд
  FAULT_ACTION_NEED_PAUSE_CHARGE = 2,           // Необходимо приостановить заряд
  FAULT_ACTION_NEED_PAUSE_DISCHARGE = 4,        // Необходимо приостановить разряд
  FAULT_ACTION_NEED_STOP_DISCHARGE = 8,         // Необходимо завершить разряда
  FAULT_ACTION_NEED_OFF_CONV_CONTACTOR = 16,    // Необходимо отключить контактор преобразователя
  FAULT_ACTION_NEED_OFF_PAYLOAD_CONTACTOR = 32, // Необходимо отключить контактор нагрузки
  FAULT_ACTION_NEED_OFF_PRECHARGE_RELAY = 64,   // Необходимо отключить реле предзаряда
} FaultActions_Typedef;

#ifndef __FAULTS_C__
extern uint32_t FaultsStates[NUM_OF_FAULTS];    // Объявление массива аварий ставим сюда, т.к. у нас вместо функций иногда используются макросы
#endif // __FAULTS_C__

void SetFault ( FaultIndex_Typedef FaultIndex );          // Установить аварию
uint32_t GetFaultState ( FaultIndex_Typedef FaultIndex ); // Проверить, установлена ли авария
uint32_t GetFaultsActions ( void );                       // Получить флаги действий, обусловленных текущими авариями
void ClearFault ( FaultIndex_Typedef FaultIndex );        // Сбросить аварию
void ClearFaults ( void );

void InitFaults ( void );     // Инициализировать подсистему аварий
void DispatchFaults ( void ); // Выполнить цикл управления подсистемой аварий
void AnalyzeFaults ( void );  // Выполнить цикл анализа подсистемы аварий

#endif // __FAULTS_H__
