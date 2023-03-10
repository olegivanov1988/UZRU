#include "MDR1986VE1T.h"
#include "DriverMDR32_EEPROM.h"
#include "TimeControl.h"

uint64_t EepromTimeStamp;

void SetEEPROMAddress ( uint32_t NewAddress ) {
  MDR_EEPROM->ADR = NewAddress;
}

uint32_t GetEEPROMAddress ( void ) {
  return MDR_EEPROM->ADR;
}

void ClearEEPROMSector ( uint32_t SectorIndex ) {
  __disable_irq ();
  MDR_EEPROM->KEY = 0x8AAA5551;
  MDR_EEPROM->CMD = 
  ( 3 << 3 ) |
  ( 1 << 9 ) |
  ( 1 << 0 );   // CON - Переключение контроллера EEPROM на регистровое управление ( 0 - управление от ядра, рабочий режим, 1 - управление от регистров, режим программирвоания )
  MDR_EEPROM->ADR = SectorIndex << 2;
  MDR_EEPROM->CMD |=
   ( 1 << 10 ) | // ERASE - Стереть строку с адресом ADR[16:9] ( 0 - нет стирания, 1 - стирание )
   ( 1 << 6 );   // XE - Выдача адреса ADR[16:9] ( 0 - не разрешено, 1 - разрешено )
  GetTimeStamp ( &EepromTimeStamp );
  while ( !IsIntervalOverFrom ( 96*5, EepromTimeStamp ) );
  MDR_EEPROM->CMD |=
   ( 1 << 13 ) ; // NVSTR - Операции записи или стирания ( 0 - при чтении, 1 - при записи или стирании )  
  GetTimeStamp ( &EepromTimeStamp );
  while ( !IsIntervalOverFrom ( 96*40000, EepromTimeStamp ) );
  MDR_EEPROM->CMD &= ~(
   ( 1 << 10 ) ); // ERASE - Стереть строку с адресом ADR[16:9] ( 0 - нет стирания, 1 - стирание )
  GetTimeStamp ( &EepromTimeStamp );
  while ( !IsIntervalOverFrom ( 96*5, EepromTimeStamp ) );
    MDR_EEPROM->CMD &= ~(
    ( 1 << 13 ) | // NVSTR - Операции записи или стирания ( 0 - при чтении, 1 - при записи или стирании )  
    ( 1 << 6 ) ); // XE - Выдача адреса ADR[16:9] ( 0 - не разрешено, 1 - разрешено )
  GetTimeStamp ( &EepromTimeStamp );
  while ( !IsIntervalOverFrom ( 96, EepromTimeStamp ) );
  MDR_EEPROM->CMD &= ~ (
  ( 1 << 9 ) |   // IFREN - Работа с блоком информации ( 0 - основная память, 1 - информационный блок )
  ( 1 << 0 ) );  // CON - Переключение контроллера EEPROM на регистровое управление ( 0 - управление от ядра, рабочий режим, 1 - управление от регистров, режим программирвоания )
  MDR_EEPROM->KEY = 0;
  __enable_irq ();
}

uint32_t GetEEPROMValue ( void ) {
  uint32_t tmpROMAddress;
  __disable_irq ();
  MDR_EEPROM->KEY = 0x8AAA5551;
  MDR_EEPROM->CMD |=
    ( 1 << 0 );   // CON - Переключение контроллера EEPROM на регистровое управление ( 0 - управление от ядра, рабочий режим, 1 - управление от регистров, режим программирвоания )
  MDR_EEPROM->CMD &= ~(
    ( 1 << 9 ) |  // IFREN - Работа с блоком информации ( 0 - основная память, 1 - информационный блок )
    ( 1 << 8 ) |  // SE - Усилитель считывания ( 0 - не включен, 1 - включен )
    ( 1 << 7 ) |  // YE - Выдача адреса ADR[8:2] ( 0 - не разрешено, 1 - разрешено )
    ( 1 << 6 ) ); // XE - Выдача адреса ADR[16:9] ( 0 - не разрешено, 1 - разрешено )  
  MDR_EEPROM->CMD |=
    ( 1 << 9 );   // IFREN - Работа с блоком информации ( 0 - основная память, 1 - информационный блок )
  MDR_EEPROM->CMD |= (
    ( 1 << 8 ) |  // SE - Усилитель считывания ( 0 - не включен, 1 - включен )
    ( 1 << 7 ) |  // YE - Выдача адреса ADR[8:2] ( 0 - не разрешено, 1 - разрешено )
    ( 1 << 6 ) );  // XE - Выдача адреса ADR[16:9] ( 0 - не разрешено, 1 - разрешено )
  tmpROMAddress *= ( tmpROMAddress + 5 )*(tmpROMAddress + 3); // Задержка 20 нс
  tmpROMAddress = MDR_EEPROM->DO;
  MDR_EEPROM->CMD &= ~(
    ( 1 << 9 ) |   // IFREN - Работа с блоком информации ( 0 - основная память, 1 - информационный блок )
    ( 1 << 8 ) |  // SE - Усилитель считывания ( 0 - не включен, 1 - включен )
    ( 1 << 7 ) |  // YE - Выдача адреса ADR[8:2] ( 0 - не разрешено, 1 - разрешено )
    ( 1 << 6 ) |  // XE - Выдача адреса ADR[16:9] ( 0 - не разрешено, 1 - разрешено )  
    ( 1 << 0 ) );  // CON - Переключение контроллера EEPROM на регистровое управление ( 0 - управление от ядра, рабочий режим, 1 - управление от регистров, режим программирвоания )
  MDR_EEPROM->KEY = 0;
  __enable_irq ();
  return tmpROMAddress;
}

uint32_t ReadEEPROM ( void ) {
  GetEEPROMValue ();
  MDR_EEPROM->ADR += 4;
  return MDR_EEPROM->DO;
}

uint32_t WriteEEPROM ( uint32_t Value ) {
    uint32_t tmpROMAddress = MDR_EEPROM->ADR;
    __disable_irq ();
    MDR_EEPROM->KEY = 0x8AAA5551;
    MDR_EEPROM->CMD = ( 3 << 3 ) |  // Delay[2:0] - Задержка памяти программ при чтении в циклах (в рабочем режиме) ( от 0 до 7 )
  ( 1 << 0 );   // CON - Переключение контроллера EEPROM на регистровое управление ( 0 - управление от ядра, рабочий режим, 1 - управление от регистров, режим программирвоания )
    MDR_EEPROM->CMD = 
    ( 1 << 9 ) |   // IFREN - Работа с блоком информации ( 0 - основная память, 1 - информационный блок )
    ( 3 << 3 ) |  // Delay[2:0] - Задержка памяти программ при чтении в циклах (в рабочем режиме) ( от 0 до 7 )
    ( 1 << 0 );   // CON - Переключение контроллера EEPROM на регистровое управление ( 0 - управление от ядра, рабочий режим, 1 - управление от регистров, режим программирвоания )
    MDR_EEPROM->ADR = tmpROMAddress;
    MDR_EEPROM->DI = Value;
    MDR_EEPROM->CMD = 
    ( 1 << 12 ) |  // PROG - Записать данные по ADR[16:2] из регистра EEPROM_DI ( 0 - нет записи, 1 - есть запись )
    ( 1 << 9 )  |  // IFREN - Работа с блоком информации ( 0 - основная память, 1 - информационный блок )
    ( 1 << 6 )  | // XE - Выдача адреса ADR[16:9] ( 0 - не разрешено, 1 - разрешено )
  ( 3 << 3 ) |  // Delay[2:0] - Задержка памяти программ при чтении в циклах (в рабочем режиме) ( от 0 до 7 )
    ( 1 << 0 );   // CON - Переключение контроллера EEPROM на регистровое управление ( 0 - управление от ядра, рабочий режим, 1 - управление от регистров, режим программирвоания )
    GetTimeStamp ( &EepromTimeStamp );
    while ( !IsIntervalOverFrom ( 96*5, EepromTimeStamp ) );
    MDR_EEPROM->CMD =
    ( 1 << 13 ) | // NVSTR - Операции записи или стирания ( 0 - при чтении, 1 - при записи или стирании )
    ( 1 << 12 ) |  // PROG - Записать данные по ADR[16:2] из регистра EEPROM_DI ( 0 - нет записи, 1 - есть запись )
    ( 1 << 9 )  |  // IFREN - Работа с блоком информации ( 0 - основная память, 1 - информационный блок )
    ( 1 << 6 )  |   // XE - Выдача адреса ADR[16:9] ( 0 - не разрешено, 1 - разрешено )
    ( 3 << 3 ) |  // Delay[2:0] - Задержка памяти программ при чтении в циклах (в рабочем режиме) ( от 0 до 7 )
    ( 1 << 0 );   // CON - Переключение контроллера EEPROM на регистровое управление ( 0 - управление от ядра, рабочий режим, 1 - управление от регистров, режим программирвоания )  
    GetTimeStamp ( &EepromTimeStamp );
    while ( !IsIntervalOverFrom ( 96*10, EepromTimeStamp ) );
    MDR_EEPROM->CMD =
    ( 1 << 13 ) | // NVSTR - Операции записи или стирания ( 0 - при чтении, 1 - при записи или стирании )
    ( 1 << 12 ) |  // PROG - Записать данные по ADR[16:2] из регистра EEPROM_DI ( 0 - нет записи, 1 - есть запись )
    ( 1 << 9 )  |  // IFREN - Работа с блоком информации ( 0 - основная память, 1 - информационный блок )
    ( 1 << 7 )  | // YE - Выдача адреса ADR[8:2] ( 0 - не разрешено, 1 - разрешено )  
    ( 1 << 6 )  | // XE - Выдача адреса ADR[16:9] ( 0 - не разрешено, 1 - разрешено )  
    ( 3 << 3 ) |  // Delay[2:0] - Задержка памяти программ при чтении в циклах (в рабочем режиме) ( от 0 до 7 )
    ( 1 << 0 );   // CON - Переключение контроллера EEPROM на регистровое управление ( 0 - управление от ядра, рабочий режим, 1 - управление от регистров, режим программирвоания )
    GetTimeStamp ( &EepromTimeStamp );
    while ( !IsIntervalOverFrom ( 96*40, EepromTimeStamp ) );
    MDR_EEPROM->CMD =
    ( 1 << 13 ) | // NVSTR - Операции записи или стирания ( 0 - при чтении, 1 - при записи или стирании )
    ( 1 << 12 ) |  // PROG - Записать данные по ADR[16:2] из регистра EEPROM_DI ( 0 - нет записи, 1 - есть запись )
    ( 1 << 9 )  |  // IFREN - Работа с блоком информации ( 0 - основная память, 1 - информационный блок )
    ( 1 << 6 )  | // XE - Выдача адреса ADR[16:9] ( 0 - не разрешено, 1 - разрешено )
    ( 3 << 3 ) |  // Delay[2:0] - Задержка памяти программ при чтении в циклах (в рабочем режиме) ( от 0 до 7 )
    ( 1 << 0 );   // CON - Переключение контроллера EEPROM на регистровое управление ( 0 - управление от ядра, рабочий режим, 1 - управление от регистров, режим программирвоания )
    tmpROMAddress += 4;
    MDR_EEPROM->CMD =
    ( 1 << 13 ) | // NVSTR - Операции записи или стирания ( 0 - при чтении, 1 - при записи или стирании )
    ( 1 << 9 )  |  // IFREN - Работа с блоком информации ( 0 - основная память, 1 - информационный блок )
    ( 1 << 6 ) | // XE - Выдача адреса ADR[16:9] ( 0 - не разрешено, 1 - разрешено )  
    ( 3 << 3 ) |  // Delay[2:0] - Задержка памяти программ при чтении в циклах (в рабочем режиме) ( от 0 до 7 )
    ( 1 << 0 );   // CON - Переключение контроллера EEPROM на регистровое управление ( 0 - управление от ядра, рабочий режим, 1 - управление от регистров, режим программирвоания )
    GetTimeStamp ( &EepromTimeStamp );
    while ( !IsIntervalOverFrom ( 96*5, EepromTimeStamp ) );    
    MDR_EEPROM->CMD =
    ( 1 << 9 ) |   // IFREN - Работа с блоком информации ( 0 - основная память, 1 - информационный блок )
    ( 3 << 3 ) |  // Delay[2:0] - Задержка памяти программ при чтении в циклах (в рабочем режиме) ( от 0 до 7 )
    ( 1 << 0 );   // CON - Переключение контроллера EEPROM на регистровое управление ( 0 - управление от ядра, рабочий режим, 1 - управление от регистров, режим программирвоания )
    MDR_EEPROM->ADR += 4;
    MDR_EEPROM->CMD = ( 3 << 3 );  // Delay[2:0] - Задержка памяти программ при чтении в циклах (в рабочем режиме) ( от 0 до 7 );  
    MDR_EEPROM->KEY = 0x00000000;
    __enable_irq ();
    return 1;
}

uint32_t ReadFromEEPROM ( uint32_t Address ) {
  uint32_t tmpResult;
  uint32_t tmpROMAddress = MDR_EEPROM->ADR;
  MDR_EEPROM->ADR = Address;
  tmpResult = ReadEEPROM ();
  MDR_EEPROM->ADR = tmpROMAddress;
  return tmpResult;
}

uint32_t WriteToEEPROM ( uint32_t Address, uint32_t Value ) {
  uint32_t tmpResult;
  uint32_t tmpROMAddress = MDR_EEPROM->ADR;
  MDR_EEPROM->ADR = Address;
  tmpResult = WriteEEPROM ( Value );
  MDR_EEPROM->ADR = tmpROMAddress;
  return tmpResult;
}

uint32_t ReadsFromEEPROM ( uint32_t* pRAMAddress, uint32_t pROMAddress, uint32_t Size ) {
  uint32_t Index;
  uint32_t Count;
  __disable_irq ();
  MDR_EEPROM->KEY = 0x8AAA5551;
  MDR_EEPROM->CMD |=
    ( 1 << 9 );   // IFREN - Работа с блоком информации ( 0 - основная память, 1 - информационный блок )
  MDR_EEPROM->ADR = pROMAddress;
  MDR_EEPROM->CMD |=
    ( 1 << 8 ) |  // SE - Усилитель считывания ( 0 - не включен, 1 - включен )
    ( 1 << 7 ) |  // YE - Выдача адреса ADR[8:2] ( 0 - не разрешено, 1 - разрешено )
    ( 1 << 6 ) |   // XE - Выдача адреса ADR[16:9] ( 0 - не разрешено, 1 - разрешено )    
  ( 3 << 3 ) ;  // Delay[2:0] - Задержка памяти программ при чтении в циклах (в рабочем режиме) ( от 0 до 7 )
  pROMAddress += 4;
  Count = ( Size + 3 ) / 4;
  for ( Index = 0; Index < Count; Index++ ) {
    *pRAMAddress = MDR_EEPROM->DO;
    pROMAddress += 4;
    MDR_EEPROM->ADR = pROMAddress;
    pRAMAddress++;
  }
  MDR_EEPROM->CMD &= ~(
  ( 1 << 9 ) |  // IFREN - Работа с блоком информации ( 0 - основная память, 1 - информационный блок )
  ( 1 << 8 ) |  // SE - Усилитель считывания ( 0 - не включен, 1 - включен )
  ( 1 << 7 ) |  // YE - Выдача адреса ADR[8:2] ( 0 - не разрешено, 1 - разрешено )
  ( 1 << 6 ) );   // XE - Выдача адреса ADR[16:9] ( 0 - не разрешено, 1 - разрешено )
  MDR_EEPROM->KEY = 0;
  __enable_irq ();
  return 1;
}
