#define __REFERENCES_C__

#include "References.h"
#include "EEPROM.h"
#include "main.h"

int32_t References[NUM_OF_REFERENCES];

void SetDefaultReferences ( ) {
  
  References[PASSWORD] = DEFAULT_PASSWORD;
  References[SIMPLE_CONST_I_CHARGE_24V_1ST_STAGE_MAX_VOLTAGE] = DEFAULT_SIMPLE_CONST_I_CHARGE_24V_1ST_STAGE_MAX_VOLTAGE;
  References[SIMPLE_CONST_I_CHARGE_24V_2ND_STAGE_MAX_VOLTAGE] = DEFAULT_SIMPLE_CONST_I_CHARGE_24V_2ND_STAGE_MAX_VOLTAGE;
  References[SIMPLE_CONST_I_CHARGE_24V_2ND_STAGE_MAX_DELTA_VOLTAGE] = DEFAULT_SIMPLE_CONST_I_CHARGE_24V_2ND_STAGE_MAX_DELTA_VOLTAGE;
  References[SIMPLE_CONST_I_CHARGE_12V_1ST_STAGE_MAX_VOLTAGE] = DEFAULT_SIMPLE_CONST_I_CHARGE_12V_1ST_STAGE_MAX_VOLTAGE;
  References[SIMPLE_CONST_I_CHARGE_12V_2ND_STAGE_MAX_VOLTAGE] = DEFAULT_SIMPLE_CONST_I_CHARGE_12V_2ND_STAGE_MAX_VOLTAGE;
  References[SIMPLE_CONST_I_CHARGE_12V_2ND_STAGE_MAX_DELTA_VOLTAGE] = DEFAULT_SIMPLE_CONST_I_CHARGE_12V_2ND_STAGE_MAX_DELTA_VOLTAGE;
  References[SIMPLE_CONST_I_CHARGE_2ND_STAGE_MIN_TIME] = DEFAULT_SIMPLE_CONST_I_CHARGE_2ND_STAGE_MIN_TIME;
  References[SIMPLE_CONST_U_CHARGE_24V_CHARGE_VOLTAGE] = DEFAULT_SIMPLE_CONST_U_CHARGE_24V_CHARGE_VOLTAGE;
  References[SIMPLE_CONST_U_CHARGE_12V_CHARGE_VOLTAGE] = DEFAULT_SIMPLE_CONST_U_CHARGE_12V_CHARGE_VOLTAGE;
  References[SIMPLE_CONTROL_DISCHARGE_21V_MIN_VOLTAGE] = DEFAULT_SIMPLE_CONTROL_DISCHARGE_21V_MIN_VOLTAGE;
  References[SIMPLE_CONTROL_DISCHARGE_20_4V_MIN_VOLTAGE] = DEFAULT_SIMPLE_CONTROL_DISCHARGE_20_4V_MIN_VOLTAGE;
  References[SIMPLE_CONTROL_DISCHARGE_10_5V_MIN_VOLTAGE] = DEFAULT_SIMPLE_CONTROL_DISCHARGE_10_5V_MIN_VOLTAGE;
  References[SIMPLE_CONTROL_DISCHARGE_10_2V_MIN_VOLTAGE] = DEFAULT_SIMPLE_CONTROL_DISCHARGE_10_2V_MIN_VOLTAGE;
  *((uint64_t*)(&(References[SIMPLE_CTC_PAUSE_TIME_HW]))) = DEFAULT_SIMPLE_CTC_PAUSE_TIME;

  References[MIN_NETWORK_VOLTAGE] = DEFAULT_MIN_NETWORK_VOLTAGE;
  References[MAX_NETWORK_VOLTAGE] = DEFAULT_MAX_NETWORK_VOLTAGE;
  References[MIN_SOURCE_DC_CONVERTER_VOLTAGE] = DEFAULT_MIN_SOURCE_DC_CONVERTER_VOLTAGE;
  References[MAX_SOURCE_DC_CONVERTER_VOLTAGE] = DEFAULT_MAX_SOURCE_DC_CONVERTER_VOLTAGE;
  References[NO_BATTERY_VOLTAGE] = DEFAULT_NO_BATTERY_VOLTAGE;
  References[MIN_BATTERY_VOLTAGE] = DEFAULT_MIN_BATTERY_VOLTAGE;
  
  References[MAX_24V_BATTERY_VOLTAGE] = DEFAULT_MAX_24V_BATTERY_VOLTAGE;
  References[MIN_24V_BATTERY_VOLTAGE] = DEFAULT_MIN_24V_BATTERY_VOLTAGE;
  References[MAX_12V_BATTERY_VOLTAGE] = DEFAULT_MAX_12V_BATTERY_VOLTAGE;
  References[MIN_12V_BATTERY_VOLTAGE] = DEFAULT_MIN_12V_BATTERY_VOLTAGE;
  
  References[MAX_RADIATOR_TEMP] = DEFAULT_MAX_RADIATOR_TEMP;
  References[NORM_RADIATOR_TEMP] = DEFAULT_NORM_RADIATOR_TEMP;
}

void SaveReferences ( ) {
  UpdateInEEPROM ( References );
}

void UpdateReferencesFromTo ( ) {
}

void InitReferences () {
  // AssignWithEEPROM ( References, NUM_OF_REFERENCES, SetDefaultReferences );
}

void AnalyzeReferences () {
}

void DispatchReferences () {
}
