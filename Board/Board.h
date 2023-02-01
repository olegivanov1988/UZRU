#ifndef __BOARD_H__
#define __BOARD_H__

#include "stdint.h"

typedef enum {
  NOT_PUSHED,
  PUSHED
} Button_State_TypeDef;

void Board_BeginSmoothStart ( void );                    // При вызове этой функции начинается плавный пуск, цепи плавного пуска расшунтируются
void Board_OverSmoothStart ( void );                    // При вызове этой функции плавный пуск завершается, цепи плавного пуска шунтируются
void Board_StartCurrent ( void );                        // При вызове этой функции запускается контроллер ШИМ, регулирующий ток в электромагните
void Board_StopCurrent ( void );                        // При вызове этой функции останавливается контроллер ШИМ, регулирующий ток в электромагните
void Board_SetTargetCurrent ( int32_t TargetCurrent );  // При вызове этой функции на управляющий вход контроллера ШИМ подаётся уставка тока
void Board_SetPositiveCurrentPolarity ( void );          // При вызове этой функции схема управления током переключается в режим генерации тока положительного направления
void Board_SetNegativeCurrentPolarity ( void );          // При вызове этой функции схема управления током переключается в режим генерации тока отрицательного направления
void Board_EnableCAN ( void );                            // При вызове этой функции разрешается работа приёмопередатчика CAN
void Board_DisableCAN (void );                            // При вызове этой функции запрещается работа приёмопередатчика CAN

void Board_TurnOnGreenEthernetLed ( void );
void Board_TurnOffGreenEthernetLed ( void );
void Board_TurnOnYellowEthernetLed ( void );
void Board_TurnOffYellowEthernetLed ( void );
void Board_TurnOnGreenLed ( void );
void Board_TurnOffGreenLed ( void );
void Board_TurnOnRedLed ( void );
void Board_TurnOffRedLed ( void );

uint32_t Board_GetSolenoidVoltage ( void );              // Данная функция возвращает значение напряжения, используется при плавном пуске
uint32_t Board_GetSolenoidCurrent ( void );              // Данная функция возвращает значение тока электромагнита
int32_t Board_GetSolenoidAngle ( void );                // Данная функция возвращает угол поворота электромагнита
void Board_GetSolenoidAngleTimeStamp ( uint64_t* pDst );
uint32_t Board_GetStopButtonState ( void );
void Board_OnTimer4Ch2Cap ( void );
void Board_OnTimer4Ch4Cap ( void );
void Board_OnADC1CH0Conversion ( uint32_t ConversionResult );
void Board_OnADC1CH1Conversion ( uint32_t ConversionResult );

typedef enum {
  ANGLE_MEASURES = 2,
  ANGLE_SENSOR_VOLTAGE_MEASURES = 2,
  CAPACITORS_VOLTAGE_MEASURES = 3,
  SOURCE_VOLTAGE_MEASURES = 4,
  PAYLOAD_CURRENT_MEASURES = 5,
  FREQ_SENSOR_VOLTAGE_MEASURES = 6,
} eADCChannels;

void Board_StartMeasures ( uint32_t MeasuresFlags );
void Board_StopMeasures ( uint32_t MeasuresFlags );
void DispatchBoard ( void );

#endif // __BOARD_H__
