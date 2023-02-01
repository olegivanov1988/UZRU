#ifndef __DRIVER_MDR_EEPROM_H__
#define __DRIVER_MDR_EEPROM_H__

#include "stdint.h"

void SetEEPROMAddress ( uint32_t NewAddress );
uint32_t GetEEPROMAddress ( void );

uint32_t GetEEPROMValue ( void ) __attribute__ ((section(".ramfunc")));
void ClearEEPROMSector ( uint32_t SectorIndex ) __attribute__ ((section(".ramfunc")));
uint32_t ReadEEPROM ( void );
uint32_t ReadsFromEEPROM ( uint32_t* pRAMAddress, uint32_t pROMAddress, uint32_t Size )  __attribute__ ((section(".ramfunc")));
uint32_t WriteEEPROM ( uint32_t Value )  __attribute__ ((section(".ramfunc")));

uint32_t ReadFromEEPROM ( uint32_t Address );
uint32_t WriteToEEPROM ( uint32_t Address, uint32_t Value );

#endif // __DRIVER_MDR_EEPROM_H__
