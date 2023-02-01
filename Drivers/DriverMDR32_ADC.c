#include "MDR1986VE1T.h"

void StartADCConversion ( MDR_ADC_TypeDef* pADC ) {
  // pADC->ADC1_CFG |= 1;
}

void StopADCConversion () {
}

void SetADCChannelForConversion () {
}

void SetADCChannelsForSwitching () {
}

void MDR1986VE1T_AddADCChannelToSwitchList ( MDR_ADC_TypeDef* pADC, uint32_t ChannelFlags ) {
  pADC->ADC1_CHSEL |= ( ChannelFlags );
}

void MDR1986VE1T_DeleteADCChannelFromSwitchList ( MDR_ADC_TypeDef* pADC, uint32_t ChannelFlags ) {
  pADC->ADC1_CHSEL &= ~( ChannelFlags );
}
