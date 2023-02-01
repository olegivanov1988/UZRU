#include "main.h"
#include "Variables.h"
#include "TimeControl.h"
#include "DriverMDR1986VE1T.h"

extern uint32_t Segments;
extern uint32_t CommaPosition;
extern uint32_t PulsationMode;

uint32_t CurrentSegment;
uint32_t CurrentSegmentTimeStamp;
uint32_t CurrentMode;
uint64_t LastModeTimeStamp;
uint64_t LastPulsationTimeStamp;
uint64_t LastPulsationModeTimeStamp;
uint64_t PulsationState;

uint32_t CodePage [16] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x6E, 0x37, 0x76, 0x73, 0x40 }; // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, А, У, П, Н, Р, -

void InitSegments ( ) {
  CommaPosition = 0;
}

uint64_t LastDigitTimeStamp = 0;
uint32_t Digit = 0;
	uint64_t LastIndicatorTimeStamp = 0;
uint64_t IndicatorIndex = 0;

void DispatchSegments ( ) {
	int32_t i;
	
  if ( IsIntervalOverFrom ( LastIndicatorTimeStamp, 96000 ) ) {
		IndicatorIndex++;
		IndicatorIndex %= 4;
		GetTimeStamp ( &LastIndicatorTimeStamp );
	  MDR_PORTA->CLRTX = ( 0xFFF );
	  MDR_PORTA->SETTX = CodePage[0xF & (Segments >> ( 8 * IndicatorIndex ) )];
		if ( CommaPosition == IndicatorIndex ) { MDR_PORTA->SETTX = 0x80; }
	  MDR_PORTA->SETTX = ( ( 1 << IndicatorIndex ) << 8 );
		if ( ( PulsationMode < 6 ) && ( PulsationState == 1 ) ) {
				if ( PulsationMode == 5 ) {
					MDR_PORTA->CLRTX = ( 0xFF );
				}
				else if ( PulsationMode == 4 ) {
					MDR_PORTA->CLRTX = ( 0x80 );
				}
				else if ( PulsationMode == IndicatorIndex ) {
					MDR_PORTA->CLRTX = ( 0x7F );
				}
			}
		if ( ( PulsationMode < 6 ) && ( IsIntervalOverFrom ( LastPulsationTimeStamp, 24000000 ) ) ) {
			PulsationState = (PulsationState+1)%2;
			GetTimeStamp ( &LastPulsationTimeStamp );
		}
	} 
}

void SegmentsIndicate () { // Функция отображает на дисплее текущее сообщение.
  
}
