#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "stdint.h"

#define NUM_OF_EEPROM_SECTORS 4
#define MAX_NUM_OF_OPERATIONS_TASKS 10

typedef enum {
  EEPROM_WORKS
} eEEPROMStates;

typedef enum {
  EEPROM_WANTS_NOTHING
} eEEPROMWants;

enum {
  CREATE,
  CLEAR_SECTOR = CREATE,     // Очистить страницу
  WRITE_NUM_OF_ELEMENTS,     // Записать размер массива
  WRITE_NEG_NUM_OF_ELEMENTS, // Записать инвертированный размер массива
  UPDATE,                    // Обновить данные в EEPROM в случае их несовпадения с данными в ОЗУ
  WRITE_CHECKSUM,            // Записать контрольную сумму
  WRITE_NCHECKSUM            // Записать инвертированную контрольную сумму
};

typedef struct {
  int32_t* RAMAddress;       // Каждому сектору соответствует некоторый массив ОЗУ
  uint32_t NumOfElements;    // В массиве ОЗУ конечное число элементов, которое нам нужно записать
} T_EEPROMDataRecord;

typedef struct {
  uint32_t DataRecordIndex;
  uint32_t ElementIndex;
  uint32_t NumOfElements;
  uint32_t NewCheckSum;
  uint32_t Operation;
} T_OperationTask;

void AssignWithEEPROM ( int32_t* Address, uint32_t Size, void (*InitProc)(void) );
void AnalyzeEEPROM ( void );
void UpdateInEEPROM ( int32_t* RAMAddress );
void InitEEPROM ( void );
void DispatchEEPROM ( void );
void DownloadFromEEPROM ( uint32_t* RAMAddress, uint32_t Size, uint32_t* Checksum );

#endif // __VOLTAGE_CONTROL_H__
