#include "MDR1986VE1T.h"


void InitRST_CLK () {

  uint32_t temp;
  MDR_RST_CLK->PER_CLOCK |= (1 << 27);              //BKP Clock enable
  temp = MDR_BKP->REG_0E;
  temp &= 0xFFFFFFC0;
  MDR_BKP->REG_0E = temp | (7 << 3) | 7;            // SelectRI = 0x7, LOW = 0x7; (for core frequency more then 80 MHz);
  
  MDR_RST_CLK->HS_CONTROL = 0x00000005;              // HSE on, osc. mode, HSE2 on, osc. mode
  while((MDR_RST_CLK->CLOCK_STATUS&0x0C)!=0x0C);    // Wait until HSE and HSE2 not ready
  MDR_RST_CLK->CPU_CLOCK=0x00000002;                //HSE=8 MHz
  
  MDR_RST_CLK->PLL_CONTROL = (11 << 8) | (1<<2);    // PLL CPU on, множитель частоты равен 11+1
  while((MDR_RST_CLK->CLOCK_STATUS&0x02)!=0x02);    //Wait until PLL CPU not ready
  
  MDR_RST_CLK->PER_CLOCK |= 0x08;                    // EEPROM_CNTRL clock enable
  MDR_EEPROM->CMD = 4<<3;                            // Delay = 4;
  MDR_RST_CLK->PER_CLOCK &= (~0x08);                    // EEPROM_CNTRL clock enable
  
/*********************Устанавливаем частоту ядра равной 96 МГц*******************************/
  MDR_RST_CLK->CPU_CLOCK =
  ( 2 << 0 )                                      // Биты выбора источника для CPU_C1 ( 0 - HSI, 1 - HSI/2, 2 - HSE, 3 - HSE/2 )
  |(1 << 2 )                                      // Биты выбора источника для CPU_C2 ( 0 - CPU_C1, 1 - PLL_CPUo )
  |(0 << 4 )                                      // Биты выбора делителя для CPU_C3 ( 0xxx - CPU_C2, 8 - CPU_C2/2, 9 - CPU_C2/4, n - CPU_C2/(2^(n-7)), 15 - CPU_C2/256 ) 
  |(1 << 8 )                                      // Биты выбора источника для HCLK ( 0 - HSI, 1 - CPU_C3, 2 - LSE, 3 - LSI )
  ;
  
  MDR_RST_CLK->ETH_CLOCK = 
  ( 1 << 24 )                                     // Разрешение тактовой частоты на Ethernet MAC ( 0 - нет частоты, 1 - есть частота )
  | ( 3 << 28 )                                    // Выбор источника частоты для Ethernet PHY ( 0 - HSI, 1 - HSE, 2 - PLLCPUo, 3 - HSE2 )
  | ( 1 << 27 )                                    // Разрешение тактовой частоты Ethernet PHY ( 0 - нет частоты, 1 - есть частота )
  ;
  
  MDR_RST_CLK->PER_CLOCK |=
  ( 1 << 0 )             // Тактирование контроллера CAN1.
  |( 0 << 1 )           // Тактирование контроллера CAN2.
  |( 0 << 2 )           // Тактирование контроллера USB.
  |( 1 << 3 )           // Тактирование контроллера EEPROM_CNTRL.
  |( 1 << 4 )           // Тактирование контроллера RST_CLK.
  |( 0 << 5 )           // Тактирование контроллера DMA.
  |( 0 << 6 )           // Тактирование контроллера UART1.
  |( 0 << 7 )           // Тактирование контроллера UART2.
  |( 0 << 8 )           // Тактирование контроллера SPI1.
  |( 0 << 9 )           // Тактирование контроллера MIL_STD_15531B1.
  |( 0 << 10 )           // Тактирование контроллера MIL_STD_15531B2.
  |( 1 << 11 )           // Тактирование контроллера POWER.
  |( 0 << 12 )           // Тактирование контроллера WWDT.
  |( 0 << 13 )           // Тактирование контроллера IWDT.
  |( 1 << 14 )           // Тактирование контроллера TIMER1.
  |( 1 << 15 )           // Тактирование контроллера TIMER2.
  |( 1 << 16 )           // Тактирование контроллера TIMER3.
  |( 1 << 17 )           // Тактирование контроллера ADC.
  |( 1 << 18 )           // Тактирование контроллера DAC.
  |( 1 << 19 )           // Тактирование контроллера TIMER4.
  |( 0 << 20 )           // Тактирование контроллера SPI2.
  |( 1 << 21 )           // Тактирование контроллера PORTA.
  |( 1 << 22 )           // Тактирование контроллера PORTB.
  |( 1 << 23 )           // Тактирование контроллера PORTC.
  |( 1 << 24 )           // Тактирование контроллера PORTD.
  |( 1 << 25 )           // Тактирование контроллера PORTE.
  |( 0 << 26 )           // Тактирование контроллера ARINC429R.
  |( 1 << 27 )           // Тактирование контроллера BKP.
  |( 0 << 28 )           // Тактирование контроллера ARINC429T.
  |( 1 << 29 )           // Тактирование контроллера PORTF.
  |( 0 << 30 )           // Тактирование контроллера EXT_BUS_CNTRL.
  |( 0 << 31 )           // Тактирование контроллера SPI3.
  ;

  MDR_RST_CLK->TIM_CLOCK =
  ( 0 << 0 )            // Делитель тактовой частоты TIMER1 ( N от 0 до 7, делитель равен 2^N, т.е. от 1 до 128 )
  |( 0 << 8 )            // Делитель тактовой частоты TIMER2 ( N от 0 до 7, делитель равен 2^N, т.е. от 1 до 128 )
  |( 0 << 16 )          // Делитель тактовой частоты TIMER3 ( N от 0 до 7, делитель равен 2^N, т.е. от 1 до 128 )
  |( 1 << 24 )          // Разрешение тактовой частоты на TIMER1 ( 0 - нет тактовой частоты, 1 - есть тактовая частота )
  |( 1 << 25 )          // Разрешение тактовой частоты на TIMER2 ( 0 - нет тактовой частоты, 1 - есть тактовая частота )
  |( 1 << 26 )          // Разрешение тактовой частоты на TIMER3 ( 0 - нет тактовой частоты, 1 - есть тактовая частота )
  ;
  
  MDR_RST_CLK->CAN_CLOCK = 
  ( 2 << 0 )             // Делитель тактовой частоты CAN1.
  |( 1 << 24 )           // Разрешение тактовой частоты на CAN1.
  |( 0 << 8 )           // Делитель тактовой частоты CAN2.
  |( 0 << 25 )           // Разрешение тактовой частоты на CAN2.
  ;
  
  MDR_RST_CLK->ADC_MCO_CLOCK = 
  ( 0 << 0 )            // Выбор источника для ADC_C1 ( 0 - CPU_C1, 1 - USB_C1, 2 - CPU_C2, 3 - USB_C2 )
  |( 2 << 4 )            // Выбор источника для ADC_C2 ( 0 - LSE, 1 - LSI, 2 - ADC_C1, 3 - HSI_C1 )
  |( (7+3) << 8)        // Выбор делителя для ADC_C3 ( 0 - ADC_C3 = ADC_C2, 8 - ADC_C3 = ADC_C2/2, 9 - ADC_C3 = ADC_C2/4, ..., n - ADC_C3 = ADC_C2/(2^(n-7)), ..., 15 - ADC_C3 = ADC_C2/256
  |( 1 << 13)            // Бит разрешения выдачи тактовой частоты ADC CLK ( 0 - запрещён, 1 - разрешён )
  ;
  
  MDR_RST_CLK->UART_CLOCK = 
  ( 0 << 0 )            // Делитель тактовой частоты UART1 ( N от 0 до 7, делитель равен 2^N, т.е. от 1 до 128 )
  |( 0 << 8 )            // Делитель тактовой частоты UART2 ( N от 0 до 7, делитель равен 2^N, т.е. от 1 до 128 )
  |( 0 << 16 )          // Делитель тактовой частоты TIMER4 ( N от 0 до 7, делитель равен 2^N, т.е. от 1 до 128 )
  |( 0 << 24 )          // Разрешение тактовой частоты на UART1 ( 0 - нет тактовой частоты, 1 - есть тактовая частота )
  |( 0 << 25 )          // Разрешение тактовой частоты на UART2 ( 0 - нет тактовой частоты, 1 - есть тактовая частота )
  |( 1 << 26 )          // Разрешение тактовой частоты на TIMER4( 0 - нет тактовой частоты, 1 - есть тактовая частота )
  ;
  
}
