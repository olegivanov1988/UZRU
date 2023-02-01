#ifndef __REFERENCES_H__
#define __REFERENCES_H__

#include "stdint.h"

// Перечисление идентификаторов уставок. Используется функциями записи или чтения уставок
typedef enum {

  PASSWORD,
  SIMPLE_CONST_I_CHARGE_24V_1ST_STAGE_MAX_VOLTAGE,
  SIMPLE_CONST_I_CHARGE_24V_2ND_STAGE_MAX_VOLTAGE,
  SIMPLE_CONST_I_CHARGE_24V_2ND_STAGE_MAX_DELTA_VOLTAGE,
  SIMPLE_CONST_I_CHARGE_12V_1ST_STAGE_MAX_VOLTAGE,
  SIMPLE_CONST_I_CHARGE_12V_2ND_STAGE_MAX_VOLTAGE,
  SIMPLE_CONST_I_CHARGE_12V_2ND_STAGE_MAX_DELTA_VOLTAGE,
  SIMPLE_CONST_I_CHARGE_2ND_STAGE_MIN_TIME,
  SIMPLE_CONST_U_CHARGE_24V_CHARGE_VOLTAGE,
  SIMPLE_CONST_U_CHARGE_12V_CHARGE_VOLTAGE,
  SIMPLE_CONTROL_DISCHARGE_21V_MIN_VOLTAGE,
  SIMPLE_CONTROL_DISCHARGE_20_4V_MIN_VOLTAGE,
  SIMPLE_CONTROL_DISCHARGE_10_5V_MIN_VOLTAGE,
  SIMPLE_CONTROL_DISCHARGE_10_2V_MIN_VOLTAGE,
  SIMPLE_CTC_PAUSE_TIME_HW,
  SIMPLE_CTC_PAUSE_TIME_LW,
  
  NO_BATTERY_VOLTAGE,
  MIN_BATTERY_VOLTAGE,
  MAX_24V_BATTERY_VOLTAGE,
  MIN_24V_BATTERY_VOLTAGE,
  MAX_12V_BATTERY_VOLTAGE,
  MIN_12V_BATTERY_VOLTAGE,
  PAYLOAD_SHORT_CIRQUIT_RESISTANCE,
  BATTERY_SHORT_CIRQUIT_RESISTANCE,
  MIN_NETWORK_VOLTAGE,
  MAX_NETWORK_VOLTAGE,
  MIN_SOURCE_DC_CONVERTER_VOLTAGE,
  MAX_SOURCE_DC_CONVERTER_VOLTAGE,
  MAX_RADIATOR_TEMP,
  NORM_RADIATOR_TEMP,
  
  NUM_OF_REFERENCES ///< Общее число уставок
} eReferenceID;

#define DEFAULT_PASSWORD 0
#define DEFAULT_SIMPLE_CONST_I_CHARGE_24V_1ST_STAGE_MAX_VOLTAGE 2880
#define DEFAULT_SIMPLE_CONST_I_CHARGE_24V_2ND_STAGE_MAX_VOLTAGE 3260
#define DEFAULT_SIMPLE_CONST_I_CHARGE_24V_2ND_STAGE_MAX_DELTA_VOLTAGE 30
#define DEFAULT_SIMPLE_CONST_I_CHARGE_12V_1ST_STAGE_MAX_VOLTAGE 1440
#define DEFAULT_SIMPLE_CONST_I_CHARGE_12V_2ND_STAGE_MAX_VOLTAGE 1630
#define DEFAULT_SIMPLE_CONST_I_CHARGE_12V_2ND_STAGE_MAX_DELTA_VOLTAGE 20
#define DEFAULT_SIMPLE_CONST_I_CHARGE_2ND_STAGE_MIN_TIME 60
#define DEFAULT_SIMPLE_CONST_U_CHARGE_24V_CHARGE_VOLTAGE 2880
#define DEFAULT_SIMPLE_CONST_U_CHARGE_12V_CHARGE_VOLTAGE 1440
#define DEFAULT_SIMPLE_CONTROL_DISCHARGE_21V_MIN_VOLTAGE 2100
#define DEFAULT_SIMPLE_CONTROL_DISCHARGE_20_4V_MIN_VOLTAGE 2040
#define DEFAULT_SIMPLE_CONTROL_DISCHARGE_10_5V_MIN_VOLTAGE 1050
#define DEFAULT_SIMPLE_CONTROL_DISCHARGE_10_2V_MIN_VOLTAGE 1020
#define DEFAULT_SIMPLE_CTC_PAUSE_TIME ((uint64_t)((uint64_t)96000000*(uint64_t)60*(uint64_t)60))

#define DEFAULT_ASIMM_CHARGE_INTERVAL (int64_t)5*60*96000000
#define DEFAULT_ASIMM_DISCHARGE_INTERVAL (int64_t)40*96000000
#define DEFAULT_ASIMM_DISCHARGE_MULTIPLIER 1
#define DEFAULT_ASIMM_DISCHARGE_DIVIDER 5

#define DEFAULT_MIN_NETWORK_VOLTAGE 4180
#define DEFAULT_MAX_NETWORK_VOLTAGE 5800
#define DEFAULT_MIN_SOURCE_DC_CONVERTER_VOLTAGE 3550
#define DEFAULT_MAX_SOURCE_DC_CONVERTER_VOLTAGE 6990
#define DEFAULT_NO_BATTERY_VOLTAGE 600
#define DEFAULT_MIN_BATTERY_VOLTAGE 650

#define DEFAULT_MAX_24V_BATTERY_VOLTAGE 3300
#define DEFAULT_MIN_24V_BATTERY_VOLTAGE 1800
#define DEFAULT_MAX_12V_BATTERY_VOLTAGE 1650
#define DEFAULT_MIN_12V_BATTERY_VOLTAGE 900

#define DEFAULT_MAX_RADIATOR_TEMP 8000
#define DEFAULT_NORM_RADIATOR_TEMP 7500

#ifndef __REFERENCES_C__
extern int32_t References [NUM_OF_REFERENCES];
#define MinNetworkVoltage (References[MIN_NETWORK_VOLTAGE])
#define MaxNetworkVoltage (References[MAX_NETWORK_VOLTAGE])
#define MinSourceDCConverterVoltage (References[MIN_SOURCE_DC_CONVERTER_VOLTAGE])
#define MaxSourceDCConverterVoltage (References[MAX_SOURCE_DC_CONVERTER_VOLTAGE])
#define NoBatteryVoltage (References[NO_BATTERY_VOLTAGE])
#define MinBatteryVoltage (References[MIN_BATTERY_VOLTAGE])
#endif

#define SetReferenceValue(x,y) if ((x)<NUM_OF_REFERENCES) { References[(x)]=(y); }
#define GetReferenceValue(x) References[(x)]
#define DefaultTargetFreqx100 (References[DEFAULT_TARGET_FREQX100])

void SaveReferences ( void );
void SetDefaultReferences ( void );
void InitReferences ( void );
void AnalyzeReferences ( void );
void DispatchReferences ( void );

#endif // __REFERENCES_H__
