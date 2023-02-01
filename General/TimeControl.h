#ifndef __TIME_CONTROL_H__
#define __TIME_CONTROL_H__

#include "stdint.h"

#pragma pack (push, 1)
typedef volatile struct {
  uint32_t LW;
  uint32_t HW;
} Uint64_Struct_TypeDef;
#pragma pack (pop)

typedef volatile union {
  uint64_t AsUint64;
  Uint64_Struct_TypeDef AsUint32s;
} Time_Stamp_TypeDef;

void GetTimeStamp ( uint64_t* pResult )   __attribute__ ((section(".ramfunc")));
void GetIntervalFrom ( uint64_t FromTimeStamp, uint64_t* pResult );
void GetIntervalFromTo ( uint64_t FromTimeStamp, uint64_t ToTimeStamp, uint64_t* pResult );
uint32_t IsIntervalOverFrom ( uint64_t Interval, uint64_t FromTimeStamp )   __attribute__ ((section(".ramfunc")));
uint32_t IsIntervalOverFromTo ( uint64_t Interval, uint64_t FromTimeStamp, uint64_t ToTimeStamp );
  

#endif // __TIME_CONTROL_H__
