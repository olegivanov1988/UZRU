#include "DriverMDR1986VE1T.h"

#include "TimeControl.h"

Time_Stamp_TypeDef tmpTimeStamp;

void GetTimeStamp ( uint64_t* pResult ) {
  do {
    tmpTimeStamp.AsUint32s.HW = MDR_TIMER3->CNT;          // Копируем в старшую часть метки времени счётчик таймера 3
    tmpTimeStamp.AsUint32s.LW = MDR_TIMER4->CNT;          // Копируем в младшую часть метки времени счётчик таймера 4 (таймеры 4 и 3 соединены 
  } while ( tmpTimeStamp.AsUint32s.HW != MDR_TIMER3->CNT );
  *pResult = tmpTimeStamp.AsUint64;
}

void GetIntervalFrom ( uint64_t FromTimeStamp, uint64_t* pResult ) {
  uint64_t Tmp;
  GetTimeStamp ( &Tmp );
  *pResult = ( Tmp - FromTimeStamp );
}

void GetIntervalFromTo ( uint64_t FromTimeStamp, uint64_t ToTimeStamp, uint64_t* pResult ) {
  *pResult = ToTimeStamp - FromTimeStamp;
}

uint32_t IsIntervalOverFrom ( uint64_t Interval, uint64_t FromTimeStamp ) {
  uint64_t tmpTimeStamp;
  GetTimeStamp ( &tmpTimeStamp );
  if ( ( tmpTimeStamp - FromTimeStamp ) >= Interval ) {
    return 1;
  }
  else {
    return 0;
  }
}

uint32_t IsIntervalOverFromTo ( uint64_t Interval, uint64_t FromTimeStamp, uint64_t ToTimeStamp ) {
  if ( ( ToTimeStamp - FromTimeStamp ) >= Interval ) {
    return 1;
  }
  else {
    return 0;
  }  
}

uint32_t GetShortTimeStamp ( uint32_t* pShortTimeStamp ) {
  *pShortTimeStamp = MDR_TIMER4->CNT;
}
