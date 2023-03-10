#include "MDR1986VE1T.h"

void InitEEPROM () {
  MDR_EEPROM->CMD =
    ( 0 << 13 ) | // NVSTR - Операции записи или стирания ( 0 - при чтении, 1 - при записи или стирании )  
    ( 0 << 12 ) | // PROG - Записать данные по ADR[16:2] из регистра EEPROM_DI ( 0 - нет записи, 1 - есть запись )
    ( 0 << 11 ) | // MAS1 - Стереть весь блок, при ERASE = 1 ( 0 - нет стирания, 1 - стирание )
    ( 0 << 10 ) | // ERASE - Стереть строку с адресом ADR[16:9] ( 0 - нет стирания, 1 - стирание )
    ( 0 << 9 ) |  // IFREN - Работа с блоком информации ( 0 - основная память, 1 - информационный блок )
    ( 0 << 8 ) |  // SE - Усилитель считывания ( 0 - не включен, 1 - включен )
    ( 0 << 7 ) |  // YE - Выдача адреса ADR[8:2] ( 0 - не разрешено, 1 - разрешено )
    ( 0 << 6 ) |  // XE - Выдача адреса ADR[16:9] ( 0 - не разрешено, 1 - разрешено )
    ( 0 << 5 ) |  // SE - Усилитель считывания ( 0 - не включен, 1 - включен )
    ( 0 << 4 ) |  // YE - Выдача адреса ADR[8:2] ( 0 - не разрешено, 1 - разрешено )
    ( 3 << 3 ) |  // Delay[2:0] - Задержка памяти программ при чтении в циклах (в рабочем режиме) ( от 0 до 7 )
    ( 0 << 2 ) |  // RD - Чтение из памяти EEPROM (в режиме программирования) ( 0 - нет чтения, 1 - есть чтение )
    ( 0 << 1 ) |  // WR - Запись в память EEPROM (в режиме программирования) ( 0 - нет записи, 1 - есть запись )
    ( 0 << 0 );   // CON - Переключение контроллера EEPROM на регистровое управление ( 0 - управление от ядра, рабочий режим, 1 - управление от регистров, режим программирвоания )
}
