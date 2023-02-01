#include "main.h"
#include "Subsystems.h"
#include "Display.h"
#include "Keyboard.h"
#include "Menu.h"
#include "Faults.h"
#include "Measurements.h"
#include "TimeControl.h"
#include "References.h"
#include "DriverMDR1986VE1T.h"

char Symbols[2][16] = { "                ", "                " };

int32_t CursorCol; // Столбец курсора, от 1 до 2
int32_t CursorRow; // Строка курсора, 1 или 2

int32_t OutCursorCol;
int32_t OutCursorRow;

#define LIGHT 0
#define E 2
#define RW 0
#define A0 1
#define DB4 3
#define DB5 4
#define DB6 5
#define DB7 6

static __inline void Pause ( uint32_t us ) {
  uint32_t Counter = us * 96;
  while ( Counter > 0 ) { Counter--; }
}

void SendCommandToLCD ( uint32_t Command ) {
  MDR_PORTA->SETTX = ( 1 << RW ) | ( 1 << A0);
  MDR_PORTA->SETTX = ( ~( ( Command >> 4 ) << DB4 ) ) & ( 0x0F << DB4 ); 
  MDR_PORTA->CLRTX = ( ( Command >> 4 ) << DB4 );
  Pause ( 4 );
  MDR_PORTA->CLRTX = 1 << E;
  Pause ( 4 );
  MDR_PORTA->SETTX = 1 << E;
  Pause ( 4 );
  MDR_PORTA->SETTX =  ( ~( ( Command & 0x0000000F ) << DB4 ) ) & ( 0x0F << DB4 ); 
  MDR_PORTA->CLRTX = ( ( Command & 0x0000000F ) << DB4 );
  Pause ( 4 );
  MDR_PORTA->CLRTX = 1 << E;
  Pause ( 4 );
  MDR_PORTA->SETTX = 1 << E;
  Pause ( 80 );
  MDR_PORTA->SETTX = ( 0x0000000F << DB4 ); 
  MDR_PORTA->SETTX = 1 << A0;
}

void SendDataToLCD ( uint32_t Data ) {
  MDR_PORTA->SETTX = ( 1 << RW ); // 0 -> RW
  MDR_PORTA->CLRTX = 1 << A0; // 1 -> A0
  MDR_PORTA->SETTX = ( ~( ( Data >> 4 ) << DB4 ) ) & ( 0x0F << DB4 );
  MDR_PORTA->CLRTX = ( ( Data >> 4 ) << DB4 );
  Pause ( 4 );
  MDR_PORTA->CLRTX = 1 << E; // 1 -> E
  Pause ( 4 );
  MDR_PORTA->SETTX = 1 << E; // 0 -> E
  Pause ( 4 );
  MDR_PORTA->SETTX = ( ~( ( Data & 0x0000000F ) << DB4 ) ) & ( 0x0F << DB4 ); 
  MDR_PORTA->CLRTX = ( ( Data & 0x0000000F ) << DB4 );
  Pause ( 4 );
  MDR_PORTA->CLRTX = 1 << E; // 1 -> E
  Pause ( 4 );
  MDR_PORTA->SETTX = 1 << E; // 0 -> E
  Pause ( 80 );
  MDR_PORTA->SETTX = 1 << A0; // 0 -> A0
  MDR_PORTA->SETTX = ( 0x0000000F << DB4 ); // 0 -> DATA
}

static __inline void LCDInit ( void ) {
  MDR_PORTA->SETTX = 1 << E;   // 0 -> E
  Pause ( 40000 );
  MDR_PORTA->SETTX = ( 1 << RW ) | ( 1 << A0 ) | ( 1 << DB6 ) | ( 1 << DB7 ); // 0 -> RW, A0, DB6, DB7
  MDR_PORTA->CLRTX = ( 1 << DB4 ) | ( 1 << DB5 ); // 1 -> DB4, DB5 ( 0011 -> DATA )
  Pause ( 4 );
  MDR_PORTA->CLRTX = 1 << E; // 1 -> E
  Pause ( 4 );
  MDR_PORTA->SETTX = 1 << E; // 0 -> E
  Pause ( 80 );
  MDR_PORTA->CLRTX = 1 << E; // 1 -> E
  Pause ( 4 );
  MDR_PORTA->SETTX = 1 << E; // 0 -> E
  Pause ( 80 );
  MDR_PORTA->CLRTX = 1 << E; // 1 -> E
  Pause ( 4 );
  MDR_PORTA->SETTX = 1 << E; // 0 -> E
  Pause ( 80 );
  
  MDR_PORTA->SETTX = 1 << DB4; // 0010 -> DATA
  MDR_PORTA->CLRTX = 1 << E; // 1 -> E
  Pause ( 4 );
  MDR_PORTA->SETTX = 1 << E; // 0 -> E
  Pause ( 80 );
  
  SendCommandToLCD ( 0x2A );
  SendCommandToLCD ( 0x0C );
  SendCommandToLCD ( 0x01 );
  SendCommandToLCD ( 0x06 );
}

void OutSymbol ( void ) {
  if ( OutCursorCol == 0 ) {
    if ( OutCursorRow ) {
      SendCommandToLCD ( 0x80 + 0x40 );
    }
    else {
      SendCommandToLCD ( 0x80 );
    }
  }
  SendDataToLCD ( Symbols[OutCursorRow][OutCursorCol] );
  OutCursorCol++;
  OutCursorRow += OutCursorCol / 16;
  OutCursorRow %= 2;
  OutCursorCol %= 16;
}

void WriteText ( char* pText, uint32_t Length, uint32_t Width, uint32_t Pos, uint32_t Visible );

void DispatchDisplay () {
  OutSymbol ();
}

void InitDisplay ( void ) {                              // Функция, выполняющая инициализацию объекта "Двигатель"
  LCDInit ();
  SendCommandToLCD ( 0x80 );
  // Board_TurnOffWorkLed ();
}

void WriteChar ( char Symbol ) {
  if ( CursorCol < 16 ) {
    Symbols[CursorRow][CursorCol] = Symbol;
  }
  CursorCol++;
}

void WriteDigit ( uint32_t Digit ) {
  if ( CursorCol < 16 ) {
    Symbols[CursorRow][CursorCol] = '0' + Digit%10;
  }
  CursorCol++;
}

void DisplayNewLine ( void ) {
  while ( CursorCol < 16 ) {
    Symbols[CursorRow][CursorCol] = ' ';
    CursorCol++;
  }
}

void WriteMinus ( void ) {
  if ( CursorCol < 16 ) {
    Symbols[CursorRow][CursorCol] = '-';
  }
  CursorCol++;
}

void WriteDot ( void ) {
  if ( CursorCol < 16 ) {
    Symbols[CursorRow][CursorCol] = '.';
  }
  CursorCol++;
}

void WriteExp ( void ) {
  if ( CursorCol < 16 ) {
    Symbols[CursorRow][CursorCol] = 'e';
  }
  CursorCol++;
}

static int32_t Index1;
static int32_t Index2;

void WriteText ( char* pText, uint32_t Length, uint32_t Width, uint32_t Pos, uint32_t Visible ) {
  if ( Visible ) {
    for ( Index1 = Pos, Index2 = 0; ( ( Index1 < Length ) && ( Index2 < Width ) && ( CursorRow < 16 ) ); Index1++, Index2++ ) {
      WriteChar ( pText[Index1] );
    }
  }
  else {
    for ( Index1 = Pos, Index2 = 0; ( ( Index1 < Length ) && ( Index2 < Width ) && ( CursorRow < 16 ) ); Index1++, Index2++ ) {
      WriteChar ( ' ' );
    }
  }
  for ( Index1 = Index2; ( ( Index1 < Width ) & ( CursorRow < 16 ) ); Index1++ ) {
    WriteChar ( ' ' );
  }
}

void WriteInt ( int32_t* pInt, uint32_t Width ) {
  int32_t i;
  int32_t tmpValue = *pInt;
  int32_t Divider = 1;
  for ( i = 0; i < Width - 1; i++ ) {
    Divider *= 10;
  }
  for ( Index1 = 0; Index1 < Width; Index1++ ) {
    WriteDigit ( tmpValue / Divider );
    tmpValue %= Divider;
    Divider /= 10;
  }
}

/*
void WriteFixedPointNumber ( int32_t* pInt, int32_t Divider, int32_t Width ) {
  int32_t tmpValue;
  int32_t tmpValue1;
  int32_t tmpDivider = 1;
  tmpValue1 = *pInt;
  tmpValue = tmpValue1/Divider;
  if ( tmpValue <= 0 ) { WriteDigit(0); WriteChar (','); Width -= 2; }
  else {
    while ( tmpValue >= tmpDivider ) {
      tmpDivider *= 10;
    }
    tmpDivider /= 10;
    while ( tmpDivider ) {
      WriteDigit ( tmpValue/tmpDivider );
      tmpValue %= tmpDivider;
      tmpDivider /= 10;
      Width--;
    }
    WriteChar (',');
    Width--;
  }
  tmpValue = tmpValue1 % Divider;
  while ( Width > 0 ) {
    Divider /= 10;
    if ( Divider  > 0 ) { WriteDigit ( tmpValue / Divider ); }
    else { WriteDigit(0); }
    tmpValue %= Divider;
    Width--;
  }
} //*/

//*
int32_t Dividers[9] = { 1, 100, 100, 100, 1000, 10000, 100000, 1000000, 10000000 };

void WriteFixedPointNumber ( int32_t tmpValue, int32_t Divider, int32_t Width ) {
  if ( tmpValue < 0 ) {
    WriteChar ( '-' );
    WriteFixedPointNumber ( -tmpValue, Divider, Width-1 );
  }
    else {
    int32_t tmpDivider = Dividers[Width];
    tmpValue = tmpValue + 5;
    if ( tmpValue < 1000 ) { tmpDivider = 100; }
    else if ( tmpValue < 10000 ) { tmpDivider = 1000; }
    else if ( tmpValue < 100000 ) { tmpDivider = 10000; }
    else if ( tmpValue < 1000000 ) { tmpDivider = 100000; }
    else if ( tmpValue < 10000000 ) { tmpDivider = 1000000; }
    else if ( tmpValue < 100000000 ) { tmpDivider = 10000000; }
    else if ( tmpValue < 1000000000 ) { tmpDivider = 100000000; }
    while ( tmpDivider > 10 ) {
      WriteDigit ( tmpValue/tmpDivider );
      tmpValue = tmpValue % tmpDivider;
      tmpDivider = tmpDivider / 10;
    }
    WriteChar (',');
    WriteDigit ( tmpValue / tmpDivider );
  }
}//*/

void WriteFloat ( float* pFloat, uint32_t Width ) {
  
}

void WriteTime ( uint64_t* pTime ) {
  uint64_t tmpTime = *pTime;
  WriteDigit ( (uint64_t)tmpTime / (uint64_t)96000000 / (uint64_t) 36000 );
  tmpTime = (uint64_t)( (uint64_t)tmpTime % ( (uint64_t)96000000 * (uint64_t)36000 ) );
  WriteDigit ( (uint64_t)tmpTime / (uint64_t)96000000 / (uint64_t) 3600 );
  tmpTime = (uint64_t)( (uint64_t)tmpTime % ( (uint64_t)96000000 * (uint64_t)3600 ) );
  WriteChar ( 'ч' );
  WriteDigit ( (uint64_t)tmpTime / (uint64_t)96000000 / (uint64_t) 600 );
  tmpTime = (uint64_t)((uint64_t)tmpTime % ( (uint64_t)96000000 * (uint64_t)600 ));
  WriteDigit ( (uint64_t)tmpTime / (uint64_t)96000000 / (uint64_t) 60 );
  tmpTime = (uint64_t)((uint64_t)tmpTime % ( (uint64_t)96000000 * (uint64_t)60 ));
  WriteChar ( 'м' );
  WriteDigit ( (uint64_t)tmpTime / ( 96000000 * 10 ) );
  tmpTime = (uint64_t)((uint64_t)tmpTime % ( (uint64_t)96000000 * (uint64_t)10 ));
  WriteDigit ( (uint64_t)tmpTime / ( 96000000 ) );
  WriteChar ( 'с' );  
}

void AnalyzeDisplay ( ) {
}

void DisplayStart1stLine () {
  CursorCol = 0;
  CursorRow = 0;
}

void DisplayStart2ndLine () {
  CursorCol = 0;
  CursorRow = 1;
}

