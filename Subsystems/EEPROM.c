#include "main.h"
#include "Board.h"
#include "EEPROM.h"
#include "DriverMDR32_EEPROM.h"
#include "MDR1986VE1T.h"
#include "TimeControl.h"
#include "Calibrations.h"
#include "References.h"

#define PREPARE_READ_EEPROM 1;
#define READ_EEPROM 2;
#define PREPARE_WRITE_EEPROM 3;
#define WRITE_EEPROM 4;

#if defined ( __ICCARM__ )
   #define __RAMFUNC      __ramfunc
#elif defined (__CMCARM__)
  #define __RAMFUNC       __ramfunc
#elif defined ( __CC_ARM )
  #define __RAMFUNC
#elif defined ( __GNUC__)
 #define __RAMFUNC
#endif

T_EEPROMDataRecord EEPROMDataTable[ NUM_OF_EEPROM_SECTORS ];
uint64_t LastOperationTimeStamp;
uint32_t TasksHead = 0;
uint32_t TasksTail = 0;
T_OperationTask Tasks[MAX_NUM_OF_OPERATIONS_TASKS];

uint32_t EEPROMSectorIndex = 0; // Число массивов в EEPROM

void DoNothing ( void );

void DoNothing ( void ) {
}

extern int32_t* pMinFaultBatteryVoltage;
extern int32_t* pMaxFaultBatteryVoltage;

void InitEEPROM1 ( void ) { // Функция инициализации EEPROM
  EEPROMSectorIndex = 0;
  AssignWithEEPROM ( Calibrations, NUM_OF_CALIBRATIONS, SetDefaultCalibrations );
  AssignWithEEPROM ( References, NUM_OF_REFERENCES, SetDefaultReferences );
  pMinFaultBatteryVoltage = &(References[MIN_24V_BATTERY_VOLTAGE]);
  pMaxFaultBatteryVoltage = &(References[MAX_24V_BATTERY_VOLTAGE]);
  /*
  SetDefaultCalibrations ();
  SetDefaultReferences ();
  //*/
}

void AddTask ( int32_t* RAMAddress, uint32_t Size, uint32_t Operation );
void AddEEPROMData ( uint32_t* RAMAddress, uint32_t ROMAddress, uint32_t Size, uint32_t MaxSize, uint32_t Checksum );
uint32_t CalcCheckSum ( uint32_t* Address, uint32_t Size );

// Функция добавляет запись в таблицу структур EEPROM, проверяет наличие актуальной структуры в EEPROM, и, если не находит,
// инициализирует массив значениями по умолчанию и записывает в EEPROM.
void AssignWithEEPROM ( int32_t* RAMAddress, uint32_t NumOfElements, void (*InitProc)(void) ) {
  uint32_t Checksum;   // Буферная переменная для хранения контрольной суммы
  uint32_t tmpValue;
  uint32_t DownloadOk = 0; // Флаг успешной выгрузки структуры из EEPROM
  if ( EEPROMSectorIndex < NUM_OF_EEPROM_SECTORS ) {    // Если есть свободное место в журнале структур, спроецированных в EEPROM, то
    ////EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "In AssignWith", 20 ); // Сообщаем "на базу", что мы зашли в процедуру AssignWithEEPROM
    EEPROMDataTable[EEPROMSectorIndex].RAMAddress = RAMAddress;
    EEPROMDataTable[EEPROMSectorIndex].NumOfElements = NumOfElements;
    SetEEPROMAddress ( EEPROMSectorIndex * 4 );
    Checksum = GetEEPROMValue ();
    ////EasyTCPIPSendViaUDP ( Dest1IP, 0x0608, 0x0608, IntToStr ( Checksum ), 20 );
    if ( Checksum == NumOfElements ) { // Если запись в EEPROM равна размеру массива, то...
      SetEEPROMAddress ( EEPROMSectorIndex * 4 + 16 );
      Checksum = GetEEPROMValue ();
      ////EasyTCPIPSendViaUDP ( Dest1IP, 0x0608, 0x0608, IntToStr ( Checksum ), 20 );
      if ( Checksum == ~NumOfElements ) { // Читаем два двойных слова из EEPROM. Там должны быть размер этой структуры и инвертированный размер
        //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "Size EEPROM Ok", 20 );
        for ( Checksum = 0; Checksum < NumOfElements; Checksum++ ) {
          SetEEPROMAddress ( EEPROMSectorIndex * 4 + 32 + Checksum * 16 );
          RAMAddress[Checksum] = GetEEPROMValue ();
          ////EasyTCPIPSendViaUDP ( Dest1IP, 0x0608, 0x0608, IntToStr ( RAMAddress[Checksum] ), 20 );
        }
        Checksum = CalcCheckSum ( RAMAddress, NumOfElements );                               // Вычислим контрольную сумму выгруженного из ПЗУ массива.
        SetEEPROMAddress ( EEPROMSectorIndex * 4 + 32 + NumOfElements * 16 );
        tmpValue = GetEEPROMValue ();
        ////EasyTCPIPSendViaUDP ( Dest1IP, 0x0608, 0x0608, IntToStr ( tmpValue ), 20 );
        if ( tmpValue == Checksum ) {
          SetEEPROMAddress ( EEPROMSectorIndex * 4 + 48 + NumOfElements * 16 );
          tmpValue = GetEEPROMValue ();
          ////EasyTCPIPSendViaUDP ( Dest1IP, 0x0608, 0x0608, IntToStr ( tmpValue ), 20 );
          if ( tmpValue == ~Checksum ) { // Теперь вычитываем из EEPROM ещё два значения.
            DownloadOk = 1;
            //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "Load EEPROM Ok", 20 );
          }
          else { 
            //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "Load ~Checksum Fault", 20 );
          }
        }
        else {
          //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "Load Checksum Fault", 20 );
        }
      } else {
        //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "Load ~Size Fault", 20 );
      }
    }
    else {
      //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "Load Size Fault", 20 );
    }
    EEPROMSectorIndex++;
    if ( DownloadOk == 0 ) {
      InitProc ();
      UpdateInEEPROM ( RAMAddress );
      //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "Load EEPROM Fault", 20 );
    }
  }
}

// Функция вычисляет контрольную сумму по массиву ( обычная арифметическая сумма элементов )
uint32_t CalcCheckSum ( uint32_t *pAddress, uint32_t NumOfElements )
{
  uint32_t Index;
  uint32_t CheckSum = 0;
  for ( Index = 0; Index < NumOfElements; Index++ ) {
    CheckSum += pAddress[Index];
  }
  return CheckSum;
}

// Функция ставит задачу в кольцевой буфер задач записи в EEPROM
void UpdateInEEPROM ( int32_t* RAMAddress ) {
  int32_t i;
  //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "UpdateInEEPROM ()", 20 );
  if ( ( ( TasksTail + 1 ) % MAX_NUM_OF_OPERATIONS_TASKS ) != TasksHead ) { // Если кольцевой буфер задач не полон, то...
    for ( i = 0; i < EEPROMSectorIndex; i++ ) {   // Сначала находим нужную запись, чтобы понять, какой массив в ПЗУ и в ОЗУ нас интересует
      if ( ( (uint32_t)EEPROMDataTable[i].RAMAddress == (uint32_t)RAMAddress ) ) {
           //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "I set a task...", 20 );
           Tasks[TasksTail].DataRecordIndex = i; // Указываем в записи задачи номер массива в EEPROM
           Tasks[TasksTail].ElementIndex = 0; // Указываем в записи задачи индекс элемента в массиве
           Tasks[TasksTail].NumOfElements = EEPROMDataTable[i].NumOfElements; // Указываем в записи задачи число элементов массива, подлежащее изменению
           Tasks[TasksTail].NewCheckSum = CalcCheckSum ( RAMAddress, EEPROMDataTable[i].NumOfElements );
           Tasks[TasksTail].Operation = CLEAR_SECTOR;     // Указываем в записи задачи тип операции
           TasksTail = ( TasksTail + 1 ) % MAX_NUM_OF_OPERATIONS_TASKS; // Отправляем задачу на выполнение
           break;
      } else {
          //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "Have no this array...", 20 );
      }
    }
  } else {
    //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "Have no task space", 20 );
  }
}

uint32_t NewTask = 0;
uint32_t tmpChecksum = 0;
uint32_t tmpValue = 0;

T_OperationTask* pTaskStruct;
T_EEPROMDataRecord* pDataStruct;

void ExecuteTaskStep () {
  uint32_t tmpEEPROMAddress;
  int32_t tmpValue;
  if ( TasksHead != TasksTail ) { // Если в кольцевом буфере задач есть невыполненные задачи, то...
    tmpEEPROMAddress = Tasks[TasksHead].DataRecordIndex;
    switch ( Tasks[TasksHead].Operation ) {  // В зависимости от кода операции выполняем действия...
      case CLEAR_SECTOR:
        //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "Clear Sector", 20 );
        ////EasyTCPIPSendViaUDP ( Dest1IP, 0x0608, 0x0608, IntToStr (Tasks[TasksHead].DataRecordIndex), 20 );
        ClearEEPROMSector ( Tasks[TasksHead].DataRecordIndex );
        Tasks[TasksHead].Operation = WRITE_NUM_OF_ELEMENTS;
        break;
      case WRITE_NUM_OF_ELEMENTS:                            // Если операция - запись размера массива в EEPROM, то выполняем соответствующие действия:
        SetEEPROMAddress ( tmpEEPROMAddress * 4 );
        tmpValue = GetEEPROMValue ();
        //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "Read Num Elements", 20 );
        ////EasyTCPIPSendViaUDP ( Dest1IP, 0x0608, 0x0608, IntToStr(tmpValue), 20 );
        if ( tmpValue != EEPROMDataTable[Tasks[TasksHead].DataRecordIndex].NumOfElements ) {
          //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "Write Num Elements", 20 );
          ////EasyTCPIPSendViaUDP ( Dest1IP, 0x0608, 0x0608, IntToStr (EEPROMDataTable[Tasks[TasksHead].DataRecordIndex].NumOfElements), 20 );
          SetEEPROMAddress ( tmpEEPROMAddress * 4 );
          WriteEEPROM ( EEPROMDataTable[Tasks[TasksHead].DataRecordIndex].NumOfElements );
          Tasks[TasksHead].Operation = WRITE_NEG_NUM_OF_ELEMENTS;
          break;
        }
        Tasks[TasksHead].Operation = WRITE_NEG_NUM_OF_ELEMENTS;
      case WRITE_NEG_NUM_OF_ELEMENTS:
        SetEEPROMAddress ( tmpEEPROMAddress * 4 + 16 );
        tmpValue = GetEEPROMValue ();
        //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "Read ~Num Elements", 20 );
        ////EasyTCPIPSendViaUDP ( Dest1IP, 0x0608, 0x0608, IntToStr(tmpValue), 20 );
        if ( tmpValue != ~( EEPROMDataTable[Tasks[TasksHead].DataRecordIndex].NumOfElements ) ) {
          //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "Write ~Num Elements", 20 );
          ////EasyTCPIPSendViaUDP ( Dest1IP, 0x0608, 0x0608, IntToStr (~(EEPROMDataTable[Tasks[TasksHead].DataRecordIndex].NumOfElements)), 20 );
          SetEEPROMAddress ( tmpEEPROMAddress * 4 + 16 );
          WriteEEPROM ( ~ ( EEPROMDataTable[Tasks[TasksHead].DataRecordIndex].NumOfElements ) );
          Tasks[TasksHead].Operation = UPDATE;
          break;
        }
        Tasks[TasksHead].Operation = UPDATE;
      case UPDATE:
        SetEEPROMAddress ( tmpEEPROMAddress * 4 + 32 + Tasks[TasksHead].ElementIndex * 16 );
        tmpValue = GetEEPROMValue ();
        //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "Read Element", 20 );
        ////EasyTCPIPSendViaUDP ( Dest1IP, 0x0608, 0x0608, IntToStr(tmpValue), 20 );
        if ( tmpValue != EEPROMDataTable[Tasks[TasksHead].DataRecordIndex].RAMAddress[Tasks[TasksHead].ElementIndex] ) {
          //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "Write Element", 20 );
          ////EasyTCPIPSendViaUDP ( Dest1IP, 0x0608, 0x0608, IntToStr ( EEPROMDataTable[Tasks[TasksHead].DataRecordIndex].RAMAddress[Tasks[TasksHead].ElementIndex] ), 20 );
          SetEEPROMAddress ( tmpEEPROMAddress * 4 + 32 + Tasks[TasksHead].ElementIndex * 16 );
          WriteEEPROM ( EEPROMDataTable[Tasks[TasksHead].DataRecordIndex].RAMAddress[Tasks[TasksHead].ElementIndex] );
          Tasks[TasksHead].ElementIndex++;
          Tasks[TasksHead].NumOfElements--;
          if ( !Tasks[TasksHead].NumOfElements ) { Tasks[TasksHead].Operation = WRITE_CHECKSUM; }
          break;
        }
        Tasks[TasksHead].ElementIndex++;
        Tasks[TasksHead].NumOfElements--;
        if ( !Tasks[TasksHead].NumOfElements ) { Tasks[TasksHead].Operation = WRITE_CHECKSUM; }
        break;
      case WRITE_CHECKSUM:
        SetEEPROMAddress ( 4 * Tasks[TasksHead].DataRecordIndex + 32 + EEPROMDataTable[Tasks[TasksHead].DataRecordIndex].NumOfElements * 16 );
        tmpValue = GetEEPROMValue ();
        //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "Read Checksum", 20 );
        ////EasyTCPIPSendViaUDP ( Dest1IP, 0x0608, 0x0608, IntToStr(tmpValue), 20 );
        if ( GetEEPROMValue () != Tasks[TasksHead].NewCheckSum ) {
          //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "Write Checksum", 20 );
          ////EasyTCPIPSendViaUDP ( Dest1IP, 0x0608, 0x0608, IntToStr ( Tasks[TasksHead].NewCheckSum ), 20 );
          SetEEPROMAddress ( 4 * Tasks[TasksHead].DataRecordIndex + 32 + EEPROMDataTable[Tasks[TasksHead].DataRecordIndex].NumOfElements * 16 );
          WriteEEPROM ( Tasks[TasksHead].NewCheckSum );
          Tasks[TasksHead].Operation = WRITE_NCHECKSUM;
          break;
        }
        Tasks[TasksHead].Operation = WRITE_NCHECKSUM;
      case WRITE_NCHECKSUM:
        SetEEPROMAddress ( 4 * Tasks[TasksHead].DataRecordIndex + 48 + EEPROMDataTable[Tasks[TasksHead].DataRecordIndex].NumOfElements * 16 );
        tmpValue = GetEEPROMValue ();
        //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "Read ~Checksum", 20 );
        ////EasyTCPIPSendViaUDP ( Dest1IP, 0x0608, 0x0608, IntToStr(tmpValue), 20 );
        if ( GetEEPROMValue () != ~(Tasks[TasksHead].NewCheckSum) ) {
          //EasyTCPIPSendViaUDP ( DestIP, 0x0608, 0x0608, "Write ~Checksum", 20 );
          ////EasyTCPIPSendViaUDP ( Dest1IP, 0x0608, 0x0608, IntToStr ( ~(Tasks[TasksHead].NewCheckSum) ), 20 );
          SetEEPROMAddress ( 4 * Tasks[TasksHead].DataRecordIndex + 48 + EEPROMDataTable[Tasks[TasksHead].DataRecordIndex].NumOfElements * 16 );
          WriteEEPROM ( ~(Tasks[TasksHead].NewCheckSum) );
          TasksHead = ( TasksHead + 1 ) % MAX_NUM_OF_OPERATIONS_TASKS;
          break;
        }
        TasksHead = ( TasksHead + 1 ) % MAX_NUM_OF_OPERATIONS_TASKS;
    }
  }
}

void AnalyzeEEPROM ( void ) {
}

void DispatchEEPROM ( void ) {
  ExecuteTaskStep ();
}
