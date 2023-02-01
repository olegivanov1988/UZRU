
#ifndef __MemAPI_c__
#define __MemAPI_c__

#include "stdint.h"
#include "MemAPI.h"

void MemCpy ( void* Dst, void* Src, uint16_t Size)
{
  uint16_t i;
  uint8_t* src = (uint8_t*)Src;
  uint8_t* dst = (uint8_t*)Dst;
  for ( i=0; i<Size; i++) { 
    *dst=*src;
    dst++;
    src++;
  }
}

uint8_t MemCmp ( void* Dst, void* Src, uint16_t Size)
{
  uint16_t i;
  uint8_t* src = (uint8_t*)Src;
  uint8_t* dst = (uint8_t*)Dst;
  for ( i=0; i<Size; i++) { 
    if ( *dst !=*src ) return 1;
    dst++;
    src++;
  }
  return 0;
}

uint32_t PreCheckSum (uint32_t PreValue, void* Buf, uint16_t Size)
{
  uint16_t Num = Size/2;
  uint16_t i;
  uint32_t cs = PreValue;
  for(i=0; i< 2*Num; i+=2 ) {
    cs += (((uint8_t*)Buf)[i] ) + ((((uint8_t*)Buf)[i+1])<< 8);
  }
  if ( Size > 2*Num ) {
    cs+= ((uint8_t*)Buf)[i];
  }
  return cs;
}

uint16_t CheckSum (uint32_t PreValue, void* Buf, uint16_t Size)
{
  uint16_t Num = Size/2;
  uint16_t i;
  uint32_t cs = PreValue;
  for(i=0; i< 2*Num; i+=2 ) {
    cs+= (((uint8_t*)Buf)[i])  + ((((uint8_t*)Buf)[i+1])<< 8);
  }
  if ( Size > 2*Num ) {
    cs+= ((uint8_t*)Buf)[i];
  }
  cs=(cs>>16)+(cs&0xFFFF);
  return (uint16_t)(~cs);
}

char i [10] = {'0','1','2','3','4','5','6','7','8','9' };
char Result[10] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};

char* IntToString (uint32_t Int ) {

  uint32_t Index = 9;
  while ( Index > 0 ) {
     Result[Index]=i[Int%10];
     Int /= 10;
     Index--;
  }
  return Result;
}

#endif // __MemAPI_c__
