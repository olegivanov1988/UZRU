#include "MDR1986VE1T.h"

void InitDAC () {

    MDR_DAC->CFG = 0xC;
    MDR_DAC->DAC1_DATA = 0;    // Данные DAC0
    MDR_DAC->DAC2_DATA = 0;
}
