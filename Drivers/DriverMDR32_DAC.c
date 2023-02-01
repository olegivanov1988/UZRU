#include "MDR1986VE1T.h"

void MDR32_OutViaDAC1 ( uint32_t Value ) {
  if ( Value > 0xFFF ) {
    MDR_DAC->DAC1_DATA = 0xFFF;
  }
  else {
    MDR_DAC->DAC1_DATA = Value;
  }
}

void MDR32_OutViaDAC2 ( uint32_t Value ) {
  if ( Value > 0xFFF ) {
    MDR_DAC->DAC2_DATA = 0xFFF;
  }
  else {
    MDR_DAC->DAC2_DATA = Value;
  }
}
