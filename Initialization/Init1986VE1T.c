#include "InitEEPROM.h"
#include "InitRST_CLK.h"
#include "InitTIMER1.h"
#include "InitTIMER2.h"
#include "InitTIMER3.h"
#include "InitADC.h"
#include "InitDAC.h"
#include "InitTIMER4.h"
#include "InitPORTA.h"
#include "InitPORTB.h"
#include "InitPORTC.h"
#include "InitPORTD.h"
#include "InitPORTE.h"
#include "InitPORTF.h"

void Init1986VE1T () {
  InitRST_CLK ();
  
  InitPORTA ();
  InitPORTB ();
  InitPORTC ();
  InitPORTD ();
  InitPORTF ();  
  InitPORTE ();  

  InitDAC ();
  InitADC ();
  
  InitEEPROM ();
  
  InitTIMER1 ();
  InitTIMER2 ();
  InitTIMER3 ();
  InitTIMER4 ();
}
