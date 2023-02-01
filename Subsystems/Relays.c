#include "MDR1986VE1T.h"
#include "DriverMDR1986VE1T.h"
#include "Relays.h"
#include "TimeControl.h"
#include "Board.h"

uint32_t RelayIndex = 0;

uint32_t RelayStates[NUM_OF_RELAYS];
RelayInfo_Typedef RelayInfos[NUM_OF_RELAYS] = {
  { (uint64_t)0, MDR_PORTD, 6 },
  { (uint64_t)0, MDR_PORTD, 5 },
  { (uint64_t)0, MDR_PORTD, 4 },
  { (uint64_t)0, MDR_PORTD, 3 },
  { (uint64_t)0, MDR_PORTD, 1 },
  { (uint64_t)0, MDR_PORTD, 0 },
  { (uint64_t)0, MDR_PORTB, 11 },
  { (uint64_t)0, MDR_PORTB, 12 },
  { (uint64_t)0, MDR_PORTB, 13 },
  { (uint64_t)0, MDR_PORTB, 14 },
  { (uint64_t)0, MDR_PORTB, 15 }
};

void InitRelays ( void ){
  int32_t i;
  for ( i = 0; i < NUM_OF_RELAYS; i++ ) {
    SetRelayState ( i, OFF);
  }
}
void SetRelayState ( uint32_t RelayIndex, uint32_t NewState ) {
  if ( RelayIndex < NUM_OF_RELAYS ) {
    if ( ( RelayStates[RelayIndex] & 0x3 ) != NewState ) {
      RelayStates[RelayIndex] = NewState;
      GetTimeStamp ( &(RelayInfos[RelayIndex].StateTimeStamp) );
    }
  }
}

uint32_t GetRelayState ( uint32_t RelayIndex ) {
  return ( RelayStates[RelayIndex] & 3 );
}

void AnalyzeRelays ( void ) {
  
}

void DispatchRelays ( void ) {
  RelayIndex++;
  RelayIndex = RelayIndex % NUM_OF_RELAYS;
  if ( RelayIndex < 4 ) {
    if ( RelayStates[RelayIndex] == OFF ) {
      (RelayInfos[RelayIndex].RelayPort)->OE &= ~( 1 << RelayInfos[RelayIndex].PortPin );
      MDR1986VE1T_SetPortPin ( RelayInfos[RelayIndex].RelayPort, RelayInfos[RelayIndex].PortPin );
    }
    else if ( RelayStates[RelayIndex] == ON ) {
      (RelayInfos[RelayIndex].RelayPort)->OE |= 1 << RelayInfos[RelayIndex].PortPin;
      MDR1986VE1T_ClearPortPin ( RelayInfos[RelayIndex].RelayPort, RelayInfos[RelayIndex].PortPin );
    }
    else if ( RelayStates[RelayIndex] == PULSE_ON ) {
      if ( IsIntervalOverFrom ( 48000000, RelayInfos[RelayIndex].StateTimeStamp ) ) {
        RelayStates[RelayIndex] = PULSE_OFF;
        (RelayInfos[RelayIndex].RelayPort)->OE &= ~( 1 << RelayInfos[RelayIndex].PortPin );
        MDR1986VE1T_SetPortPin ( RelayInfos[RelayIndex].RelayPort, RelayInfos[RelayIndex].PortPin );
        GetTimeStamp ( &(RelayInfos[RelayIndex].StateTimeStamp) );
      }
    }
    else if ( RelayStates[RelayIndex] == PULSE_OFF ) {
      if ( IsIntervalOverFrom ( 48000000, RelayInfos[RelayIndex].StateTimeStamp ) ) {
        RelayStates[RelayIndex] = PULSE_ON;
        (RelayInfos[RelayIndex].RelayPort)->OE |= 1 << RelayInfos[RelayIndex].PortPin;
        MDR1986VE1T_ClearPortPin ( RelayInfos[RelayIndex].RelayPort, RelayInfos[RelayIndex].PortPin );
        GetTimeStamp ( &(RelayInfos[RelayIndex].StateTimeStamp) );
      }
    }
  }
  else {
    if ( RelayStates[RelayIndex] == OFF ) {
      MDR1986VE1T_ClearPortPin ( RelayInfos[RelayIndex].RelayPort, RelayInfos[RelayIndex].PortPin );
    }
    else if ( RelayStates[RelayIndex] == ON ) {
      MDR1986VE1T_SetPortPin ( RelayInfos[RelayIndex].RelayPort, RelayInfos[RelayIndex].PortPin );
    }
    else if ( RelayStates[RelayIndex] == PULSE_ON ) {
      if ( IsIntervalOverFrom ( 48000000, RelayInfos[RelayIndex].StateTimeStamp ) ) {
        RelayStates[RelayIndex] = PULSE_OFF;
        MDR1986VE1T_ClearPortPin ( RelayInfos[RelayIndex].RelayPort, RelayInfos[RelayIndex].PortPin );
        GetTimeStamp ( &(RelayInfos[RelayIndex].StateTimeStamp) );
      }
    }
    else if ( RelayStates[RelayIndex] == PULSE_OFF ) {
      if ( IsIntervalOverFrom ( 48000000, RelayInfos[RelayIndex].StateTimeStamp ) ) {
        RelayStates[RelayIndex] = PULSE_ON;
        MDR1986VE1T_SetPortPin ( RelayInfos[RelayIndex].RelayPort, RelayInfos[RelayIndex].PortPin );
        GetTimeStamp ( &(RelayInfos[RelayIndex].StateTimeStamp) );
      }
    }
  }
}