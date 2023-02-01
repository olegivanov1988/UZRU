#include "main.h"
#include "TimeControl.h"
#include "MDR1986VE1T.h"

void ADC_Handler ( void ) {
  Board_OnADCConversion ( ( MDR_ADC->ADC1_RESULT >> 16 ) & 0x1F, MDR_ADC->ADC1_RESULT & 0xFFF );
  MDR_ADC->ADC1_STATUS &= ~(1<<2);
}
