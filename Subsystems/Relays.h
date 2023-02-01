#ifndef __RELAYS_H__
#define __RELAYS_H__

#include "stdint.h"
#include "MDR1986VE1T.h"
#include "Display.h"

typedef enum {
  OFF = 0,
  ON = 1,
  PULSE = 2,
  PULSE_OFF = PULSE,
  PULSE_ON = PULSE_OFF | 4
} RelayState_Typedef;

typedef enum {
  WORK_LED,
  FAULT_LED,
  HIGHLIGHT,
  LOCK_PAYLOAD,
  START_DISCHARGE,
  START_CHARGE,
  CONVERTER_CONTACTOR,
  LOAD_CONTACTOR,
  PRECHARGE_RELAY,
  GREEN_LED,
  YELLOW_LED,
  NUM_OF_RELAYS
} RelayIndex_Typedef;

#ifndef __RELAYS_C__
extern uint32_t RelayStates[NUM_OF_RELAYS];
#endif // __RELAYS_C__

typedef struct {
  uint64_t StateTimeStamp;
  MDR_PORT_TypeDef* RelayPort;
  uint32_t PortPin;
} RelayInfo_Typedef;

void InitRelays ( void );
void SetRelayState ( uint32_t RelayIndex, uint32_t NewState );
uint32_t GetRelayState ( uint32_t RelayIndex );
void AnalyzeRelays ( void );
void DispatchRelays ( void );

#endif // __RELAYS_H__
