/* Требования к модулю:
Реализованные:
1. Преобразование сигналов от датчиков в численные значения параметров
2. Для каждого параметра усреднение параметра по методу скользящего среднего с выбранным шагом усреднения

Реализовать:
1. Для каждого параметра установка или снятие флага установившегося режима
2. Для каждого параметра установка или отключение аварий при выходе значения параметра за допустимые границы
3. Для каждого параметра возможность изменения шага усреднения от усреднения по 1 до усреднения по 50 значениям в процессе работы при изменении соотв калибровочного параметра
4. Для каждого параметра определение максимального за последнюю секунду значения
5. Для каждого параметра определение минимального за последнюю секунду значения
6. Для каждого параметра определение максимального за последнюю минуту значения
7. Для каждого параметра определение минимального за последнюю минуту значения
8. Для каждого параметра определение максимального за последние N минут значения
9. Для каждого параметра определение минимального за последние N минут значения
10. Изменение числа N минут, за которые определеются минимальное и максимальное значения
*/

#define __MEASUREMENTS_C__

#include "main.h"
#include "Faults.h"
#include "Measurements.h"
#include "References.h"
#include "Calibrations.h"
#include "TimeControl.h"
#include "Board.h"

int32_t Values [NUM_OF_PARAMETERS];
int32_t PreValues [NUM_OF_PARAMETERS][MAX_NUM_OF_MEASUREMENTS];
int32_t PreValuesSum [NUM_OF_PARAMETERS];
int32_t NormedPreValues[NUM_OF_PARAMETERS];
int32_t PreValuesIndexes[NUM_OF_PARAMETERS];
int32_t NumOfMeasures[NUM_OF_PARAMETERS];

int32_t* LittleChargeCurrents = &(Calibrations[CHARGE_CURRENT_ON_0_PREVALUE_INDEX]);
int32_t* BigChargeCurrents = &(Calibrations[CHARGE_CURRENT_ON_256_PREVALUE_INDEX]);
int32_t* BatteryVoltages = &(Calibrations[BATTERY_VOLTAGE_ON_0_PREVALUE_INDEX]);
int32_t* LittleDischargeCurrents = &(Calibrations[DISCHARGE_CURRENT_ON_0_PREVALUE_INDEX]);
int32_t* BigDischargeCurrents = &(Calibrations[DISCHARGE_CURRENT_ON_512_PREVALUE_INDEX]);
int32_t* RadiatorTemperatures = &(Calibrations[RADIATOR_TEMPERATURE_ON_0_PREVALUE_INDEX]);
int32_t* SourceDCConverterVoltages = &(Calibrations[SOURCE_DC_CONVERTER_VOLTAGE_ON_0_PREVALUE_INDEX]);
int32_t* NetworkVoltages = &(Calibrations[NETWORK_VOLTAGE_ON_0_PREVALUE_INDEX]);

uint64_t Tmp1 = (uint64_t)96000000 * (uint64_t)6000;    // Константа, используемая для вычисления частоты

int32_t NumOfChargeCurrentMeasuresForAverage = MAX_NUM_OF_CHARGE_CURRENT_MEASURES_FOR_AVERAGE;
int32_t NumOfBatteryVoltageMeasuresForAverage = MAX_NUM_OF_BATTERY_VOLTAGE_MEASURES_FOR_AVERAGE;
int32_t NumOfDischargeCurrentMeasuresForAverage = MAX_NUM_OF_DISCHARGE_CURRENT_MEASURES_FOR_AVERAGE;
int32_t NumOfRadiatorTemperatureMeasuresForAverage = MAX_NUM_OF_RADIATOR_TEMPERATURE_MEASURES_FOR_AVERAGE;
int32_t NumOfSourceDCConverterVoltageMeasuresForAverage = MAX_NUM_OF_SOURCE_DC_CONVERTER_VOLTAGE_MEASURES_FOR_AVERAGE;
int32_t NumOfNetworkVoltageMeasuresForAverage = MAX_NUM_OF_NETWORK_VOLTAGE_MEASURES_FOR_AVERAGE;

void InitMeasurements ( void ) {
   int32_t i, j;
   for ( i = 0; i < NUM_OF_PARAMETERS; i++ ) {
     for ( j = 0; j < MAX_NUM_OF_MEASUREMENTS; j++ ) {
       PreValues[i][j] = 0;
     }
     Values[i] = 0;
     NormedPreValues[i] = 0;
     PreValuesIndexes[i] = 0;
     PreValuesSum[i] = 0;
   }
}

uint32_t FrequenciesWasUpdated = 1;

void DispatchMeasurements ( void ) {
  uint32_t Index;
}

uint64_t LastSourceVoltageMeasureTimeStamp = (uint64_t) 0;
uint64_t LastCapacitorsVoltageMeasureTimeStamp = (uint64_t) 0;
uint64_t LastFreqSensorVoltageMeasureTimeStamp = (uint64_t) 0;
uint64_t LastGlobalCurrentMeasureTimeStamp = (uint64_t) 0;

uint64_t tmpMeasurementsInterval = 0;

void AnalyzeMeasurements ( void ) {
}
/*
void RegisterMeasure ( uint32_t MeasureIndex, uint32_t Value ) {
}*/

//*
void RegisterMeasure ( uint32_t MeasureIndex, uint32_t Value ) {
  if ( MeasureIndex < NUM_OF_PARAMETERS ) {
    int32_t MinPreValueValueIndex, MaxPreValueValueIndex, DeltaPreValue, tmpPreValue;
    PreValuesSum[MeasureIndex] -= PreValues[MeasureIndex][PreValuesIndexes[MeasureIndex]];
    PreValues[MeasureIndex][PreValuesIndexes[MeasureIndex]] = Value;
    PreValuesSum[MeasureIndex] += Value;
    tmpPreValue = PreValuesSum[MeasureIndex]/MAX_NUM_OF_MEASUREMENTS;
    switch ( MeasureIndex ) {
      case CHARGE_CURRENT:
        if ( tmpPreValue < 256 ) {
          MinPreValueValueIndex = tmpPreValue/16;
          MaxPreValueValueIndex = MinPreValueValueIndex + 1;
          DeltaPreValue = tmpPreValue%16;
          Values[MeasureIndex] = LittleChargeCurrents[MinPreValueValueIndex] + DeltaPreValue * ( LittleChargeCurrents[MaxPreValueValueIndex]-LittleChargeCurrents[MinPreValueValueIndex] ) / 16;
        }
        else {
          MinPreValueValueIndex = tmpPreValue/256-1;
          MaxPreValueValueIndex = MinPreValueValueIndex + 1;
          DeltaPreValue = tmpPreValue%256;
          Values[MeasureIndex] = BigChargeCurrents[MinPreValueValueIndex] + DeltaPreValue * ( BigChargeCurrents[MaxPreValueValueIndex]-BigChargeCurrents[MinPreValueValueIndex] ) / 256;
        }
        break;
      case BATTERY_VOLTAGE:
        MinPreValueValueIndex = tmpPreValue/256;
        MaxPreValueValueIndex = MinPreValueValueIndex + 1;
        DeltaPreValue = tmpPreValue%256;
        Values[MeasureIndex] = BatteryVoltages[MinPreValueValueIndex] + DeltaPreValue * ( BatteryVoltages[MaxPreValueValueIndex] - BatteryVoltages[MinPreValueValueIndex] ) / 256;
        Values[BATTERY_VOLTAGE] += ( Values[DISCHARGE_CURRENT] - Values[CHARGE_CURRENT] ) * Calibrations [WIRES_RESISTANCE_INDEX] / 1000;
			  if ( Values[BATTERY_VOLTAGE] < 0 ) { Values[BATTERY_VOLTAGE] = 0; }
        break;
      case DISCHARGE_CURRENT:
        if ( tmpPreValue < 512 ) {
          MinPreValueValueIndex = tmpPreValue/32;
          MaxPreValueValueIndex = MinPreValueValueIndex + 1;
          DeltaPreValue = tmpPreValue%32;
          Values[MeasureIndex] = LittleDischargeCurrents[MinPreValueValueIndex] + DeltaPreValue * ( LittleDischargeCurrents[MaxPreValueValueIndex]-LittleDischargeCurrents[MinPreValueValueIndex] ) / 32;
        }
        else {
          MinPreValueValueIndex = tmpPreValue/256 - 2;
          MaxPreValueValueIndex = MinPreValueValueIndex + 1;
          DeltaPreValue = tmpPreValue%256;
          Values[MeasureIndex] = BigDischargeCurrents[MinPreValueValueIndex] + DeltaPreValue * ( BigDischargeCurrents[MaxPreValueValueIndex]-BigDischargeCurrents[MinPreValueValueIndex] ) / 256;
        }
        break;
      case RADIATOR_TEMPERATURE:
        MinPreValueValueIndex = tmpPreValue/256;
        MaxPreValueValueIndex = MinPreValueValueIndex + 1;
        DeltaPreValue = tmpPreValue%256;
        Values[MeasureIndex] = RadiatorTemperatures[MinPreValueValueIndex] + DeltaPreValue * ( RadiatorTemperatures[MaxPreValueValueIndex]-RadiatorTemperatures[MinPreValueValueIndex] ) / 256;
        break;
      case SOURCE_DC_CONVERTER_VOLTAGE:
        MinPreValueValueIndex = tmpPreValue/256;
        MaxPreValueValueIndex = MinPreValueValueIndex + 1;
        DeltaPreValue = tmpPreValue%256;
        Values[MeasureIndex] = SourceDCConverterVoltages[MinPreValueValueIndex] + DeltaPreValue * ( SourceDCConverterVoltages[MaxPreValueValueIndex]-SourceDCConverterVoltages[MinPreValueValueIndex] ) / 256;
        break;
      case NETWORK_VOLTAGE:
        MinPreValueValueIndex = tmpPreValue/256;
        MaxPreValueValueIndex = MinPreValueValueIndex + 1;
        DeltaPreValue = tmpPreValue%256;
        Values[NETWORK_VOLTAGE] = NetworkVoltages[MinPreValueValueIndex] + DeltaPreValue * ( NetworkVoltages[MaxPreValueValueIndex]-NetworkVoltages[MinPreValueValueIndex] ) / 256;
        break;
    }
    // Вот здесь должен быть рассчёт значения параметра
    NormedPreValues[MeasureIndex] = tmpPreValue;
    PreValuesIndexes[MeasureIndex]++;
    PreValuesIndexes[MeasureIndex] %= MAX_NUM_OF_MEASUREMENTS;
  }
} //*/

void ChangeNumOfMeasuresForParameterCalculation ( uint32_t MeasureIndex, uint32_t NewNumOfMeasures ) {
}
