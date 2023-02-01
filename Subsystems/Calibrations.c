#define __CALIBRATIONS_C__

#include "main.h"
#include "EEPROM.h"
#include "Calibrations.h"
#include "Measurements.h"

int32_t Calibrations[NUM_OF_CALIBRATIONS];

void SetDefaultCalibrations ( ) {
   Calibrations[CHARGE_CURRENT_ON_0_PREVALUE_INDEX] = CHARGE_CURRENT_ON_0_PREVALUE; // 0
   Calibrations[CHARGE_CURRENT_ON_16_PREVALUE_INDEX] = CHARGE_CURRENT_ON_16_PREVALUE; // 44
   Calibrations[CHARGE_CURRENT_ON_32_PREVALUE_INDEX] = CHARGE_CURRENT_ON_32_PREVALUE; // 88
   Calibrations[CHARGE_CURRENT_ON_48_PREVALUE_INDEX] = CHARGE_CURRENT_ON_48_PREVALUE; // 132
   Calibrations[CHARGE_CURRENT_ON_64_PREVALUE_INDEX] = CHARGE_CURRENT_ON_64_PREVALUE; // 176
   Calibrations[CHARGE_CURRENT_ON_80_PREVALUE_INDEX] = CHARGE_CURRENT_ON_80_PREVALUE; // 220
   Calibrations[CHARGE_CURRENT_ON_96_PREVALUE_INDEX] = CHARGE_CURRENT_ON_96_PREVALUE; // 264
   Calibrations[CHARGE_CURRENT_ON_112_PREVALUE_INDEX] = CHARGE_CURRENT_ON_112_PREVALUE; // 308
   Calibrations[CHARGE_CURRENT_ON_128_PREVALUE_INDEX] = CHARGE_CURRENT_ON_128_PREVALUE; // 352
   Calibrations[CHARGE_CURRENT_ON_144_PREVALUE_INDEX] = CHARGE_CURRENT_ON_144_PREVALUE; // 397
   Calibrations[CHARGE_CURRENT_ON_160_PREVALUE_INDEX] = CHARGE_CURRENT_ON_160_PREVALUE; // 441
   Calibrations[CHARGE_CURRENT_ON_176_PREVALUE_INDEX] = CHARGE_CURRENT_ON_176_PREVALUE; // 485
   Calibrations[CHARGE_CURRENT_ON_192_PREVALUE_INDEX] = CHARGE_CURRENT_ON_192_PREVALUE; // 529
   Calibrations[CHARGE_CURRENT_ON_208_PREVALUE_INDEX] = CHARGE_CURRENT_ON_208_PREVALUE; // 573
   Calibrations[CHARGE_CURRENT_ON_224_PREVALUE_INDEX] = CHARGE_CURRENT_ON_224_PREVALUE; // 617
   Calibrations[CHARGE_CURRENT_ON_240_PREVALUE_INDEX] = CHARGE_CURRENT_ON_240_PREVALUE; // 661
   Calibrations[CHARGE_CURRENT_ON_256_PREVALUE_INDEX] = CHARGE_CURRENT_ON_256_PREVALUE; // 705
   Calibrations[CHARGE_CURRENT_ON_512_PREVALUE_INDEX] = CHARGE_CURRENT_ON_512_PREVALUE; // 1211
   Calibrations[CHARGE_CURRENT_ON_768_PREVALUE_INDEX] = CHARGE_CURRENT_ON_768_PREVALUE; // 1755
   Calibrations[CHARGE_CURRENT_ON_1024_PREVALUE_INDEX] = CHARGE_CURRENT_ON_1024_PREVALUE; // 2292
   Calibrations[CHARGE_CURRENT_ON_1280_PREVALUE_INDEX] = CHARGE_CURRENT_ON_1280_PREVALUE; // 3120
   Calibrations[CHARGE_CURRENT_ON_1536_PREVALUE_INDEX] = CHARGE_CURRENT_ON_1536_PREVALUE; // 3533
   Calibrations[CHARGE_CURRENT_ON_1792_PREVALUE_INDEX] = CHARGE_CURRENT_ON_1792_PREVALUE; // 4048
   Calibrations[CHARGE_CURRENT_ON_2048_PREVALUE_INDEX] = CHARGE_CURRENT_ON_2048_PREVALUE; // 4567
   Calibrations[CHARGE_CURRENT_ON_2304_PREVALUE_INDEX] = CHARGE_CURRENT_ON_2304_PREVALUE; // 5083
   Calibrations[CHARGE_CURRENT_ON_2560_PREVALUE_INDEX] = CHARGE_CURRENT_ON_2560_PREVALUE; // 5596
   Calibrations[CHARGE_CURRENT_ON_2816_PREVALUE_INDEX] = CHARGE_CURRENT_ON_2816_PREVALUE; // 6111
   Calibrations[CHARGE_CURRENT_ON_3072_PREVALUE_INDEX] = CHARGE_CURRENT_ON_3072_PREVALUE; // 6627
   Calibrations[CHARGE_CURRENT_ON_3328_PREVALUE_INDEX] = CHARGE_CURRENT_ON_3328_PREVALUE; // 7140
   Calibrations[CHARGE_CURRENT_ON_3584_PREVALUE_INDEX] = CHARGE_CURRENT_ON_3584_PREVALUE; // 7658
   Calibrations[CHARGE_CURRENT_ON_3840_PREVALUE_INDEX] = CHARGE_CURRENT_ON_3840_PREVALUE; // 8176
   Calibrations[CHARGE_CURRENT_ON_4096_PREVALUE_INDEX] = CHARGE_CURRENT_ON_4096_PREVALUE; // 8668

   Calibrations[BATTERY_VOLTAGE_ON_0_PREVALUE_INDEX] = BATTERY_VOLTAGE_ON_0_PREVALUE; // 0
   Calibrations[BATTERY_VOLTAGE_ON_256_PREVALUE_INDEX] = BATTERY_VOLTAGE_ON_256_PREVALUE; // 298
   Calibrations[BATTERY_VOLTAGE_ON_512_PREVALUE_INDEX] = BATTERY_VOLTAGE_ON_512_PREVALUE; // 549
   Calibrations[BATTERY_VOLTAGE_ON_768_PREVALUE_INDEX] = BATTERY_VOLTAGE_ON_768_PREVALUE; // 798
   Calibrations[BATTERY_VOLTAGE_ON_1024_PREVALUE_INDEX] = BATTERY_VOLTAGE_ON_1024_PREVALUE; // 1048
   Calibrations[BATTERY_VOLTAGE_ON_1280_PREVALUE_INDEX] = BATTERY_VOLTAGE_ON_1280_PREVALUE; // 1299
   Calibrations[BATTERY_VOLTAGE_ON_1536_PREVALUE_INDEX] = BATTERY_VOLTAGE_ON_1536_PREVALUE; // 1548
   Calibrations[BATTERY_VOLTAGE_ON_1792_PREVALUE_INDEX] = BATTERY_VOLTAGE_ON_1792_PREVALUE; // 1798
   Calibrations[BATTERY_VOLTAGE_ON_2048_PREVALUE_INDEX] = BATTERY_VOLTAGE_ON_2048_PREVALUE; // 2046
   Calibrations[BATTERY_VOLTAGE_ON_2304_PREVALUE_INDEX] = BATTERY_VOLTAGE_ON_2304_PREVALUE; // 2296
   Calibrations[BATTERY_VOLTAGE_ON_2560_PREVALUE_INDEX] = BATTERY_VOLTAGE_ON_2560_PREVALUE; // 2545
   Calibrations[BATTERY_VOLTAGE_ON_2816_PREVALUE_INDEX] = BATTERY_VOLTAGE_ON_2816_PREVALUE; // 2794
   Calibrations[BATTERY_VOLTAGE_ON_3072_PREVALUE_INDEX] = BATTERY_VOLTAGE_ON_3072_PREVALUE; // 3044
   Calibrations[BATTERY_VOLTAGE_ON_3328_PREVALUE_INDEX] = BATTERY_VOLTAGE_ON_3328_PREVALUE; // 3298
   Calibrations[BATTERY_VOLTAGE_ON_3584_PREVALUE_INDEX] = BATTERY_VOLTAGE_ON_3584_PREVALUE; // 3549
   Calibrations[BATTERY_VOLTAGE_ON_3840_PREVALUE_INDEX] = BATTERY_VOLTAGE_ON_3840_PREVALUE; // 3798
   Calibrations[BATTERY_VOLTAGE_ON_4096_PREVALUE_INDEX] = BATTERY_VOLTAGE_ON_4096_PREVALUE; // 4040

   Calibrations[DISCHARGE_CURRENT_ON_0_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_0_PREVALUE; // 0
   Calibrations[DISCHARGE_CURRENT_ON_32_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_32_PREVALUE; // 44
   Calibrations[DISCHARGE_CURRENT_ON_64_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_64_PREVALUE; // 88
   Calibrations[DISCHARGE_CURRENT_ON_96_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_96_PREVALUE; // 132
   Calibrations[DISCHARGE_CURRENT_ON_128_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_128_PREVALUE; // 176
   Calibrations[DISCHARGE_CURRENT_ON_160_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_160_PREVALUE; // 220
   Calibrations[DISCHARGE_CURRENT_ON_192_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_192_PREVALUE; // 264
   Calibrations[DISCHARGE_CURRENT_ON_224_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_224_PREVALUE; // 308
   Calibrations[DISCHARGE_CURRENT_ON_256_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_256_PREVALUE; // 352
   Calibrations[DISCHARGE_CURRENT_ON_288_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_288_PREVALUE; // 397
   Calibrations[DISCHARGE_CURRENT_ON_320_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_320_PREVALUE; // 441
   Calibrations[DISCHARGE_CURRENT_ON_352_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_352_PREVALUE; // 485
   Calibrations[DISCHARGE_CURRENT_ON_384_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_384_PREVALUE; // 529
   Calibrations[DISCHARGE_CURRENT_ON_416_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_416_PREVALUE; // 573
   Calibrations[DISCHARGE_CURRENT_ON_448_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_448_PREVALUE; // 617
   Calibrations[DISCHARGE_CURRENT_ON_480_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_480_PREVALUE; // 661
   Calibrations[DISCHARGE_CURRENT_ON_512_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_512_PREVALUE; // 643
   Calibrations[DISCHARGE_CURRENT_ON_768_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_768_PREVALUE; // 1754
   Calibrations[DISCHARGE_CURRENT_ON_1024_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_1024_PREVALUE; // 2311
   Calibrations[DISCHARGE_CURRENT_ON_1280_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_1280_PREVALUE; // 3101
   Calibrations[DISCHARGE_CURRENT_ON_1536_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_1536_PREVALUE; // 3442
   Calibrations[DISCHARGE_CURRENT_ON_1792_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_1792_PREVALUE; // 4004
   Calibrations[DISCHARGE_CURRENT_ON_2048_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_2048_PREVALUE; // 4572
   Calibrations[DISCHARGE_CURRENT_ON_2304_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_2304_PREVALUE; // 5136
   Calibrations[DISCHARGE_CURRENT_ON_2560_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_2560_PREVALUE; // 5698
   Calibrations[DISCHARGE_CURRENT_ON_2816_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_2816_PREVALUE; // 6266
   Calibrations[DISCHARGE_CURRENT_ON_3072_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_3072_PREVALUE; // 6831
   Calibrations[DISCHARGE_CURRENT_ON_3328_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_3328_PREVALUE; // 7390
   Calibrations[DISCHARGE_CURRENT_ON_3584_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_3584_PREVALUE; // 7955
   Calibrations[DISCHARGE_CURRENT_ON_3840_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_3840_PREVALUE; // 8515
   Calibrations[DISCHARGE_CURRENT_ON_4096_PREVALUE_INDEX] = DISCHARGE_CURRENT_ON_4096_PREVALUE; // 9050

   Calibrations[RADIATOR_TEMPERATURE_ON_0_PREVALUE_INDEX] = RADIATOR_TEMPERATURE_ON_0_PREVALUE; // -32059
   Calibrations[RADIATOR_TEMPERATURE_ON_256_PREVALUE_INDEX] = RADIATOR_TEMPERATURE_ON_256_PREVALUE; // -29157
   Calibrations[RADIATOR_TEMPERATURE_ON_512_PREVALUE_INDEX] = RADIATOR_TEMPERATURE_ON_512_PREVALUE; // -26256
   Calibrations[RADIATOR_TEMPERATURE_ON_768_PREVALUE_INDEX] = RADIATOR_TEMPERATURE_ON_768_PREVALUE; // -23355
   Calibrations[RADIATOR_TEMPERATURE_ON_1024_PREVALUE_INDEX] = RADIATOR_TEMPERATURE_ON_1024_PREVALUE; // -20453
   Calibrations[RADIATOR_TEMPERATURE_ON_1280_PREVALUE_INDEX] = RADIATOR_TEMPERATURE_ON_1280_PREVALUE; // -17552
   Calibrations[RADIATOR_TEMPERATURE_ON_1536_PREVALUE_INDEX] = RADIATOR_TEMPERATURE_ON_1536_PREVALUE; // -14651
   Calibrations[RADIATOR_TEMPERATURE_ON_1792_PREVALUE_INDEX] = RADIATOR_TEMPERATURE_ON_1792_PREVALUE; // -11749
   Calibrations[RADIATOR_TEMPERATURE_ON_2048_PREVALUE_INDEX] = RADIATOR_TEMPERATURE_ON_2048_PREVALUE; // -8848
   Calibrations[RADIATOR_TEMPERATURE_ON_2304_PREVALUE_INDEX] = RADIATOR_TEMPERATURE_ON_2304_PREVALUE; // -5947
   Calibrations[RADIATOR_TEMPERATURE_ON_2560_PREVALUE_INDEX] = RADIATOR_TEMPERATURE_ON_2560_PREVALUE; // -3045
   Calibrations[RADIATOR_TEMPERATURE_ON_2816_PREVALUE_INDEX] = RADIATOR_TEMPERATURE_ON_2816_PREVALUE; // -144
   Calibrations[RADIATOR_TEMPERATURE_ON_3072_PREVALUE_INDEX] = RADIATOR_TEMPERATURE_ON_3072_PREVALUE; // 2756
   Calibrations[RADIATOR_TEMPERATURE_ON_3328_PREVALUE_INDEX] = RADIATOR_TEMPERATURE_ON_3328_PREVALUE; // 5658
   Calibrations[RADIATOR_TEMPERATURE_ON_3584_PREVALUE_INDEX] = RADIATOR_TEMPERATURE_ON_3584_PREVALUE; // 8559
   Calibrations[RADIATOR_TEMPERATURE_ON_3840_PREVALUE_INDEX] = RADIATOR_TEMPERATURE_ON_3840_PREVALUE; // 11460
   Calibrations[RADIATOR_TEMPERATURE_ON_4096_PREVALUE_INDEX] = RADIATOR_TEMPERATURE_ON_4096_PREVALUE; // 14362

   Calibrations[SOURCE_DC_CONVERTER_VOLTAGE_ON_0_PREVALUE_INDEX] = SOURCE_DC_CONVERTER_VOLTAGE_ON_0_PREVALUE; // 76
   Calibrations[SOURCE_DC_CONVERTER_VOLTAGE_ON_256_PREVALUE_INDEX] = SOURCE_DC_CONVERTER_VOLTAGE_ON_256_PREVALUE; // 507
   Calibrations[SOURCE_DC_CONVERTER_VOLTAGE_ON_512_PREVALUE_INDEX] = SOURCE_DC_CONVERTER_VOLTAGE_ON_512_PREVALUE; // 933
   Calibrations[SOURCE_DC_CONVERTER_VOLTAGE_ON_768_PREVALUE_INDEX] = SOURCE_DC_CONVERTER_VOLTAGE_ON_768_PREVALUE; // 1360
   Calibrations[SOURCE_DC_CONVERTER_VOLTAGE_ON_1024_PREVALUE_INDEX] = SOURCE_DC_CONVERTER_VOLTAGE_ON_1024_PREVALUE; // 1789
   Calibrations[SOURCE_DC_CONVERTER_VOLTAGE_ON_1280_PREVALUE_INDEX] = SOURCE_DC_CONVERTER_VOLTAGE_ON_1280_PREVALUE; // 2216
   Calibrations[SOURCE_DC_CONVERTER_VOLTAGE_ON_1536_PREVALUE_INDEX] = SOURCE_DC_CONVERTER_VOLTAGE_ON_1536_PREVALUE; // 2643
   Calibrations[SOURCE_DC_CONVERTER_VOLTAGE_ON_1792_PREVALUE_INDEX] = SOURCE_DC_CONVERTER_VOLTAGE_ON_1792_PREVALUE; // 3070
   Calibrations[SOURCE_DC_CONVERTER_VOLTAGE_ON_2048_PREVALUE_INDEX] = SOURCE_DC_CONVERTER_VOLTAGE_ON_2048_PREVALUE; // 3496
   Calibrations[SOURCE_DC_CONVERTER_VOLTAGE_ON_2304_PREVALUE_INDEX] = SOURCE_DC_CONVERTER_VOLTAGE_ON_2304_PREVALUE; // 3923
   Calibrations[SOURCE_DC_CONVERTER_VOLTAGE_ON_2560_PREVALUE_INDEX] = SOURCE_DC_CONVERTER_VOLTAGE_ON_2560_PREVALUE; // 4350
   Calibrations[SOURCE_DC_CONVERTER_VOLTAGE_ON_2816_PREVALUE_INDEX] = SOURCE_DC_CONVERTER_VOLTAGE_ON_2816_PREVALUE; // 4775
   Calibrations[SOURCE_DC_CONVERTER_VOLTAGE_ON_3072_PREVALUE_INDEX] = SOURCE_DC_CONVERTER_VOLTAGE_ON_3072_PREVALUE; // 5203
   Calibrations[SOURCE_DC_CONVERTER_VOLTAGE_ON_3328_PREVALUE_INDEX] = SOURCE_DC_CONVERTER_VOLTAGE_ON_3328_PREVALUE; // 5629
   Calibrations[SOURCE_DC_CONVERTER_VOLTAGE_ON_3584_PREVALUE_INDEX] = SOURCE_DC_CONVERTER_VOLTAGE_ON_3584_PREVALUE; // 6055
   Calibrations[SOURCE_DC_CONVERTER_VOLTAGE_ON_3840_PREVALUE_INDEX] = SOURCE_DC_CONVERTER_VOLTAGE_ON_3840_PREVALUE; // 6481
   Calibrations[SOURCE_DC_CONVERTER_VOLTAGE_ON_4096_PREVALUE_INDEX] = SOURCE_DC_CONVERTER_VOLTAGE_ON_4096_PREVALUE; // 6907

   Calibrations[NETWORK_VOLTAGE_ON_0_PREVALUE_INDEX] = NETWORK_VOLTAGE_ON_0_PREVALUE; // 76
   Calibrations[NETWORK_VOLTAGE_ON_256_PREVALUE_INDEX] = NETWORK_VOLTAGE_ON_256_PREVALUE; // 507
   Calibrations[NETWORK_VOLTAGE_ON_512_PREVALUE_INDEX] = NETWORK_VOLTAGE_ON_512_PREVALUE; // 933
   Calibrations[NETWORK_VOLTAGE_ON_768_PREVALUE_INDEX] = NETWORK_VOLTAGE_ON_768_PREVALUE; // 1360
   Calibrations[NETWORK_VOLTAGE_ON_1024_PREVALUE_INDEX] = NETWORK_VOLTAGE_ON_1024_PREVALUE; // 1789
   Calibrations[NETWORK_VOLTAGE_ON_1280_PREVALUE_INDEX] = NETWORK_VOLTAGE_ON_1280_PREVALUE; // 2216
   Calibrations[NETWORK_VOLTAGE_ON_1536_PREVALUE_INDEX] = NETWORK_VOLTAGE_ON_1536_PREVALUE; // 2643
   Calibrations[NETWORK_VOLTAGE_ON_1792_PREVALUE_INDEX] = NETWORK_VOLTAGE_ON_1792_PREVALUE; // 3070
   Calibrations[NETWORK_VOLTAGE_ON_2048_PREVALUE_INDEX] = NETWORK_VOLTAGE_ON_2048_PREVALUE; // 3496
   Calibrations[NETWORK_VOLTAGE_ON_2304_PREVALUE_INDEX] = NETWORK_VOLTAGE_ON_2304_PREVALUE; // 3923
   Calibrations[NETWORK_VOLTAGE_ON_2560_PREVALUE_INDEX] = NETWORK_VOLTAGE_ON_2560_PREVALUE; // 4350
   Calibrations[NETWORK_VOLTAGE_ON_2816_PREVALUE_INDEX] = NETWORK_VOLTAGE_ON_2816_PREVALUE; // 4775
   Calibrations[NETWORK_VOLTAGE_ON_3072_PREVALUE_INDEX] = NETWORK_VOLTAGE_ON_3072_PREVALUE; // 5203
   Calibrations[NETWORK_VOLTAGE_ON_3328_PREVALUE_INDEX] = NETWORK_VOLTAGE_ON_3328_PREVALUE; // 5629
   Calibrations[NETWORK_VOLTAGE_ON_3584_PREVALUE_INDEX] = NETWORK_VOLTAGE_ON_3584_PREVALUE; // 6055
   Calibrations[NETWORK_VOLTAGE_ON_3840_PREVALUE_INDEX] = NETWORK_VOLTAGE_ON_3840_PREVALUE; // 6481
   Calibrations[NETWORK_VOLTAGE_ON_4096_PREVALUE_INDEX] = NETWORK_VOLTAGE_ON_4096_PREVALUE; // 6907
   
   Calibrations[PAYLOAD_RESISTANCE_ON_0_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_0_SETTING; // 0
   Calibrations[PAYLOAD_RESISTANCE_ON_16_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_16_SETTING; // 44
   Calibrations[PAYLOAD_RESISTANCE_ON_32_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_32_SETTING; // 88
   Calibrations[PAYLOAD_RESISTANCE_ON_48_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_48_SETTING; // 132
   Calibrations[PAYLOAD_RESISTANCE_ON_64_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_64_SETTING; // 176
   Calibrations[PAYLOAD_RESISTANCE_ON_80_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_80_SETTING; // 220
   Calibrations[PAYLOAD_RESISTANCE_ON_96_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_96_SETTING; // 264
   Calibrations[PAYLOAD_RESISTANCE_ON_112_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_112_SETTING; // 308
   Calibrations[PAYLOAD_RESISTANCE_ON_128_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_128_SETTING; // 352
   Calibrations[PAYLOAD_RESISTANCE_ON_144_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_144_SETTING; // 397
   Calibrations[PAYLOAD_RESISTANCE_ON_160_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_160_SETTING; // 441
   Calibrations[PAYLOAD_RESISTANCE_ON_176_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_176_SETTING; // 485
   Calibrations[PAYLOAD_RESISTANCE_ON_192_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_192_SETTING; // 529
   Calibrations[PAYLOAD_RESISTANCE_ON_208_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_208_SETTING; // 573
   Calibrations[PAYLOAD_RESISTANCE_ON_224_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_224_SETTING; // 617
   Calibrations[PAYLOAD_RESISTANCE_ON_240_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_240_SETTING; // 661
   Calibrations[PAYLOAD_RESISTANCE_ON_256_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_256_SETTING; // 643
   Calibrations[PAYLOAD_RESISTANCE_ON_512_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_512_SETTING; // 1185
   Calibrations[PAYLOAD_RESISTANCE_ON_768_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_768_SETTING; // 1754
   Calibrations[PAYLOAD_RESISTANCE_ON_1024_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_1024_SETTING; // 2311
   Calibrations[PAYLOAD_RESISTANCE_ON_1280_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_1280_SETTING; // 3101
   Calibrations[PAYLOAD_RESISTANCE_ON_1536_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_1536_SETTING; // 3442
   Calibrations[PAYLOAD_RESISTANCE_ON_1792_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_1792_SETTING; // 4004
   Calibrations[PAYLOAD_RESISTANCE_ON_2048_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_2048_SETTING; // 4572
   Calibrations[PAYLOAD_RESISTANCE_ON_2304_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_2304_SETTING; // 5136
   Calibrations[PAYLOAD_RESISTANCE_ON_2560_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_2560_SETTING; // 5698
   Calibrations[PAYLOAD_RESISTANCE_ON_2816_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_2816_SETTING; // 6266
   Calibrations[PAYLOAD_RESISTANCE_ON_3072_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_3072_SETTING; // 6831
   Calibrations[PAYLOAD_RESISTANCE_ON_3328_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_3328_SETTING; // 7390
   Calibrations[PAYLOAD_RESISTANCE_ON_3584_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_3584_SETTING; // 7955
   Calibrations[PAYLOAD_RESISTANCE_ON_3840_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_3840_SETTING; // 8515
   Calibrations[PAYLOAD_RESISTANCE_ON_4096_SETTING_INDEX] = PAYLOAD_RESISTANCE_ON_4096_SETTING; // 9050
   
   Calibrations[WIRES_RESISTANCE_INDEX] = (int32_t)(1000*WIRES_RESISTANCE);
   
}

void InitCalibrations () {
  // SetDefaultCalibrations ();
  // AssignWithEEPROM ( &(Calibrations[0][0]), NUM_OF_PARAMETERS * NUM_OF_CALIBRATIONS, SetDefaultCalibrations );
}

void AnalyzeCalibrations () {
}

void DispatchCalibrations () {
}

void SaveCalibrations ( ) {
  UpdateInEEPROM ( Calibrations );
}

void ChangeCalibration ( uint32_t MeasurementIndex, int32_t PreValueIndex, int32_t Value ) {
  Calibrations[MeasurementIndex*17+PreValueIndex] = Value;
}