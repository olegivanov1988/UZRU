#ifndef __MemAPI_h__
#define __MemAPI_h__

#include "stdint.h"
//#include "1986VE1T.h"

void MemCpy ( void* Dst, void* Src, uint16_t Size);
uint8_t MemCmp ( void* Dst, void* Src, uint16_t Size);

uint32_t PreCheckSum (uint32_t PreValue, void* Buf, uint16_t Size);
uint16_t CheckSum (uint32_t PreValue, void* Buf, uint16_t Size);

#endif // __MemAPI_h__
