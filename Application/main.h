#ifndef __MAIN_H__
#define __MAIN_H__

#include "stdint.h"
///********************************
///****** Параметры массивов ******
///********************************
#define MAX_NUM_OF_CHARGE_CURRENT_MEASURES_FOR_AVERAGE 50                          ///< Массив частот для усреднения
#define MAX_NUM_OF_BATTERY_VOLTAGE_MEASURES_FOR_AVERAGE 50                        ///< Массив производных частоты для усреднения значений производной частоты
#define MAX_NUM_OF_DISCHARGE_CURRENT_MEASURES_FOR_AVERAGE 50                    ///< Массив напряжений питания для усредненния
#define MAX_NUM_OF_RADIATOR_TEMPERATURE_MEASURES_FOR_AVERAGE 50                      ///< Массив напряжений входных конденсаторов для усредения
#define MAX_NUM_OF_SOURCE_DC_CONVERTER_VOLTAGE_MEASURES_FOR_AVERAGE 50  ///< Массив токов РЧВ для усредения
#define MAX_NUM_OF_NETWORK_VOLTAGE_MEASURES_FOR_AVERAGE 50

#define MAX_NUM_OF_MEASUREMENTS 50

#define CPU_CORE_FREQ 96000000
#define DISCHARGE_PWM_SIGNAL_FREQ 10000
#define MAX_DISCHARGE_SIGNAL CPU_CORE_FREQ / DISCHARGE_PWM_SIGNAL_FREQ
#define MIN_DISCHARGE_SIGNAL 100

///********************************************
///****** Уставки измерений по умолчанию ******
#define DEFAULT_TIMEOUT_OF_CAPACITORS_VOLTAGE_MEASURES 960000              ///< Интервал времени между измерениями напряжения конденсаторов
#define DEFAULT_TIMEOUT_OF_SOURCE_VOLTAGE_MEASURES 960000              ///< Интервал времени между измерениями напряжения питания
#define DEFAULT_TIMEOUT_OF_PAYLOAD_CURRENT_MEASURES 960000             ///< Интервал времени между измерениями тока РЧВ
#define DEFAULT_TIMEOUT_OF_FREQ_SENSOR_VOLTAGE_MEASURES 960000        ///< Интервал времени между измерениями напряжения датчика частоты
#define DEFAULT_TIMEOUT_OF_FREQ_DISPERSION_CONTROL_INTERVAL 96000     ///< Интервал времени, за который оценивается дисперсия частоты
#define DEFAULT_TIMEOUT_OF_ANGLE_DISPERSION_CONTROL_INTERVAL 96000    ///< Интервал времени, за который оценивается дисперсия угла
#define DEFAULT_NUM_OF_FREQ_MEASURES_FOR_DISPERSION_CALCULATION 50    ///< Число значений частот, по которым вычисляется дисперсия
#define DEFAULT_NUM_OF_ANGLE_MEASURES_FOR_DISPERSION_CALCULATION 50   ///< Число значений угла, по которым вычисляется дисперсия

///****** Уставки по умолчанию *******////
#define DEFAULT_NUM_OF_IMPULSES_FOR_FREQ_CALCULATION 100              ///< Число импульсов, используемых для получения значения частоты
#define DEFAULT_NUM_OF_FREQ_MEASURES_FOR_AVERAGE 10                   ///< Число значений частот для получения скользящего среднего
#define DEFAULT_NUM_OF_FREQ_DERIVATIVE_MEASURES_FOR_AVERAGE 10        ///< Число значений производных частоты для получения скользящего среднего
#define DEFAULT_NUM_OF_ANGLE_MEASURES_FOR_AVERAGE 10                  ///< Число значений угла для получения скользящего среднего
#define DEFAULT_NUM_OF_SOURCE_VOLTAGE_MEASURES_FOR_AVERAGE 10          ///< Число значений напряжения питания для получения скользящего среднего
#define DEFAULT_NUM_OF_CAPACITORS_VOLTAGE_MEASURES_FOR_AVERAGE 10          ///< Число значений напряжения входных конденсаторов для усредения
#define DEFAULT_NUM_OF_PAYLOAD_CURRENT_MEASURES_FOR_AVERAGE 10         ///< Число значений тока РЧВ для усредения
#define DEFAULT_NUM_OF_FREQ_SENSOR_VOLTAGES_FOR_AVERAGE 10            ///< Число значений напряжения датчика частоты для усреднения
#define DEFAULT_NUM_OF_ANGLE_DERIVATIVES_FOR_AVERAGE 10      ///< Число значений производной угла для получения скользящего среднего
#define DEFAULT_SOLENOID_DIRECTION 0                                  ///< Направление вращения электромагнита. Т.е. сигнал с датчика угла возрастает при увеличении подачи топлива или убывает
#define DEFAULT_SOLENOID_CURRENT_DIRECTION 0                          ///< Направление тока электромагнита. Т.е. какая полярность тока приводит к открытию клапана топлива
#define DEFAULT_MAX_ENGINE_FREQ 1700                                  ///< Аварийная частота вращения двигателя
#define DEFAULT_MAX_ENGINE_FREQ_INTERVAL 960000                       ///< Интервал времени, по истечении которого устанавливается ошибка "Обороты выше нормы"
#define DEFAULT_MIN_ENGINE_FREQ 1300                                  ///< Аварийная минимальная частота вращения двигателя
#define DEFAULT_MIN_ENGINE_FREQ_INTERVAL 960000                       ///< Интервал времени, по истечении которого устанавливается ошибка "Обороты ниже нормы"
#define DEFAULT_NUM_OF_TEETH 125                                      ///< Число зубцов зубчатого венца маховика коленвала
#define DEFAULT_IS_STATISM_ON 0                                       ///< Включен или отключен статизм при включении питания (статизм также может быть включен по CAN или UDP)
#define DEFAULT_TARGET_FREQ 1550                                      ///< Аварийная частота вращения двигателя
#define DEFAULT_MAX_SOURCE_VOLTAGE 30                                  ///< Аварийное напряжение питания, при котором отключается управление и закрывается шунт
#define DEFAULT_MAX_SOURCE_VOLTAGE_INTERVAL 9600000                    ///< Интервал времени, по истечении которого устанавливается ошибка "Напряжение питания выше нормы"
#define DEFAULT_MIN_SOURCE_VOLTAGE 15                                  ///< Аварийное напряжение питания, при котором отключается управление и закрывается шунт
#define DEFAULT_MIN_SOURCE_VOLTAGE_INTERVAL 9600000                    ///< Интервал времени, по истечении которого устанавливается ошибка "Напряжение питания ниже нормы"
#define DEFAULT_CAPACITORS_VOLTAGE_BOUND 15                                ///< Граничное значение напряжения конденсаторов, при котором начинается отсчёт времени для открытия шунта
#define DEFAULT_CAPACITORS_VOLTAGE_BOUND_INTERVAL 0                        ///< Интервал времени после достижения граничного напряжения, по истечении которого входные конденсаторы считаются заряженными
#define DEFAULT_MAX_PAYLOAD_CURRENT 10                                 ///< Максимально допустимое значение тока РЧВ
#define DEFAULT_MAX_PAYLOAD_CURRENT_INTERVAL 96000                     ///< Интервал времени, по истечении которого устанавливается ошибка "КЗ РЧВ"
#define DEFAULT_MAX_FREQ_SENSOR_FAULT_INTERVAL 960000                 ///< Интервал времени, по истечении которого устанавливается ошибка "Обрыв датчика частоты вращения"
#define DEFAULT_MAX_KIANGLE_ACTION 1000                               ///< Максимальное значение интегральной составляющей сигнала управления углом
#define DEFAULT_MAX_KIFREQ_ACTION 1500                                ///< Максимальное значение интегральной составляющей сигнала управлен
#define DEFAULT_INTERVAL_FAULT_ENGINE_FREQ_TOO_LOW 0                  ///< Интервал времени, после которого генерируется авария "Обороты дизеля ниже нормы"
#define DEFAULT_INTERVAL_FAULT_ENGINE_FREQ_TOO_HIGH 0                 ///< Интервал времени, после которого генерируется авария "Обороты дизеля выше нормы"
//#define DEFAULT_NUM_OF_ANGLE_PREVALUES 10                           ///< Количество предзначений угла для усреднения
#define DEFAULT_STATIC_ANGLE_DISPERSION 1000                          ///< Граничное значение дисперсии угла, соответствующее установившемуся режиму
#define DEFAULT_STATIC_FREQ_DISPERSION 1000                           ///< Граничное значение дисперсии частоты, соответствующее установившемуся режиму
#define DEFAULT_BASE_ANGLEX100 80000                                  ///< Угол поворота соленоида, соответствующий холостому ходу
#define DEFAULT_MKU_CAN_ID 10                                         ///< Идентификатор пакетов МКУ в сети CAN (не используется)
#define DEFAULT_R4V_CAN_ID 10                                         ///< Идентификатор пакетов от РЧВ и к РЧВ
#define DEFAULT_CAN_SPEED 512                                         ///< Скорость обмена по интерфейсу CAN
#define DEFAULT_MKU_CAN_MODE 1                                        ///< Режим пакетов МКУ-РЧВ ( обычный или расширенный "РАС." "НОР."
#define DEFAULT_MIN_ANGLE_PREVALUEX100 300                   ///< Минимальное предзначение с датчика угла (если предзначение меньше, то обрыв датчика)
#define DEFAULT_MAX_ANGLE_PREVALUEX100 2000                  ///< Максимальное предзначение с датчика угла (если предзначение больше, то КЗ датчика)
#define DEFAULT_INTERVAL_FAULT_ANGLE_SENSOR_DISCONNECT 960000         ///< Интервал времени, после которого устанавилваеся авария "Обрыв датчика угла"
#define DEFAULT_INTERVAL_FAULT_ANGLE_SENSOR_SHORT_CIRQUIT 960000      ///< Интервал времени, после которого устанавилваеся авария "КЗ датчика угла"

#define POSITIVE 1
#define NEGATIVE -1

#define FALSE 0
#define TRUE 1

#define STATISM_STATE TRUE

#define TELEMETRY_ON FALSE
#define MESSAGES_ON FALSE

#define DEFAULT_CURRENT_DIRECTION POSITIVE // POSITIVE или NEGATIVE


///****** Калибровки по умолчанию (если в EEPROM нет значений)*******/////
///****** Калибровки по умолчанию, используемые для регулирования частоты *******

#define DEFAULT_ANGLEX100_ON_70RPM_MISMATCH 1000
#define DEFAULT_ANGLEX100_ON_65RPM_MISMATCH 950
#define DEFAULT_ANGLEX100_ON_60RPM_MISMATCH 900
#define DEFAULT_ANGLEX100_ON_55RPM_MISMATCH 850
#define DEFAULT_ANGLEX100_ON_50RPM_MISMATCH 800
#define DEFAULT_ANGLEX100_ON_45RPM_MISMATCH 750
#define DEFAULT_ANGLEX100_ON_40RPM_MISMATCH 700
#define DEFAULT_ANGLEX100_ON_35RPM_MISMATCH 650
#define DEFAULT_ANGLEX100_ON_30RPM_MISMATCH 600
#define DEFAULT_ANGLEX100_ON_25RPM_MISMATCH 550
#define DEFAULT_ANGLEX100_ON_20RPM_MISMATCH 500
#define DEFAULT_ANGLEX100_ON_15RPM_MISMATCH 450
#define DEFAULT_ANGLEX100_ON_10RPM_MISMATCH 400
#define DEFAULT_ANGLEX100_ON_5RPM_MISMATCH 350
#define DEFAULT_ANGLEX100_ON_0RPM_MISMATCH 300
#define DEFAULT_ANGLEX100_ON_PLUS_5RPM_MISMATCH 250
#define DEFAULT_ANGLEX100_ON_PLUS_10RPM_MISMATCH 200
#define DEFAULT_ANGLEX100_ON_PLUS_15RPM_MISMATCH 150
#define DEFAULT_ANGLEX100_ON_PLUS_20RPM_MISMATCH 100
#define DEFAULT_ANGLEX100_ON_PLUS_25RPM_MISMATCH 50
#define DEFAULT_ANGLEX100_ON_PLUS_30RPM_MISMATCH 40
#define DEFAULT_ANGLEX100_ON_PLUS_35RPM_MISMATCH 30
#define DEFAULT_ANGLEX100_ON_PLUS_40RPM_MISMATCH 20
#define DEFAULT_ANGLEX100_ON_PLUS_45RPM_MISMATCH 10
#define DEFAULT_ANGLEX100_ON_PLUS_50RPM_MISMATCH 0

#define DEFAULT_KIFREQ1_ANGLEX100 1
#define DEFAULT_KIFREQ1X100 1
#define DEFAULT_KIFREQ2_ANGLEX100 1
#define DEFAULT_KIFREQ2X100 1

#define DEFAULT_KDFREQ1_ANGLEX100 1
#define DEFAULT_KDFREQ1X100 1
#define DEFAULT_KDFREQ2_ANGLEX100 1
#define DEFAULT_KDFREQ2X100 1

///****** Калибровки по умолчанию, используемые для демпфирования регулирования частоты *******
#define DEFAULT_KPFREQ_DEMPF1_ANGLEX100 0
#define DEFAULT_KPFREQ_DEMPF1_DELTAANGLE1X100 0
#define DEFAULT_KPFREQ_DEMPF1_COEFF1 0
#define DEFAULT_KPFREQ_DEMPF1_DELTAANGLE2X100 0
#define DEFAULT_KPFREQ_DEMPF1_COEFF2 0

#define DEFAULT_KPFREQ_DEMPF2_ANGLEX100 0
#define DEFAULT_KPFREQ_DEMPF2_DELTAANGLE1X100 0
#define DEFAULT_KPFREQ_DEMPF2_COEFF1 0
#define DEFAULT_KPFREQ_DEMPF2_DELTAANGLE2X100 0
#define DEFAULT_KPFREQ_DEMPF2_COEFF2 0

#define DEFAULT_KIFREQ_DEMPF1_ANGLEX100 0
#define DEFAULT_KIFREQ_DEMPF1_DELTAANGLE1X100 0
#define DEFAULT_KIFREQ_DEMPF1_COEFF1 0
#define DEFAULT_KIFREQ_DEMPF1_DELTAANGLE2X100 0
#define DEFAULT_KIFREQ_DEMPF1_COEFF2 0

#define DEFAULT_KIFREQ_DEMPF2_ANGLEX100 0
#define DEFAULT_KIFREQ_DEMPF2_DELTAANGLE1X100 0
#define DEFAULT_KIFREQ_DEMPF2_COEFF1 0
#define DEFAULT_KIFREQ_DEMPF2_DELTAANGLE2X100 0
#define DEFAULT_KIFREQ_DEMPF2_COEFF2 0

#define DEFAULT_KDFREQ_DEMPF1_ANGLEX100 0
#define DEFAULT_KDFREQ_DEMPF1_DELTAANGLE1X100 0
#define DEFAULT_KDFREQ_DEMPF1_COEFF1 0
#define DEFAULT_KDFREQ_DEMPF1_DELTAANGLE2X100 0
#define DEFAULT_KDFREQ_DEMPF1_COEFF2 0

#define DEFAULT_KDFREQ_DEMPF2_ANGLEX100 0
#define DEFAULT_KDFREQ_DEMPF2_DELTAANGLE1X100 0
#define DEFAULT_KDFREQ_DEMPF2_COEFF1 0
#define DEFAULT_KDFREQ_DEMPF2_DELTAANGLE2X100 0
#define DEFAULT_KDFREQ_DEMPF2_COEFF2 0

///****** Калибровки по умолчанию, используемые для регулирования угла *******
#define DEFAULT_BASE_CURRENTX100 1400          ///< Удалить, эта калибровка аналогична следующей

#define DEFAULT_CURRENTX100_OF_0_DEGREES 400     ///< Смещение тока для поворота магнита на 0 единиц АЦП
#define DEFAULT_CURRENTX100_OF_100_DEGREES 500
#define DEFAULT_CURRENTX100_OF_200_DEGREES 600
#define DEFAULT_CURRENTX100_OF_300_DEGREES 700
#define DEFAULT_CURRENTX100_OF_400_DEGREES 800
#define DEFAULT_CURRENTX100_OF_500_DEGREES 900
#define DEFAULT_CURRENTX100_OF_600_DEGREES 1000
#define DEFAULT_CURRENTX100_OF_700_DEGREES 1100
#define DEFAULT_CURRENTX100_OF_800_DEGREES 1200
#define DEFAULT_CURRENTX100_OF_900_DEGREES 1300
#define DEFAULT_CURRENTX100_OF_1000_DEGREES 1400
#define DEFAULT_CURRENTX100_OF_1100_DEGREES 1500
#define DEFAULT_CURRENTX100_OF_1200_DEGREES 1600
#define DEFAULT_CURRENTX100_OF_1300_DEGREES 1700
#define DEFAULT_CURRENTX100_OF_1400_DEGREES 1800
#define DEFAULT_CURRENTX100_OF_1500_DEGREES 1900
#define DEFAULT_CURRENTX100_OF_1600_DEGREES 2000
#define DEFAULT_CURRENTX100_OF_1700_DEGREES 2200
#define DEFAULT_CURRENTX100_OF_1800_DEGREES 2300
#define DEFAULT_CURRENTX100_OF_1900_DEGREES 2400
#define DEFAULT_CURRENTX100_OF_2000_DEGREES 2500
#define DEFAULT_CURRENTX100_OF_2100_DEGREES 2600
#define DEFAULT_CURRENTX100_OF_2200_DEGREES 2700
#define DEFAULT_CURRENTX100_OF_2300_DEGREES 2800
#define DEFAULT_CURRENTX100_OF_2400_DEGREES 2900

#define DEFAULT_KPANGLE1_DELTAANGLEX100 100
#define DEFAULT_KPANGLE1X100 29
#define DEFAULT_KPANGLE2_DELTAANGLEX100 1000
#define DEFAULT_KPANGLE2X100 30

#define DEFAULT_KIANGLE1_ANGLEX100 0
#define DEFAULT_KIANGLE1X100 0
#define DEFAULT_KIANGLE2_ANGLEX100 0
#define DEFAULT_KIANGLE2X100 0

#define DEFAULT_KDANGLE1_ANGLEX100 0
#define DEFAULT_KDANGLE1X100 0
#define DEFAULT_KDANGLE2_ANGLEX100 0
#define DEFAULT_KDANGLE2X100 0

///****** Калибровки по умолчанию, используемые для демпфирования регулирования угла ******
#define DEFAULT_ANGLE_DEMPF1_ANGLEX100 0
#define DEFAULT_ANGLE_DEMPF1_DELTAANGLE1X100 0
#define DEFAULT_ANGLE_DEMPF1_COEFF1 0
#define DEFAULT_ANGLE_DEMPF1_DELTAANGLE2X100 0
#define DEFAULT_ANGLE_DEMPF1_COEFF2 0

#define DEFAULT_ANGLE_DEMPF2_ANGLEX100 0
#define DEFAULT_ANGLE_DEMPF2_DELTAANGLE1X100 0
#define DEFAULT_ANGLE_DEMPF2_COEFF1 0
#define DEFAULT_ANGLE_DEMPF2_DELTAANGLE2X100 0
#define DEFAULT_ANGLE_DEMPF2_COEFF2 0

#define DEFAULT_KPANGLE_DEMPF1_ANGLEX100 0
#define DEFAULT_KPANGLE_DEMPF1_DELTACURRENT1X100 0
#define DEFAULT_KPANGLE_DEMPF1_COEFF1 0
#define DEFAULT_KPANGLE_DEMPF1_DELTACURRENT2X100 0
#define DEFAULT_KPANGLE_DEMPF1_COEFF2 0

#define DEFAULT_KPANGLE_DEMPF2_ANGLEX100 0
#define DEFAULT_KPANGLE_DEMPF2_DELTACURRENT1X100 0
#define DEFAULT_KPANGLE_DEMPF2_COEFF1 0
#define DEFAULT_KPANGLE_DEMPF2_DELTACURRENT2X100 0
#define DEFAULT_KPANGLE_DEMPF2_COEFF2 0

#define DEFAULT_KIANGLE_DEMPF1_ANGLEX100 0
#define DEFAULT_KIANGLE_DEMPF1_DELTACURRENT1X100 0
#define DEFAULT_KIANGLE_DEMPF1_COEFF1 0
#define DEFAULT_KIANGLE_DEMPF1_DELTACURRENT2X100 0
#define DEFAULT_KIANGLE_DEMPF1_COEFF2 0

#define DEFAULT_KIANGLE_DEMPF2_ANGLEX100 0
#define DEFAULT_KIANGLE_DEMPF2_DELTACURRENT1X100 0
#define DEFAULT_KIANGLE_DEMPF2_COEFF1 0
#define DEFAULT_KIANGLE_DEMPF2_DELTACURRENT2X100 0
#define DEFAULT_KIANGLE_DEMPF2_COEFF2 0

#define DEFAULT_KDANGLE_DEMPF1_ANGLEX100 0
#define DEFAULT_KDANGLE_DEMPF1_DELTACURRENT1X100 0
#define DEFAULT_KDANGLE_DEMPF1_COEFF1 0
#define DEFAULT_KDANGLE_DEMPF1_DELTACURRENT2X100 0
#define DEFAULT_KDANGLE_DEMPF1_COEFF2 0

#define DEFAULT_KDANGLE_DEMPF2_ANGLEX100 0
#define DEFAULT_KDANGLE_DEMPF2_DELTACURRENT1X100 0
#define DEFAULT_KDANGLE_DEMPF2_COEFF1 0
#define DEFAULT_KDANGLE_DEMPF2_DELTACURRENT2X100 0
#define DEFAULT_KDANGLE_DEMPF2_COEFF2 0
///****** Калибровки по умолчанию, используемые для демпфирования заданий тока (напряжения) ******
#define DEFAULT_CURRENT_DEMPF1_CURRENTX100 0
#define DEFAULT_CURRENT_DEMPF1_DELTACURRENT1X100 0
#define DEFAULT_CURRENT_DEMPF1_COEFF1 0
#define DEFAULT_CURRENT_DEMPF1_DELTACURRENT2X100 0
#define DEFAULT_CURRENT_DEMPF1_COEFF2 0

#define DEFAULT_CURRENT_DEMPF2_CURRENTX100 0
#define DEFAULT_CURRENT_DEMPF2_DELTACURRENT1X100 0
#define DEFAULT_CURRENT_DEMPF2_COEFF1 0
#define DEFAULT_CURRENT_DEMPF2_DELTACURRENT2X100 0
#define DEFAULT_CURRENT_DEMPF2_COEFF2 0

#define DEFAULT_SOURCE_VOLTAGE_10V_PREVALUE 1000
#define DEFAULT_SOURCE_VOLTAGE_25V_PREVALUE 1000
#define DEFAULT_SOURCE_VOLTAGE_K 400
#define DEFAULT_SOURCE_VOLTAGE_B 10

#define DEFAULT_CAPACITORS_VOLTAGE_10V_PREVALUE 1000
#define DEFAULT_CAPACITORS_VOLTAGE_25V_PREVALUE 1000
#define DEFAULT_CAPACITORS_VOLTAGE_K 1000
#define DEFAULT_CAPACITORS_VOLTAGE_B 20

#define DEFAULT_PAYLOAD_CURRENT_0A_PREVALUE 1000
#define DEFAULT_PAYLOAD_CURRENT_5A_PREVALUE 1000
#define DEFAULT_ANGLE_BASE_ANGLE_PREVALUE 1000
#define DEFAULT_ANGLE_FULL_ANGLE_PREVALUE 1000

///****** Калибровки по умолчанию, используемые для вычисления параметров *******

//* Коэффициенты регулирования частоты и демпфирования управляющих воздействий *//

#define DEFAULT_MAX_KIFREQ_ACTIONX100 0
#define DEFAULT_MAX_KIANGLE_ACTIONX100 300000

#define DEFAULT_NUM_OF_ANGLE_MEASURES 256
#define DEFAULT_ANGLE_SMOOTH_INTERVAL 4
#define DEFAULT_DERIVATIVE_SMOOTH_INTERVAL 4      // Должно быть меньше числа измерений
#define DEFAULT_DERIVATIVE_CALCULATION_INTERVAL 4

#endif // __MAIN_H__
