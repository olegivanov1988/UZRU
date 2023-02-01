#include "main.h"
#include "MDR1986VE1T.h"
#include "Board.h"
#include "Time.h"

void TIMER2_Handler ( void ) {
  MDR_TIMER2->STATUS = 0;
}

void HardFault_Handler ( void ) {
  
}