#ifndef __MEASUREMENTS_H__
#define __MEASUREMENTS_H__

#include "main.h"

#define MAX_NUM_OF_MEASUREMENTS 50

enum {
  CHARGE_CURRENT,
  BATTERY_VOLTAGE,
  DISCHARGE_CURRENT,
  RADIATOR_TEMPERATURE,
  SOURCE_DC_CONVERTER_VOLTAGE,
  NETWORK_VOLTAGE,
  
  NUM_OF_PARAMETERS
};

typedef struct {
  int32_t Value;                    ///< Значение
  int32_t Dispersion;               ///< Предзначение
  int32_t IsStaticMode;             ///< Зарегистрирован установившийся режим
} Measurement_Info_TypeDef;

typedef struct {
  uint32_t MeasurementID;           ///< Идентификатор измерения
  uint32_t PreValue;                ///< Предзначение параметра
  int32_t Value;                    ///< Значение параметра
} Calibration_Info_TypeDef;

#ifndef __MEASUREMENTS_C__
extern int32_t Measurements[NUM_OF_PARAMETERS];
extern int32_t Values[NUM_OF_PARAMETERS];
extern int32_t PreValues[NUM_OF_PARAMETERS];
extern int32_t PreValuesSum[NUM_OF_PARAMETERS];
extern int32_t NormedPreValues[NUM_OF_PARAMETERS];
#endif

#define SetMeasurementValue(x,y) if ((x)<NUM_OF_PARAMETERS) { Measurements[(x)]=(y); }
#define GetMeasurementValue(x) Values[(x)]

void RegisterEngineImpulse1 ( uint64_t TimeStamp );
void RegisterEngineImpulse2 ( uint64_t TimeStamp );
void RegisterEngineImpulse ( void );
void RegisterFreqx100Measure ( int32_t NewValue );
void RegisterAngleMeasure ( int32_t PreValue );
void RegisterSourceVoltageMeasure ( int32_t PreValue );
void RegisterCapacitorsVoltageMeasure ( int32_t PreValue );
void RegisterFreqSensorVoltageMeasure ( int32_t PreValue );
void RegisterCurrentMeasure ( int32_t PreValue );
uint32_t GetMeasurementPreValue ( uint32_t MeasurementID );

void ChangeNumOfSourceVoltageMeasuresForAverage ( uint32_t NewNumOf );
void ChangeNumOfCapacitorsVoltageMeasuresForAverage ( uint32_t NewNumOf );
void ChangeNumOfCurrentMeasuresForAverage ( uint32_t NewValue );
void ChangeNumOfAngleSensorVoltageMeasuresForAverage ( uint32_t NewNumOf );
void ChangeNumOfAngleMeasuresForAverage ( uint32_t NewNumOf );
void ChangeNumOfFreqSensorVoltageMeasuresForAverage ( uint32_t NewNumOf );
void ChangeNumOfFreqx100MeasuresForAverage ( int32_t NewValue );
void ChangeNumOfFreqx100DerivativeMeasuresForAverage ( uint32_t NewNumOf );

void InitMeasurements ( void );
void AnalyzeMeasurements ( void );
void DispatchMeasurements ( void );

#endif // __MEASUREMENTS_H__
