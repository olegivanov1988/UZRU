#ifndef __SUBSYSTEMS_H__
#define __SUBSYSTEMS_H__

#include "stdint.h"

typedef enum {
  R4V = 0,            // < Это устройство - как бы супервизор всех устройств РЧВ
  ENGINE,              // < Двигатель. Устройство-контейнер для получения доступа к состоянию двигателя (вращается или нет)
  ANGLE_REG,          // < Регулятор угла соленоида.
  FREQ_REG,            // < Регулятор частоты двигателя.
  CURRENT_REG,        // < Регулятор тока соленоида.
  UDP_OPERATOR,        // < Оператор, управляющий РЧВ через Ethernet.
  UDP_ABONENT,        // < Абонент, принимающий пакеты UDP и отправляющий отчёты.
  CAN_OPERATOR,        // < Оператор, управляющий РЧВ через CAN.
  CAN_ABONENT,        // < Абонент, принимающий пакеты CAN и отправляющий ответы.  
  MANUAL_OPERATOR,    // < Оператор, управляющий РЧВ с клавиатуры
  DISPLAY,            // < Подсистема дисплея
  KEYBOARD,           // < Подсистема клавиатуры
  MAIN_OPERATOR,      // < Виртуальный главный оператор
  UDP_REPORTS,        // < Служба отчётов по протоколу UDP
  REFERENCES,         ///< Подсистема уставок
  MEASUREMENTS,         ///< Подсистема параметров
  CALIBRATIONS,       ///< Подсистема калибровочных коэффициентов
  EEPROM,             ///< Подсистема ПЗУ
  VOLTAGE_CONTROL,    ///< Подсистема контроля напряжения подзаряда
  FAULTS,             ///< Подсистема аварий
  CHARGE_CONTROLLER,
  CURRENT_REGULATOR,
  PAYLOAD_CONTROLLER,
  PRECHARGE_CONTROLLER,
  CONV_CONTACTOR_CONTROLLER,
  UZRU10A,
  
  NUM_OF_SUBSYSTEMS
} eSubsystemsID;

#ifndef __SUBSYSTEMS_C__
extern uint32_t SubsystemsStates[NUM_OF_SUBSYSTEMS];
#endif // __SUBSYSTEMS_C__

typedef struct {
  uint64_t Wants;            //< Флаги желаний подсистемы
} sSubsystemInfo;

void InitSubsystems ( void ); // Функция инициализации всех подсистем
void AnalyzeSubsystems ( void ); // Функция, вызывающая поочередно функции анализа каждой подсистемы, начиная с подчиненных, заканчивая главными
void DispatchSubsystems ( void );// Функция, вызывающая поочередно функции управления каждой подсистемы, начиная с главных, заканчивая подчиненными
void SetSubsystemState ( eSubsystemsID SubsystemID, uint32_t CurrentState ); // Функция устанавливает состояние подсистемы
void OnSubsystemStateChange ( uint32_t SubsystemID ); // Обработчик изменения состояния подсистемы (например, для отправки телеметрии)
uint32_t GetSubsystemState ( eSubsystemsID SubsystemID ); // Функция возвращает состояние подсистемы
void SetSubsystemWants ( eSubsystemsID SubsystemID, uint64_t WantsFlags ); // Функция устанавливает флаги целей подсистемы
void OnSubsystemWantsChange ( uint32_t SubsystemID ); // Обработчик изменения целей подсистемы (например, для отправки телеметрии)
uint64_t GetSubsystemWants ( eSubsystemsID SubsystemID ); // Функция возвращает флаги целей подсистемы
void DropSubsystemWants ( eSubsystemsID SubsystemID, uint64_t WantsFlags ); // Функция сбрасывает флаги целей подсистемы
uint32_t IsSubsystemWants ( eSubsystemsID SubsystemID, uint64_t WantsFlags ); // Функция возвращает утвердительный ответ, если все указанные флаги целей установлены
uint32_t IsSubsystemWantsOr ( eSubsystemsID SubsystemID, uint64_t WantsFlags ); // Функция возвращает утвердительный ответ, если хотя бы один из флагов целей установлен


#endif // __SUBSYSTEMS_H__
