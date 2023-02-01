// ????????? ??? ????
#include "MDR1986VE1T.h"
#include "stdint.h"
#include "Menu.h"
#include "Keyboard.h"
#include "Subsystems.h"
#include "References.h"
#include "Calibrations.h"
#include "Measurements.h"
#include "Relays.h"
#include "Faults.h"
#include "Uzru10a.h"

int32_t NumOfMenuLines = -1;
int32_t NumOfMenuItems = 0;
int32_t NumOfTextItems = 0;
int32_t NumOfMarkerItems = 0;
int32_t NumOfFloatPlayerItems = 0;
int32_t NumOfIntPlayerItems = 0;
int32_t NumOfFixedPointNumberPlayerItems = 0;
int32_t NumOfTimePlayerItems = 0;
int32_t NumOfFloatEditorItems = 0;
int32_t NumOfIntEditorItems = 0;
int32_t NumOfFixedPointNumberEditorItems = 0;
int32_t NumOfTimeEditorItems = 0;
int32_t NumOfVoidCommandItems = 0;
int32_t NumOfIntCommandItems = 0;
int32_t NumOfComboBoxItems = 0;
int32_t NumOfComboBoxItemsElements = 0;

uint32_t MenuMarkerLine = 0;
uint32_t MenuTopBevelLine = 0;

uint32_t MenuTimeStamp;

int32_t ChargeMode;

extern uint32_t ModeStopOrPauseReason;

extern uint32_t Stage1Reached;
extern int32_t Stage1Umax, Stage1Umin;
extern int32_t Stage1Imax, Stage1Imin;
extern uint64_t tmpStage1Volume;
extern int32_t Stage1Volume;
extern uint64_t Stage1Time;
extern uint64_t Stage1MaxTime;

extern uint32_t Stage2Reached;
extern int32_t Stage2Umax, Stage2Umin;
extern int32_t Stage2Imax, Stage2Imin;
extern uint64_t tmpStage2Volume;
extern int32_t Stage2Volume;
extern uint64_t Stage2Time;
extern uint64_t Stage2MaxTime;

extern uint32_t Stage3Reached;
extern int32_t Stage3Umax, Stage3Umin;
extern int32_t Stage3Imax, Stage3Imin;
extern uint64_t tmpStage3Volume;
extern int32_t Stage3Volume;
extern uint64_t Stage3Time;
extern uint64_t Stage3MaxTime;

extern uint32_t Stage4Reached;
extern int32_t Stage4Umax, Stage4Umin;
extern int32_t Stage4Imax, Stage4Imin;
extern uint64_t tmpStage4Volume;
extern int32_t Stage4Volume;
extern uint64_t Stage4Time;
extern uint64_t Stage4MaxTime;

extern uint32_t Stage5Reached;
extern int32_t Stage5Umax, Stage5Umin;
extern int32_t Stage5Imax, Stage5Imin;
extern uint64_t tmpStage5Volume;
extern int32_t Stage5Volume;
extern uint64_t Stage5Time;
extern uint64_t Stage5MaxTime;

extern uint32_t Stage6Reached;
extern int32_t Stage6Umax, Stage6Umin;
extern int32_t Stage6Imax, Stage6Imin;
extern uint64_t tmpStage6Volume;
extern int32_t Stage6Volume;
extern uint64_t Stage6Time;
extern uint64_t Stage6MaxTime;

extern uint32_t Stage7Reached;
extern int32_t Stage7Umax, Stage7Umin;
extern int32_t Stage7Imax, Stage7Imin;
extern uint64_t tmpStage7Volume;
extern int32_t Stage7Volume;
extern uint64_t Stage7Time;
extern uint64_t Stage7MaxTime;

extern uint32_t Stage8Reached;
extern int32_t Stage8Umax, Stage8Umin;
extern int32_t Stage8Imax, Stage8Imin;
extern uint64_t tmpStage8Volume;
extern int32_t Stage8Volume;
extern uint64_t Stage8Time;
extern uint64_t Stage8MaxTime;

extern uint32_t Stage9Reached;
extern int32_t Stage9Umax, Stage9Umin;
extern int32_t Stage9Imax, Stage9Imin;
extern uint64_t tmpStage9Volume;
extern int32_t Stage9Volume;
extern uint64_t Stage9Time;
extern uint64_t Stage9MaxTime;

extern uint32_t Stage10Reached;

extern int32_t TotalVolume;
extern uint64_t TotalTime;

// ModesParameters

char* Arguments[32] = { "(0000)", "(0016)", "(0032)", "(0048)", "(0064)", "(0080)", "(0096)", "(0112)", "(0128)", "(0144)", "(0160)", "(0176)", "(0192)", "(0208)", "(0224)", "(0240)", 
                        "(0256)", "(0512)", "(0768)", "(1024)", "(1280)", "(1536)", "(1792)", "(2048)", "(2304)", "(2560)", "(2816)", "(3072)", "(3328)", "(3584)", "(3840)", "(4096)" };

char* DischargeArguments[31] = { "(0000)", "(0032)", "(0064)", "(0096)", "(0128)", "(0160)", "(0192)", "(0224)", "(0256)", "(0288)", "(0320)", "(0352)", "(0384)", "(0416)", "(0448)", "(0480)", 
                        "(0512)", "(0768)", "(1024)", "(1280)", "(1536)", "(1792)", "(2048)", "(2304)", "(2560)", "(2816)", "(3072)", "(3328)", "(3584)", "(3840)", "(4096)" };

char* ChargeVoltageValues[17] = { "00,0", "02,5", "05,0", "07,5", "10,0", "12,5", "15,0", "17,5", "20,0", "22,5", "25,0", "27,5", "30,0", "32,5", "35,0", "37,5", "40,0" };
  
char* ChargeCurrentValues[37] = { "00,00", "00,25", "00,50", "00,75", "01,00", "01,25", "01,50", "01,75", "02,00", "02,25", "02,50", "02,75", "03,00", "03,25", "03,50", "03,75", "04,00",
                                  "04,25", "04,50", "04,75", "05,00", "10,00", "15,00", "20,00", "25,00", "30,00", "35,00", "40,00", "45,00", "50,00", "55,00", "60,00", "65,00", "70,00",
                                  "75,00", "80,00", "85,00" };
                        
static int32_t i, j, k;

enum {
  CONST_U_CHARGE,          // ???? ?????????? ?????? ?????????? ?????
  CONST_I_CHARGE,          // ???? ?????????? ?????? ?????????? ???????????
  COMBINE_CHARGE,          // ???? ?????????? ???????????????? ??????
  ASIMM_CHARGE,
  CONTROL_DISCHARGE,       // ???? ?????????? ??????? ?????????? ?????
} ChargeModeEnum;

uint32_t MenuIndex = 0;

void (*MenuLoadMenu)(void);      // ????????? ?? ??????? ???????? ????
void (*MenuOnUpButtonPress)(void);   // ????????? ?? ?????????? ?????? "?????"
void (*MenuOnDownButtonPress)(void);  // ????????? ?? ?????????? ?????? "????"
void (*MenuOnLeftButtonPress)(void);  // ????????? ?? ?????????? ?????? "?????"
void (*MenuOnRightButtonPress)(void); // ????????? ?? ?????????? ?????? "??????"
void (*MenuOnStartButtonPress)(void); // ????????? ?? ?????????? ?????? "?????"
void (*MenuOnStopButtonPress)(void);  // ????????? ?? ?????????? ?????? "????"


uint32_t MarkerActive = 1;

void LoadMainMenu ();
void LoadDifficultModesMenu ();
void LoadConstIChargeMenu ();
void LoadSimpleConstIChargeMenu ();
void LoadConstUChargeMenu ();
void LoadSimpleConstUChargeMenu ();
void LoadFastCombineChargeMenu ();
void LoadSimpleFastCombineChargeMenu ();
void LoadSlowCombineChargeMenu ();
void LoadConstIWithLimitUChargeMenu ();
void LoadAsimmChargeMenu ();
void LoadControlDischargeMenu ();
void LoadSimpleControlDischargeMenu ();
void LoadCTCMenu ();
void LoadSimpleCTCMenu ();
void LoadMeasuresMenu ();
void LoadFaultsMenu ();
void LoadMyMenu ();
void DefaultOnUpButtonPress ();
void DefaultOnDownButtonPress ();
void DefaultOnStartButtonPress ();
void DefaultOnStopButtonPress ();

typedef struct {
  int32_t FirstItemIndex;
} MenuLineInfoStruct;

#define MAX_NUM_OF_MENU_LINES 80

MenuLineInfoStruct MenuLines[MAX_NUM_OF_MENU_LINES];

typedef enum {
  TEXT_ITEM,                      // РўРµРєСЃС‚РѕРІС‹Р№ СЌР»РµРјРµРЅС‚ - РїСЂРѕСЃС‚Рѕ СЃС‚Р°С‚РёС‡РµСЃРєРёР№ С‚РµРєСЃС‚, Р·Р°РЅРёРјР°СЋС‰РёР№ РЅРµСЃРєРѕР»СЊРєРѕ СЃРёРјРІРѕР»РѕРІ РІ СЃС‚СЂРѕРєРµ
  MARKER_ITEM,                    // РњР°СЂРєРµСЂ - С‚РµРєСЃС‚, РєРѕС‚РѕСЂС‹Р№ РІС‹РІРѕРґРёС‚СЃСЏ РјРёРіР°СЋС‰РёРј, РєРѕРіРґР° РєСѓСЂСЃРѕСЂ РЅР°С…РѕРґРёС‚СЃСЏ РЅР° С‚РµРєСѓС‰РµР№ СЃС‚СЂРѕРєРµ Рё РЅРµ Р·Р°С…РІР°С‡РµРЅ
  STATIC_MARKER_ITEM,             // РќРёРіРґРµ РЅРµ РёСЃРїРѕР»СЊР·СѓРµС‚СЃСЏ
  FLOAT_PLAYER_ITEM,              // РќРёРіРґРµ РЅРµ РёСЃРїРѕР»СЊР·СѓРµС‚СЃСЏ
  INT_PLAYER_ITEM,                // Р¦РµР»РѕРµ С‡РёСЃР»Рѕ - С‡РёСЃР»Рѕ, РєРѕС‚РѕСЂРѕРµ РІС‹РІРѕРґРёС‚СЃСЏ РЅР° СЃРёРјРІРѕР»С‹ СЃС‚СЂРѕРєРё РёР· РЅРµРєРѕС‚РѕСЂРѕР№ РїРµСЂРµРјРµРЅРЅРѕР№
  FIXED_POINT_NUMBER_PLAYER_ITEM, // Р§РёСЃР»Рѕ СЃ С„РёРєСЃРёСЂРѕРІР°РЅРЅРѕР№ С‚РѕС‡РєРѕР№
  TIME_PLAYER_ITEM,               // Р’СЂРµРјСЏ
  FLOAT_EDITOR_ITEM,              // Р РµРґР°РєС‚РѕСЂ С‡РёСЃР»Р° СЃ РїР»Р°РІР°СЋС‰РµР№ С‚РѕС‡РєРѕР№
  INT_EDITOR_ITEM,                // Р РµРґР°РєС‚РѕСЂ С†РµР»РѕРіРѕ С‡РёСЃР»Р°
  FIXED_POINT_NUMBER_EDITOR_ITEM, // Р РµРґР°РєС‚РѕСЂ С‡РёСЃР»Р° СЃ С„РёРєСЃРёСЂРѕРІР°РЅРЅРѕР№ С‚РѕС‡РєРѕР№
  TIME_EDITOR_ITEM,               // Р РµРґР°РєС‚РѕСЂ РІСЂРµРјРµРЅРё
  COMBO_BOX_ITEM,                 // РђРЅР°Р»РѕРі РІС‹РїР°РґР°СЋС‰РµРіРѕ СЃРїРёСЃРєР°
  VOID_COMMAND_ITEM,              // РљРѕРјР°РЅРґР° Р±РµР· Р°СЂРіСѓРјРµРЅС‚РѕРІ
  INT_COMMAND_ITEM,               // РљРѕРјР°РЅРґР° СЃ РѕРґРЅРёРј С†РµР»С‹Рј Р°СЂРіСѓРјРµРЅС‚РѕРј
  
  NUM_OF_ITEM_KINDS
} MenuItemKindEnum;

typedef struct {
  uint32_t ItemKind;
  uint32_t ItemIndex;
} MenuItemInfoStruct;

#define MAX_NUM_OF_MENU_ITEMS 160

MenuItemInfoStruct MenuItems[MAX_NUM_OF_MENU_ITEMS];

typedef struct {
  char* pText;
  uint32_t Length;
} TextItemInfoStruct;

#define MAX_NUM_OF_TEXT_ITEMS 110

TextItemInfoStruct TextItems[MAX_NUM_OF_TEXT_ITEMS];

typedef struct {
  char* pMarkerText;
  uint32_t Length;
} MarkerItemInfoStruct;

#define MAX_NUM_OF_MARKER_ITEMS 74

MarkerItemInfoStruct MarkerItems[MAX_NUM_OF_MARKER_ITEMS];

typedef struct {
  int32_t* pInt;
  uint32_t Width;
} IntPlayerItemInfoStruct;

#define MAX_NUM_OF_INT_PLAYER_ITEMS 10

IntPlayerItemInfoStruct IntPlayerItems[MAX_NUM_OF_INT_PLAYER_ITEMS];

typedef struct {
  int32_t* pInt;
  int32_t Value;
  uint64_t LastValueUpdateTimeStamp;
  int32_t Divider;
  int32_t PointPosition;
  uint32_t Width;
} FixedPointNumberPlayerItemInfoStruct;

#define MAX_NUM_OF_FIXED_POINT_NUMBER_PLAYER_ITEMS 40

FixedPointNumberPlayerItemInfoStruct FixedPointNumberPlayerItems [MAX_NUM_OF_FIXED_POINT_NUMBER_PLAYER_ITEMS];

typedef struct {
  uint64_t* pTime;
} TimePlayerItemInfoStruct;

#define MAX_NUM_OF_TIME_PLAYER_ITEMS 10

TimePlayerItemInfoStruct TimePlayerItems[MAX_NUM_OF_TIME_PLAYER_ITEMS];

typedef struct {
  int32_t CursorPosition; // РџРѕР·РёС†РёСЏ РєСѓСЂСЃРѕСЂР° ( )
  int32_t PointPosition; // РџРѕР·РёС†РёСЏ РґРµСЃСЏС‚РёС‡РЅРѕР№ С‚РѕС‡РєРё
  int32_t Divider; // Р”РµР»РёС‚РµР»СЊ
  int32_t Width; // Р§РёСЃР»Рѕ РІС‹РІРѕРґРёРјС‹С… РЅР° РґРёСЃРїР»РµР№ СЃРёРјРІРѕР»РѕРІ
  char Digits[12]; // Р¦РёС„СЂС‹ РєР°Р¶РґРѕРіРѕ РґРµСЃСЏС‚РёС‡РЅРѕРіРѕ СЂР°Р·СЂСЏРґР° С‡РёСЃР»Р° 
  int32_t* pValue; // РЈРєР°Р·Р°С‚РµР»СЊ РЅР° РїРµСЂРµРјРµРЅРЅСѓСЋ, СЃ РєРѕС‚РѕСЂРѕР№ СЃРёРЅС…СЂРѕРЅРёР·РёСЂРѕРІР°РЅ СЂРµРґР°РєС‚РѕСЂ
  int32_t MinValue; // РњРёРЅРёРјР°Р»СЊРЅРѕРµ РґРѕРїСѓСЃС‚РёРјРѕРµ Р·РЅР°С‡РµРЅРёРµ
  int32_t MaxValue; // РњР°РєСЃРёРјР°Р»СЊРЅРѕРµ РґРѕРїСѓСЃС‚РёРјРѕРµ Р·РЅР°С‡РµРЅРёРµ
  void (*OnConfirm)(int32_t NewValue); // РЈРєР°Р·Р°С‚РµР»СЊ РЅР° С„СѓРЅРєС†РёСЋ-РѕР±СЂР°Р±РѕС‚С‡РёРє РІРІРѕРґР° (РІРґСЂСѓРі РјС‹ С…РѕС‚РёРј РІС‹РїРѕР»РЅРёС‚СЊ РєР°РєРёРµ-С‚Рѕ РґРµР№СЃС‚РІРёСЏ РїРѕРјРёРјРѕ РёР·РјРµРЅРµРЅРёСЏ РїРµСЂРµРјРµРЅРЅРѕР№ РІ РїР°РјСЏС‚Рё)
} FixedPointNumberEditorItemInfoStruct;

#define MAX_NUM_OF_FIXED_POINT_NUMBER_EDITOR_ITEMS 40 // РњР°РєСЃРёРјР°Р»СЊРЅРѕРµ С‡РёСЃР»Рѕ СЂРµРґР°РєС‚РѕСЂРѕРІ С‡РёСЃРµР» СЃ С„РёРєСЃРёСЂРѕРІР°РЅРЅРѕР№ С‚РѕС‡РєРѕР№

FixedPointNumberEditorItemInfoStruct FixedPointNumberEditorItems [MAX_NUM_OF_FIXED_POINT_NUMBER_EDITOR_ITEMS]; // РњР°СЃСЃРёРІ СЂРµРґР°РєС‚РѕСЂРѕРІ С‡РёСЃРµР» СЃ С„РёРєСЃРёСЂРѕРІР°РЅРЅРѕР№ С‚РѕС‡РєРѕР№

typedef struct {
  uint64_t* pTime; // РЈРєР°Р·Р°С‚РµР»СЊ РЅР° РїРµСЂРµРјРµРЅРЅСЋ, СЃ РєРѕС‚РѕСЂРѕР№ СЃРёРЅС…СЂРѕРЅРёР·РёСЂРѕРІР°РЅ РґР°РЅРЅС‹Р№ СЂРµРґР°РєС‚РѕСЂ РІСЂРµРјРµРЅРё
  uint64_t MinTime; // РњРёРЅРёРјР°Р»СЊРЅРѕРµ РґРѕРїСѓСЃС‚РёРјРѕРµ Р·РЅР°С‡РµРЅРёРµ РІСЂРµРјРµРЅРё
  uint64_t MaxTime; // РњР°РєСЃРёРјР°Р»СЊРЅРѕРµ РґРѕРїСѓСЃС‚РёРјРѕРµ Р·РЅР°С‡РµРЅРёРµ РІСЂРµРјРµРЅРё
  char Digits[6]; // Р¦РёС„СЂС‹ РєР°Р¶РґРѕРіРѕ СЂР°Р·СЂСЏРґР° РІСЂРµРјРµРЅРё (Р§Р§РњРњРЎРЎ)
  int32_t CursorPosition; // РџРѕР·РёС†РёСЏ РєСѓСЂСЃРѕСЂР° (0 - РєСѓСЂСЃРѕСЂ РЅРµ Р·Р°С…РІР°С‡РµРЅ, 1 - 6 - РєСѓСЂСЃРѕСЂ Р·Р°С…РІР°С‡РµРЅ Рё РЅР°С…РѕРґРёС‚СЃСЏ РЅР°Рґ С†РёС„СЂРѕР№ СЃ РёРЅРґРµРєСЃРѕРј CursorPosition)
  void (*OnConfirm)(uint64_t NewValue); // РЈРєР°Р·Р°С‚РµР»СЊ РЅР° РѕР±СЂР°Р±РѕС‚С‡РёРє РІРІРµРґРµРЅРЅРѕРіРѕ Р·РЅР°С‡РµРЅРёСЏ (РІРґСЂСѓРі РјС‹ С…РѕС‚РёРј РІС‹РїРѕР»РЅРёС‚СЊ РєР°РєРёРµ-С‚Рѕ РґРµР№СЃС‚РІРёСЏ РїРѕРјРёРјРѕ РёР·РјРµРЅРµРЅРёСЏ РїРµСЂРµРјРµРЅРЅРѕР№ РІ РїР°РјСЏС‚Рё)
} TimeEditorItemInfoStruct;

#define MAX_NUM_OF_TIME_EDITOR_ITEMS 10 // РњР°РєСЃРёРјР°Р»СЊРЅРѕРµ С‡РёСЃР»Рѕ СЂРµРґР°РєС‚РѕСЂРѕРІ РІСЂРµРјРµРЅРё РІ РјРµРЅСЋ

TimeEditorItemInfoStruct TimeEditorItems[MAX_NUM_OF_TIME_EDITOR_ITEMS]; // РњР°СЃСЃРёРІ СЂРµРґР°РєС‚РѕСЂРѕРІ РІСЂРµРјРµРЅРё

typedef enum {
  BTN_RIGHT_ONLY, // РљРѕРјР°РЅРґР° Р°РєС‚РёРІРёСЂСѓРµС‚СЃСЏ С‚РѕР»СЊРєРѕ РєРЅРѕРїРєРѕР№ Р’РџР РђР’Рћ
  BTN_START_ONLY, // РљРѕРјР°РЅРґР° Р°РєС‚РёРІРёСЂСѓРµС‚СЃСЏ С‚РѕР»СЊРєРѕ РєРЅРѕРїРєРѕР№ РџРЈРЎРљ
} eCommandButtons;

typedef struct {
  int32_t ButtonKinds; // РљРЅРѕРїРєРё, РєРѕС‚РѕСЂС‹Рµ РјРѕРіСѓС‚ РїСЂРёРІРѕРґРёС‚СЊ Рє РІС‹РїРѕР»РЅРµРЅРёСЋ РєРѕРјР°РЅРґС‹
  void (*Action)(void);// РЈРєР°Р·Р°С‚РµР»СЊ РЅР° С„СѓРЅРєС†РёСЋ-РѕР±СЂР°Р±РѕС‚С‡РёРє РІС‹Р±РѕСЂР° РєРѕРјР°РЅРґС‹
} VoidCommandItemInfoStruct;

#define MAX_NUM_OF_VOID_COMMAND_ITEMS 10 // РњР°РєСЃРёРјР°Р»СЊРЅРѕРµ С‡РёСЃР»Рѕ РєРѕРјР°РЅРґ Р±РµР· Р°СЂРіСѓРјРµРЅС‚Р° РІ РјРµРЅСЋ

VoidCommandItemInfoStruct VoidCommandItems[MAX_NUM_OF_VOID_COMMAND_ITEMS]; // РњР°СЃСЃРёРІ РєРѕРјР°РЅРґ Р±РµР· Р°СЂРіСѓРјРµРЅС‚Р°

typedef struct {
  void (*Action)(int32_t Argument); // РЈРєР°Р·Р°С‚РµР»СЊ РЅР° С„СѓРЅРєС†РёСЋ-РѕР±СЂР°Р±РѕС‚С‡РёРє РІС‹Р±РѕСЂР° РєРѕРјР°РЅРґС‹
  int32_t Argument;                 // РђСЂРіСѓРјРµРЅС‚ РѕР±СЂР°Р±РѕС‚С‡РёРєР°
} IntCommandItemInfoStruct;

#define MAX_NUM_OF_INT_COMMAND_ITEMS NUM_OF_FAULTS // Р§РёСЃР»Рѕ РєРѕРјР°РЅРґ СЃ Р°СЂРіСѓРјРµРЅС‚РѕРј РІ РјРµРЅСЋ

IntCommandItemInfoStruct IntCommandItems[MAX_NUM_OF_INT_COMMAND_ITEMS]; // РњР°СЃСЃРёРІ РєРѕРјР°РЅРґ СЃ Р°СЂРіСѓРјРµРЅС‚РѕРј

typedef struct {
  char* pItemText;   // РЈРєР°Р·Р°С‚РµР»СЊ РЅР° СЃС‚СЂРѕРєСѓ СЌР»РµРјРµРЅС‚Р° СЃРїРёСЃРєР°
  int32_t TextLength;// Р§РёСЃР»Рѕ РІС‹РІРѕРґРёРјС‹С… СЃРёРјРІРѕР»РѕРІ
 } ComboBoxElementInfoStruct;
 
 #define MAX_NUM_OF_COMBO_BOX_ELEMENTS 4 // Р§РёСЃР»Рѕ СЌР»РµРјРµРЅС‚РѕРІ СЃРїРёСЃРєРѕРІ РІ РјРµРЅСЋ
 
 ComboBoxElementInfoStruct ComboBoxElements[MAX_NUM_OF_COMBO_BOX_ELEMENTS]; // РњР°СЃСЃРёРІ СЌР»РµРјРµРЅС‚РѕРІ СЃРїРёСЃРєРѕРІ
 
 typedef struct {
   int32_t FirstElementIndex; // РРЅРґРµРєСЃ РїРµСЂРІРѕРіРѕ СЌР»РµРјРµРЅС‚Р° СЃРїРёСЃРєР°
   int32_t MaxNumOfElements;  // РњР°РєСЃРёРјР°Р»СЊРЅРѕРµ С‡РёСЃР»Рѕ СЌР»РµРјРµРЅС‚РѕРІ С‚РµРєСѓС‰РµРіРѕ СЃРїРёСЃРєР°
   int32_t NumOfElements;     // Р§РёСЃР»Рѕ СЌР»РµРјРµРЅС‚РѕРІ С‚РµРєСѓС‰РµРіРѕ СЃРїРёСЃРєР°
   int32_t ElementIndex;      // РРЅРґРµРєСЃ Р°РєС‚РёРІРЅРѕРіРѕ СЌР»РµРјРµРЅС‚Р° СЃРїРёСЃРєР°
   int32_t CursorPosition;    // РџРѕР·РёС†РёСЏ РєСѓСЂСЃРѕСЂР° ( 0 - РєСѓСЂСЃРѕСЂ СЃРІРѕР±РѕРґРµРЅ, 1 - РєСѓСЂСЃРѕСЂ Р·Р°С…РІР°С‡РµРЅ )
   void (*Action)(int32_t SelectedItemIndex); // РЈРєР°Р·Р°С‚РµР»СЊ РЅР° РѕР±СЂР°Р±РѕС‚С‡РёРє РІС‹Р±РѕСЂР° СЌР»РµРјРµРЅС‚Р° СЃРїРёСЃРєР°
 } ComboBoxItemInfoStruct;
 
 #define MAX_NUM_OF_COMBO_BOX_ITEMS 4 // РњР°РєСЃРёРјР°Р»СЊРЅРѕРµ С‡РёСЃР»Рѕ СЃРїРёСЃРєРѕРІ РІ РјРµРЅСЋ
 
 ComboBoxItemInfoStruct ComboBoxItems[MAX_NUM_OF_COMBO_BOX_ITEMS]; // РњР°СЃСЃРёРІ РІС‹РїР°РґР°СЋС‰РёС… СЃРїРёСЃРєРѕРІ
 
void ClearMenu ( void ) {
  NumOfMenuLines = -1;
  NumOfMenuItems = 0;
  NumOfTextItems = 0;
  NumOfMarkerItems = 0;
  NumOfFloatPlayerItems = 0;
  NumOfIntPlayerItems = 0;
  NumOfFixedPointNumberPlayerItems = 0;
  NumOfTimePlayerItems = 0;
  NumOfFloatEditorItems = 0;
  NumOfIntEditorItems = 0;
  NumOfFixedPointNumberEditorItems = 0;
  NumOfTimeEditorItems = 0;
  NumOfVoidCommandItems = 0;
  NumOfIntCommandItems = 0;
  NumOfComboBoxItems = 0;
  NumOfComboBoxItemsElements = 0;
  MenuMarkerLine = 0;
  MenuTopBevelLine = 0;
  MenuOnUpButtonPress = DefaultOnUpButtonPress;
  MenuOnLeftButtonPress = 0;
  MenuOnDownButtonPress = DefaultOnDownButtonPress;
  MenuOnStopButtonPress = DefaultOnStopButtonPress;
}

void InitMenu ( void ) {
  LoadMainMenu ();
}

void DispatchMenu () {
  GetShortTimeStamp ( &MenuTimeStamp );
  DisplayMenu ();
}

void MenuAddLine () {
  if ( NumOfMenuLines < MAX_NUM_OF_MENU_LINES ) {
    NumOfMenuLines++;
    MenuLines[NumOfMenuLines].FirstItemIndex = NumOfMenuItems;
    MenuLines[NumOfMenuLines+1].FirstItemIndex = NumOfMenuItems;
  }
}

void AddMenuItem ( uint32_t ItemKind, uint32_t ItemIndex ) {
  if ( NumOfMenuItems < MAX_NUM_OF_MENU_ITEMS ) {
    MenuItems[NumOfMenuItems].ItemKind = ItemKind;
    MenuItems[NumOfMenuItems].ItemIndex = ItemIndex;
    MenuLines[NumOfMenuLines+1].FirstItemIndex++;
    NumOfMenuItems++;
  }
}

void MenuAddTextItem ( char* pText, uint32_t Length, uint32_t Width ) {
  if ( NumOfTextItems < MAX_NUM_OF_TEXT_ITEMS ) {
    TextItems[NumOfTextItems].pText = pText;
    TextItems[NumOfTextItems].Length = Length;
    AddMenuItem ( TEXT_ITEM, NumOfTextItems );
    NumOfTextItems++;
  }
}

void MenuAddIntPlayerItem ( int32_t* pInt, uint32_t Width ) {
  if ( NumOfIntPlayerItems < MAX_NUM_OF_INT_PLAYER_ITEMS ) {
    IntPlayerItems[NumOfIntPlayerItems].pInt = pInt;
    IntPlayerItems[NumOfIntPlayerItems].Width = Width; 
    AddMenuItem ( INT_PLAYER_ITEM, NumOfIntPlayerItems );
    NumOfIntPlayerItems++;
  }
}

void MenuAddFixedPointNumberPlayerItem ( int32_t* pInt, uint32_t Width, int32_t Divider, int32_t PointPosition ) {
  if ( NumOfFixedPointNumberPlayerItems < MAX_NUM_OF_FIXED_POINT_NUMBER_PLAYER_ITEMS ) {
    FixedPointNumberPlayerItems[NumOfFixedPointNumberPlayerItems].pInt = pInt;
    FixedPointNumberPlayerItems[NumOfFixedPointNumberPlayerItems].Value = *pInt;
    GetTimeStamp ( & ( FixedPointNumberPlayerItems[NumOfFixedPointNumberPlayerItems].LastValueUpdateTimeStamp ) );
    FixedPointNumberPlayerItems[NumOfFixedPointNumberPlayerItems].Divider = Divider;
    FixedPointNumberPlayerItems[NumOfFixedPointNumberPlayerItems].Width = Width;
    AddMenuItem ( FIXED_POINT_NUMBER_PLAYER_ITEM, NumOfFixedPointNumberPlayerItems );
    NumOfFixedPointNumberPlayerItems++;
  }
}

void MenuAddTimePlayerItem ( int64_t* pTime ) {
  if ( NumOfTimePlayerItems < MAX_NUM_OF_TIME_PLAYER_ITEMS ) {
    TimePlayerItems[NumOfTimePlayerItems].pTime = pTime;
    AddMenuItem ( TIME_PLAYER_ITEM, NumOfTimePlayerItems );
    NumOfTimePlayerItems++;
  }
}

int32_t MenuAddComboBoxItem ( int32_t MaxNumOfElements, void (*Action) (int32_t ElementIndex) ) {
  int32_t tmpComboBoxItem = NumOfComboBoxItems;
  if ( NumOfComboBoxItems < MAX_NUM_OF_COMBO_BOX_ITEMS ) {
    ComboBoxItems[NumOfComboBoxItems].CursorPosition = 0;
    ComboBoxItems[NumOfComboBoxItems].FirstElementIndex = NumOfComboBoxItemsElements;
    NumOfComboBoxItemsElements += MaxNumOfElements;
    ComboBoxItems[NumOfComboBoxItems].NumOfElements = 0;
    ComboBoxItems[NumOfComboBoxItems].MaxNumOfElements = MaxNumOfElements;
    ComboBoxItems[NumOfComboBoxItems].Action = Action;
    ComboBoxItems[NumOfComboBoxItems].ElementIndex = 0;
    AddMenuItem ( COMBO_BOX_ITEM, NumOfComboBoxItems );
  }
  NumOfComboBoxItems++;
  return NumOfComboBoxItems - 1;
}

void MenuAddComboBoxItemElement ( int32_t ComboBoxItemIndex, char* pText, int32_t TextLength ) {
  int32_t tmpComboBoxItemElement = ComboBoxItems[ComboBoxItemIndex].FirstElementIndex + ComboBoxItems[ComboBoxItemIndex].NumOfElements;
  if ( ComboBoxItemIndex < NumOfComboBoxItems ) {
    if ( ComboBoxItems[ComboBoxItemIndex].NumOfElements < ( ComboBoxItems[ComboBoxItemIndex].MaxNumOfElements ) ) {
      ComboBoxElements[tmpComboBoxItemElement].pItemText = pText;
      ComboBoxElements[tmpComboBoxItemElement].TextLength = TextLength;
      ComboBoxItems[ComboBoxItemIndex].NumOfElements++;
    }
  }
}

void ClearComboBoxItemElements ( int32_t ComboBoxItem ) {
  ComboBoxItems[ComboBoxItem].NumOfElements = 0;
  ComboBoxItems[ComboBoxItem].ElementIndex = 0;
}

int32_t ComboBoxGetElementIndex ( int32_t ComboBoxItem ) {
  return ComboBoxItems[ComboBoxItem].ElementIndex;
}

void ComboBoxSetElementIndex ( int32_t ComboBoxItemIndex, int32_t ElementIndex ) {
  if ( ComboBoxItems[ComboBoxItemIndex].NumOfElements > ElementIndex ) {
    ComboBoxItems[ComboBoxItemIndex].ElementIndex = ElementIndex;
    if ( ComboBoxItems[ComboBoxItemIndex].Action ) {
      ( ComboBoxItems[ComboBoxItemIndex].Action ( ElementIndex ) );
    }
  }
}

extern int32_t Dividers[9];

void DownloadFixedPointNumberEditorItemValue ( int32_t FixedPointNumberEditorIndex ) {
  int32_t i;
  int32_t tmpValue = FixedPointNumberEditorItems[FixedPointNumberEditorIndex].PointPosition;
  int32_t tmpDivider = FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Divider;
  while ( tmpValue > 0 ) {
    tmpDivider *= 10;
    tmpValue--;
  }
  tmpValue = *(FixedPointNumberEditorItems[FixedPointNumberEditorIndex].pValue);
  tmpValue = tmpValue % tmpDivider;
  tmpDivider /= 10;
  for ( i = 0; i < FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Width; i++ ) {
    if ( i == FixedPointNumberEditorItems[FixedPointNumberEditorIndex].PointPosition ) {
      FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Digits[i] = ',';
    }
    else {
      FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Digits[i] = '0' + tmpValue/tmpDivider;
      tmpValue %= tmpDivider;
      tmpDivider /= 10;
    }
  }
}

void MenuAddFixedPointNumberEditorItem (int32_t* pValue, uint32_t Width, int32_t PointPosition, int32_t Divider, void (*OnConfirm)(int32_t NewValue), int32_t MinValue, int32_t MaxValue ) {
  if ( NumOfFixedPointNumberEditorItems < MAX_NUM_OF_FIXED_POINT_NUMBER_EDITOR_ITEMS ) {
    FixedPointNumberEditorItems[NumOfFixedPointNumberEditorItems].CursorPosition = Width+1;
    FixedPointNumberEditorItems[NumOfFixedPointNumberEditorItems].Width = Width;
    FixedPointNumberEditorItems[NumOfFixedPointNumberEditorItems].Divider = Divider;
    FixedPointNumberEditorItems[NumOfFixedPointNumberEditorItems].PointPosition = PointPosition;
    FixedPointNumberEditorItems[NumOfFixedPointNumberEditorItems].OnConfirm = OnConfirm;
  if ( *pValue < MinValue ) { *pValue = MinValue; }
  else if ( *pValue > MaxValue ) { *pValue = MaxValue; }
    FixedPointNumberEditorItems[NumOfFixedPointNumberEditorItems].pValue = pValue;
  FixedPointNumberEditorItems[NumOfFixedPointNumberEditorItems].MinValue = MinValue;
  FixedPointNumberEditorItems[NumOfFixedPointNumberEditorItems].MaxValue = MaxValue;
    DownloadFixedPointNumberEditorItemValue (NumOfFixedPointNumberEditorItems);
    AddMenuItem ( FIXED_POINT_NUMBER_EDITOR_ITEM, NumOfFixedPointNumberEditorItems );
    NumOfFixedPointNumberEditorItems++;
  }
}

void DownloadTimeEditorItemValue ( int32_t TimeEditorItemIndex ) {
  uint32_t tmpValue;
  uint64_t tmpTime = (uint64_t)*(TimeEditorItems[TimeEditorItemIndex].pTime);
  tmpTime = (uint64_t)((uint64_t)tmpTime/(uint64_t)96000000);
  tmpValue = (uint64_t)tmpTime % 60;
  TimeEditorItems[TimeEditorItemIndex].Digits[5] = tmpValue%10 + '0';
  tmpValue /= 10;
  TimeEditorItems[TimeEditorItemIndex].Digits[4] = tmpValue + '0';
  tmpTime = (uint64_t)( (uint64_t)tmpTime / 60 );
  tmpValue = (uint64_t)tmpTime % 60;
  TimeEditorItems[TimeEditorItemIndex].Digits[3] = tmpValue%10 + '0';
  tmpValue /= 10;
  TimeEditorItems[TimeEditorItemIndex].Digits[2] = tmpValue + '0';
  tmpTime = (uint64_t)( (uint64_t)tmpTime / 60 );
  tmpValue = (uint64_t)tmpTime % 100;
  TimeEditorItems[TimeEditorItemIndex].Digits[1] = tmpValue%10 + '0';
  tmpValue /= 10;
  TimeEditorItems[TimeEditorItemIndex].Digits[0] = tmpValue + '0';
}

void MenuAddTimeEditorItem ( int64_t* pTime ) {
  if ( NumOfTimeEditorItems < MAX_NUM_OF_TIME_EDITOR_ITEMS ) {
    TimeEditorItems[NumOfTimeEditorItems].pTime = pTime;
    TimeEditorItems[NumOfTimeEditorItems].Digits[0] = '0';
    TimeEditorItems[NumOfTimeEditorItems].Digits[1] = '0';
    TimeEditorItems[NumOfTimeEditorItems].Digits[2] = '0';
    TimeEditorItems[NumOfTimeEditorItems].Digits[3] = '0';
    TimeEditorItems[NumOfTimeEditorItems].Digits[4] = '0';
    TimeEditorItems[NumOfTimeEditorItems].Digits[5] = '0';
    TimeEditorItems[NumOfTimeEditorItems].CursorPosition = 7;
  DownloadTimeEditorItemValue ( NumOfTimeEditorItems );
    AddMenuItem ( TIME_EDITOR_ITEM, NumOfTimeEditorItems );
    NumOfTimeEditorItems++;
  }
}

void MenuAddMarkerItem ( char* pMarkerText, uint32_t Length, uint32_t Width ) {
  if ( NumOfMarkerItems < MAX_NUM_OF_MARKER_ITEMS ) {
    MarkerItems[NumOfMarkerItems].pMarkerText = pMarkerText;
    MarkerItems[NumOfMarkerItems].Length = Length;
    AddMenuItem (MARKER_ITEM, NumOfMarkerItems);
    NumOfMarkerItems++;
  }
}

void MenuAddVoidCommandItem ( void (*Action)(void), eCommandButtons CommandButtons ) {
  if ( NumOfVoidCommandItems < MAX_NUM_OF_VOID_COMMAND_ITEMS ) {
    VoidCommandItems[NumOfVoidCommandItems].Action = Action;
    VoidCommandItems[NumOfVoidCommandItems].ButtonKinds = CommandButtons;
    AddMenuItem ( VOID_COMMAND_ITEM, NumOfVoidCommandItems );
    NumOfVoidCommandItems++;
  }
}

void MenuAddIntCommandItem ( void (*Action) (int32_t Argument), int32_t Argument ) {
  if ( NumOfIntCommandItems < MAX_NUM_OF_INT_COMMAND_ITEMS ) {
    IntCommandItems[NumOfIntCommandItems].Action = Action;
    IntCommandItems[NumOfIntCommandItems].Argument = Argument;
    AddMenuItem ( INT_COMMAND_ITEM, NumOfIntCommandItems );
    NumOfIntCommandItems++;
  }
}

uint32_t tmpUZRU10AState;
char pEmptyText [] = "                ";

char chUmax2 [] = "Umax2 = ";
char chAmpers [] = " А";
char chVolts [] = " В";
char chAmperHours [] = " Ач";
char chStartCharge [] = "Начать заряд";
char chChargeIsGoing [] = "Идёт заряд током";
char chStageTime [] = "tэтапа=";
char chStageVolume [] = "Qэтапа=";
char chTotalTime [] = "tобщ";
char chTotalVolume [] = "Qобщ";
char chMenuEnd [] = "---конец-меню---";

int32_t IsEngeneeringMode = 0;
int32_t Password = 0;

void TryPassword ( int32_t Password ) {
  if ( Password == References[PASSWORD] ) {
    IsEngeneeringMode = 1;
  }
  else {
    IsEngeneeringMode = 0;
  }
  LoadMainMenu ();
}

void LoadPasswordMenu ( void ) {
   ClearMenu ();
   MenuIndex = PASSWORD_MENU;
   MenuAddLine ();
   MenuAddMarkerItem ("Пароль: ", 8, 8);
   MenuAddFixedPointNumberEditorItem ( &Password, 8, 8, 1, TryPassword, 0, 2000000000 );
}

void LoadDifficultModesMenu () {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadMainMenu;
  MenuIndex = ADDITIONAL_MODES_MENU;
  MenuAddLine ();
  MenuAddTextItem ("*Дополн. режимы*", 16, 16);
  MenuAddLine ();
  MenuAddMarkerItem ("Эксп. зар пост I", 16, 16);
  MenuAddVoidCommandItem ( LoadConstIChargeMenu, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddMarkerItem ("Эксп. зар пост U", 16, 16);
  MenuAddVoidCommandItem ( LoadConstUChargeMenu, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddMarkerItem ("Быстр комбин зар", 16, 16);
  MenuAddVoidCommandItem ( LoadFastCombineChargeMenu, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddMarkerItem ("Медленн комб зар", 16, 16);
  MenuAddVoidCommandItem ( LoadSlowCombineChargeMenu, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddMarkerItem ("Заряд I с огр. U", 16, 16);
  MenuAddVoidCommandItem ( LoadConstIWithLimitUChargeMenu, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddMarkerItem ("Заряд асимметр I", 16, 16);
  MenuAddVoidCommandItem ( LoadAsimmChargeMenu, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddMarkerItem ("Эксп. контр разр", 16, 16);
  MenuAddVoidCommandItem ( LoadSimpleControlDischargeMenu, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddMarkerItem ("КТЦ", 3, 16);
  MenuAddVoidCommandItem ( LoadCTCMenu, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddTextItem ("---конец-меню---", 16, 16);
}

extern int32_t SimpleConstIChargeStage1MaxVoltage;
extern int32_t SimpleConstIChargeStage1MaxCurrent;
extern int32_t SimpleConstIChargeStage2MaxCurrent;
extern int32_t SimpleConstIChargeStage2MaxVoltage;
extern uint64_t SimpleConstIChargeStage2MaxTime;
extern int32_t SimpleConstIChargeStage2MaxVoltageIntervalPerHour;

void LoadMenuCommands ( void );

int32_t SimpleControlDischargeCurrent = 100;

int32_t DischargeComboBox;

void LoadMainMenu ( void ) {
  int32_t tmpComboBox;
  tmpUZRU10AState = GetSubsystemState ( UZRU10A ) & CHARGE_MODE_MASK;
  ClearMenu ();
  MenuIndex = MAIN_MENU;
  MenuAddLine ();
  MenuAddTextItem ("**Главное меню**", 16, 16);
  if ( tmpUZRU10AState == 0xFF000000 ) {
    MenuAddLine (); MenuAddMarkerItem ("Заряд постоянн I", 16, 16); MenuAddVoidCommandItem ( LoadSimpleConstIChargeMenu, BTN_RIGHT_ONLY );
    MenuAddLine (); MenuAddMarkerItem ("Заряд постоянн U", 16, 16); MenuAddVoidCommandItem ( LoadSimpleConstUChargeMenu, BTN_RIGHT_ONLY );
    MenuAddLine (); MenuAddMarkerItem ("Ускор комб заряд", 16, 16); MenuAddVoidCommandItem ( LoadSimpleFastCombineChargeMenu, BTN_RIGHT_ONLY );
    MenuAddLine (); MenuAddMarkerItem ("Контрольн разряд", 16, 16); MenuAddVoidCommandItem ( LoadSimpleControlDischargeMenu, BTN_RIGHT_ONLY );
    MenuAddLine (); MenuAddMarkerItem ("КТЦ", 3, 16); MenuAddVoidCommandItem ( LoadSimpleCTCMenu, BTN_RIGHT_ONLY );
    MenuAddLine (); MenuAddMarkerItem ("Дополнит. режимы", 16, 16); MenuAddVoidCommandItem ( LoadDifficultModesMenu, BTN_RIGHT_ONLY );
  }
  else {
    if ( tmpUZRU10AState == SIMPLE_CONST_I_CHARGE_MASK ) { MenuAddLine (); MenuAddMarkerItem ("Заряд постоянн I", 16, 16); MenuAddVoidCommandItem ( LoadSimpleConstIChargeMenu, BTN_RIGHT_ONLY ); }
    if ( tmpUZRU10AState == SIMPLE_CONST_U_CHARGE_MASK ) { MenuAddLine (); MenuAddMarkerItem ("Заряд постоянн U", 16, 16); MenuAddVoidCommandItem ( LoadSimpleConstUChargeMenu, BTN_RIGHT_ONLY ); }
    if ( tmpUZRU10AState == SIMPLE_FAST_COMBINE_CHARGE_MASK ) { MenuAddLine (); MenuAddMarkerItem ("Ускоренный заряд", 16, 16); MenuAddVoidCommandItem ( LoadSimpleFastCombineChargeMenu, BTN_RIGHT_ONLY ); }
    if ( tmpUZRU10AState == SIMPLE_CONTROL_DISCHARGE_MASK ) { MenuAddLine (); MenuAddMarkerItem ("Контрольн разряд", 16, 16); MenuAddVoidCommandItem ( LoadSimpleControlDischargeMenu, BTN_RIGHT_ONLY ); }
    if ( tmpUZRU10AState == CONST_I_CHARGE_MASK ) { MenuAddLine (); MenuAddMarkerItem ("Эксп. зар пост I", 16, 16); MenuAddVoidCommandItem ( LoadConstIChargeMenu, BTN_RIGHT_ONLY ); }
    if ( tmpUZRU10AState == CONST_U_CHARGE_MASK ) { MenuAddLine (); MenuAddMarkerItem ("Эксп. зар пост U", 16, 16); MenuAddVoidCommandItem ( LoadConstUChargeMenu, BTN_RIGHT_ONLY ); }
    if ( tmpUZRU10AState == FAST_COMBINE_CHARGE_MASK ) { MenuAddLine (); MenuAddMarkerItem ("Быстр комбин зар", 16, 16); MenuAddVoidCommandItem ( LoadFastCombineChargeMenu, BTN_RIGHT_ONLY );  }
    if ( tmpUZRU10AState == SLOW_COMBINE_CHARGE_MASK ) { MenuAddLine (); MenuAddMarkerItem ("Медл комбин зар", 15, 16); MenuAddVoidCommandItem ( LoadSlowCombineChargeMenu, BTN_RIGHT_ONLY );  }
    if ( tmpUZRU10AState == CONST_I_WITH_LIMIT_U_CHARGE_MASK ) { MenuAddLine (); MenuAddMarkerItem ("Заряд I с огр. U", 16, 16); MenuAddVoidCommandItem ( LoadConstIWithLimitUChargeMenu, BTN_RIGHT_ONLY ); }
    if ( tmpUZRU10AState == SLOW_COMBINE_CHARGE_MASK ) { MenuAddLine (); MenuAddMarkerItem ("Медл комбин зар", 15, 16); MenuAddVoidCommandItem ( LoadSlowCombineChargeMenu, BTN_RIGHT_ONLY ); }
    if ( tmpUZRU10AState == ASIMM_CHARGE_MASK ) { MenuAddLine (); MenuAddMarkerItem ("Заряд асимметр I", 16, 16); MenuAddVoidCommandItem ( LoadAsimmChargeMenu, BTN_RIGHT_ONLY ); }
    if ( tmpUZRU10AState == CONTROL_DISCHARGE_MASK ) { MenuAddLine (); MenuAddMarkerItem ("Эксп. контр разр", 16, 16); MenuAddVoidCommandItem ( LoadControlDischargeMenu, BTN_RIGHT_ONLY ); }
    if ( tmpUZRU10AState == CTC_MASK ) { MenuAddLine (); MenuAddMarkerItem ("КТЦ", 3, 16); MenuAddVoidCommandItem ( LoadCTCMenu, BTN_RIGHT_ONLY ); }
    if ( tmpUZRU10AState == SIMPLE_CTC_MASK ) { MenuAddLine (); MenuAddMarkerItem ("КТЦ", 3, 16); MenuAddVoidCommandItem ( LoadSimpleCTCMenu, BTN_RIGHT_ONLY ); }
  }
  MenuAddLine ();
  MenuAddMarkerItem("Измерения", 9, 16);
  MenuAddVoidCommandItem ( LoadMeasuresMenu, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddMarkerItem("Аварии", 6, 16);
  MenuAddVoidCommandItem ( LoadFaultsMenu, BTN_RIGHT_ONLY );
  if ( IsEngeneeringMode ) {
    MenuAddLine ();
    MenuAddMarkerItem("Технологич. меню", 16, 16);
    MenuAddVoidCommandItem ( LoadMyMenu, BTN_RIGHT_ONLY );
  }
  MenuAddLine ();
  MenuAddTextItem (chMenuEnd, 16, 16);
}

void LoadAdvancedMenu () {
  
}

uint32_t tmpSubsystemState;

void LoadCurrentMenu ( void ) {
  switch ( MenuIndex ) {
    case CONST_I_CHARGE_MENU:
      LoadConstIChargeMenu ();
      break;
    case CONST_U_CHARGE_MENU:
      LoadConstUChargeMenu ();
      break;
    case FAST_COMBINE_CHARGE_MENU:
      LoadFastCombineChargeMenu ();
      break;
    case SLOW_COMBINE_CHARGE_MENU:
      LoadSlowCombineChargeMenu ();
      break;
    case CONST_I_WITH_LIMIT_U_CHARGE_MENU:
      LoadConstIWithLimitUChargeMenu ();
      break;
    case ASIMM_CHARGE_MENU:
      LoadAsimmChargeMenu ();
      break;
    case CONTROL_DISCHARGE_MENU:
      LoadControlDischargeMenu ();
      break;
    case CTC_MENU:
      LoadCTCMenu ();
      break;
    case MAIN_MENU:
      LoadMainMenu ();
      break;
    case FAULTS_MENU:
      LoadFaultsMenu ();
      break;
    case MEASURES_MENU:
      LoadMeasuresMenu ();
      break;
    case SIMPLE_CONST_I_CHARGE_MENU:
      LoadSimpleConstIChargeMenu ();
      break;
    case SIMPLE_CONST_U_CHARGE_MENU:
      LoadSimpleConstUChargeMenu ();
      break;
    case SIMPLE_FAST_COMBINE_CHARGE_MENU:
      LoadSimpleFastCombineChargeMenu ();
      break;
    case SIMPLE_CONTROL_DISCHARGE_MENU:
      LoadSimpleControlDischargeMenu ();
      break;
  case SIMPLE_CTC_MENU:
    LoadSimpleCTCMenu ();
    break;
  }
}

void StartMode ( void ) {
  SetRelayState ( HIGHLIGHT, ON );
  SetRelayState ( WORK_LED, ON );
  if ( GetSubsystemState ( UZRU10A ) == UZRU10A_OFF ) {
    StartNewRegulation ();
  }
  switch ( MenuIndex ) {
    case SIMPLE_CONST_I_CHARGE_MENU:
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONST_I_CHARGE_START_STAGE1 );
      break;
    case SIMPLE_CONST_U_CHARGE_MENU:
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONST_U_CHARGE_START_STAGE1 );
      break;
    case SIMPLE_FAST_COMBINE_CHARGE_MENU:
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_FAST_COMBINE_CHARGE_START_STAGE1 );
      break;
    case SIMPLE_CONTROL_DISCHARGE_MENU:
      SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CONTROL_DISCHARGE_START_DISCHARGE );
      break;
  case SIMPLE_CTC_MENU:
    SetSubsystemState ( UZRU10A, UZRU10A_SIMPLE_CTC_START_STAGE1 );
    break;
    case CONST_I_CHARGE_MENU:
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_CHARGE_START_STAGE1 );
      break;
    case CONST_U_CHARGE_MENU:
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_U_CHARGE_START_STAGE1 );
      break;
    case FAST_COMBINE_CHARGE_MENU:
      SetSubsystemState ( UZRU10A, UZRU10A_FAST_COMBINE_CHARGE_START_STAGE1 );
      break;
    case SLOW_COMBINE_CHARGE_MENU:
      SetSubsystemState ( UZRU10A, UZRU10A_SLOW_COMBINE_CHARGE_START_STAGE1 );
      break;
    case CONST_I_WITH_LIMIT_U_CHARGE_MENU:
      SetSubsystemState ( UZRU10A, UZRU10A_CONST_I_WITH_LIMIT_U_CHARGE_START_STAGE1 );
      break;
    case ASIMM_CHARGE_MENU:
      SetSubsystemState ( UZRU10A, UZRU10A_ASIMM_CHARGE_START_STAGE1 );
      break;
    case CONTROL_DISCHARGE_MENU:
      SetSubsystemState ( UZRU10A, UZRU10A_CONTROL_DISCHARGE_START_DISCHARGE );
      break;
    case CTC_MENU:
      SetSubsystemState ( UZRU10A, UZRU10A_CTC_START_STAGE1 );
      break;
  }
}

void ManualPauseMode ( void ) {
  SetRelayState ( WORK_LED, PULSE );
  ModeStopOrPauseReason = OPERATOR;
  SetSubsystemState ( UZRU10A, GetSubsystemState ( UZRU10A ) & ( CHARGE_MODE_MASK | 0x000000FF ) | STATE_PAUSE | MENU_MODE_PAUSED );
}

void ManualStopMode ( void ) {
  SetRelayState ( WORK_LED, OFF );
  ModeStopOrPauseReason = OPERATOR;
  SetSubsystemState ( UZRU10A, GetSubsystemState ( UZRU10A ) & CHARGE_MODE_MASK | STATE_STOP | MENU_MODE_STOPPED );
}

void PauseMode ( void ) {
  SetRelayState ( HIGHLIGHT, PULSE );
  SetRelayState ( WORK_LED, PULSE );
  SetSubsystemState ( UZRU10A, GetSubsystemState ( UZRU10A ) & ( CHARGE_MODE_MASK | 0x000000FF ) | STATE_PAUSE | MENU_MODE_PAUSED );
}

void StopMode ( void ) { 
  uint32_t tmpUzru10aState = GetSubsystemState ( UZRU10A );
  SetRelayState ( HIGHLIGHT, PULSE );
  SetRelayState ( WORK_LED, OFF );
  if ( ModeStopOrPauseReason == FAULT ) {
    if ( tmpUzru10aState & CHARGE_MODE_MASK ) { SetFault ( FAULT_MODE_CANCEL ); }
  }
  SetSubsystemState ( UZRU10A, GetSubsystemState ( UZRU10A ) & CHARGE_MODE_MASK | STATE_STOP | MENU_MODE_STOPPED );
}

void ClearMode ( void ) {
  SetSubsystemState ( UZRU10A, UZRU10A_CLEAR );
}

void ResumeMode ( void ) {
  SetSubsystemState ( UZRU10A, GetSubsystemState ( UZRU10A ) & ( CHARGE_MODE_MASK |  0x000000FF )  | STATE_START | MENU_MODE_WORKS );
}

void LoadBatteryVoltageCoeffMenu ();
void LoadBatteryVoltageCalibrationMenu ();
void LoadChargeCurrentCalibrationMenu ();
void LoadDischargeCurrentCalibrationMenu ();

// int32_t* BatteryVoltageCalibrations = &(Calibrations[BATTERY_VOLTAGE_ON_0_PREVALUE_INDEX]);
// int32_t* ChargeCurrentCalibrations = &(Calibrations[CHARGE_CURRENT_ON_0_PREVALUE_INDEX]);
// int32_t* DischargeCurrentCalibrations = &(Calibrations[DISCHARGE_CURRENT_ON_0_PREVALUE_INDEX]);
int32_t CalibrationIndex = 0;

void StartBatteryVoltageMeasureCalibration () {
  CalibrationIndex = 1;
  SetSubsystemState ( UZRU10A, UZRU10A_START_BATTERY_VOLTAGE_MEASURE_CALIBRATION );
  LoadBatteryVoltageCalibrationMenu ();
}

void CalibrateBatteryVoltage ( int32_t NewBatteryVoltageCalibration ) {
  BatteryVoltageMeasureCalibrations[CalibrationIndex] = NewBatteryVoltageCalibration;
  CalibrationIndex++;
  LoadBatteryVoltageCalibrationMenu ();
}

void CalibrateChargeCurrent ( int32_t NewChargeCurrentCalibration ) {
  ChargeCurrentMeasureCalibrations[CalibrationIndex] = NewChargeCurrentCalibration;
  CalibrationIndex++;
  LoadChargeCurrentCalibrationMenu ();
}

void CalibrateDischargeCurrent ( int32_t NewDischargeCurrentCalibration ) {
  DischargeCurrentMeasureCalibrations[CalibrationIndex] = NewDischargeCurrentCalibration;
  CalibrationIndex++;
  LoadDischargeCurrentCalibrationMenu ();
}

void StartChargeCurrentMeasureCalibration () {
  SetSubsystemState ( UZRU10A, UZRU10A_START_CHARGE_CURRENT_MEASURE_CALIBRATION );
  CalibrationIndex = 1;
  LoadChargeCurrentCalibrationMenu ();
}

void StartDischargeCurrentMeasureCalibration () {
  SetSubsystemState ( UZRU10A, UZRU10A_START_DISCHARGE_CURRENT_MEASURE_CALIBRATION );
  CalibrationIndex = 1;
  LoadDischargeCurrentCalibrationMenu ();
}

void CancelChargeCurrentCalibration () {
   if ( CalibrationIndex == 0 ) {
      SetSubsystemState ( UZRU10A, UZRU10A_STOP_CHARGE_CURRENT_MEASURE_CALIBRATION );
      LoadMyMenu ();
   }
   else {
      CalibrationIndex--;
    LoadChargeCurrentCalibrationMenu ();
   }
}

void CancelBatteryVoltageCalibration () {
   if ( CalibrationIndex == 0 ) {
      SetSubsystemState ( UZRU10A, UZRU10A_STOP_BATTERY_VOLTAGE_MEASURE_CALIBRATION );
      LoadMyMenu ();
   }
   else {
      CalibrationIndex--;
    LoadBatteryVoltageCalibrationMenu ();
   }
}

void LoadChargeCurrentCalibrationMenu () {
  ClearMenu ();
  MenuOnLeftButtonPress = CancelChargeCurrentCalibration;
    if ( CalibrationIndex < 17 ) {
     SetTargetChargeCurrentSignal ( 16 * CalibrationIndex );
     SetTargetChargeVoltageSignal ( 4095 );
     MenuAddLine ();
     MenuAddMarkerItem ( "Iзар=", 5, 5 ); MenuAddFixedPointNumberEditorItem ( &(ChargeCurrentMeasureCalibrations[CalibrationIndex]), 5, 2, 100, CalibrateChargeCurrent, 0, 100000 ); MenuAddTextItem("~", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 5, 2, 100 );
     MenuAddLine();
     MenuAddMarkerItem ( "Rпр=", 4, 4); MenuAddFixedPointNumberEditorItem ( &(Calibrations[WIRES_RESISTANCE_INDEX]), 5, 1, 1000, 0, 0, 1000000 ); MenuAddTextItem("U=", 2, 2); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
     }
   else if ( CalibrationIndex < 31 ) {
     SetTargetChargeCurrentSignal ( 256 * ( CalibrationIndex - 15 ) );
     SetTargetChargeVoltageSignal ( 4095 );
     MenuAddLine ();
     MenuAddMarkerItem ( "Iзар=", 5, 5 ); MenuAddFixedPointNumberEditorItem ( &(ChargeCurrentMeasureCalibrations[CalibrationIndex]), 5, 2, 100, CalibrateChargeCurrent, 0, 100000 ); MenuAddTextItem("~", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 5, 2, 100 );
     MenuAddLine();
     MenuAddMarkerItem ( "Rпр=", 4, 4); MenuAddFixedPointNumberEditorItem ( &(Calibrations[WIRES_RESISTANCE_INDEX]), 5, 1, 1000, 0, 0, 100000 ); MenuAddTextItem("U=", 2, 2); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
   }
   else {
		 Calibrations[CHARGE_CURRENT_ON_4096_PREVALUE_INDEX] = 2 * Calibrations[CHARGE_CURRENT_ON_3840_PREVALUE_INDEX] - Calibrations[CHARGE_CURRENT_ON_3584_PREVALUE_INDEX];
     UpdateInEEPROM ( Calibrations );
     SetSubsystemState ( UZRU10A, UZRU10A_STOP_CHARGE_CURRENT_MEASURE_CALIBRATION );
     LoadMyMenu ();
   }
  MenuAddLine ();
  MenuAddTextItem ("---конец-меню---", 16, 16);
}

void CancelDischargeCurrentCalibration () {
   if ( CalibrationIndex == 0 ) {
      SetSubsystemState ( UZRU10A, UZRU10A_STOP_DISCHARGE_CURRENT_MEASURE_CALIBRATION );
      LoadMyMenu ();
   }
   else {
      CalibrationIndex--;
    LoadDischargeCurrentCalibrationMenu ();
   }
}

void LoadBatteryVoltageCalibrationMenu () {
   ClearMenu ();
   MenuOnLeftButtonPress = CancelBatteryVoltageCalibration;
   if ( CalibrationIndex < 16 ) {
     SetTargetChargeVoltageSignal ( 256 * CalibrationIndex );
     SetTargetChargeCurrentSignal ( 4095 );
     MenuAddLine ();
     MenuAddMarkerItem ( "Uакб=", 5, 5 ); MenuAddFixedPointNumberEditorItem ( &(BatteryVoltageMeasureCalibrations[CalibrationIndex]), 5, 2, 100, CalibrateBatteryVoltage, 0, 100000 ); MenuAddTextItem("~", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 5, 2, 100 );
   }
   else {
		 Calibrations[BATTERY_VOLTAGE_ON_4096_PREVALUE_INDEX] = 2 * Calibrations[BATTERY_VOLTAGE_ON_3840_PREVALUE_INDEX] - Calibrations[BATTERY_VOLTAGE_ON_3584_PREVALUE_INDEX];
     UpdateInEEPROM ( Calibrations );
     SetSubsystemState ( UZRU10A, UZRU10A_STOP_BATTERY_VOLTAGE_MEASURE_CALIBRATION );
     LoadMyMenu ();
   }
  MenuAddLine ();
  MenuAddTextItem ("---конец-меню---", 16, 16);
}

void LoadDischargeCurrentCalibrationMenu () {
  ClearMenu ();
  MenuOnLeftButtonPress = CancelDischargeCurrentCalibration;
  if ( CalibrationIndex < 16 ) {
    SetTargetDischargeCurrentSignal ( 32 * CalibrationIndex );
    MenuAddLine ();
    MenuAddMarkerItem ( "Iрзр=", 5, 5 ); MenuAddFixedPointNumberEditorItem ( &(DischargeCurrentMeasureCalibrations[CalibrationIndex]), 5, 2, 100, CalibrateDischargeCurrent, 0, 100000 ); MenuAddTextItem("~", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[DISCHARGE_CURRENT]), 5, 2, 100 );
  }
  else if ( CalibrationIndex < 31 ) {
    SetTargetDischargeCurrentSignal ( 256 * ( CalibrationIndex - 14 ) );
    MenuAddLine ();
    MenuAddMarkerItem ( "Iрзр=", 5, 5 ); MenuAddFixedPointNumberEditorItem ( &(DischargeCurrentMeasureCalibrations[CalibrationIndex]), 5, 2, 100, CalibrateDischargeCurrent, 0, 100000 ); MenuAddTextItem("~", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[DISCHARGE_CURRENT]), 5, 2, 100 );
  }
  else {
		Calibrations[DISCHARGE_CURRENT_ON_4096_PREVALUE_INDEX] = 2 * Calibrations[DISCHARGE_CURRENT_ON_3840_PREVALUE_INDEX] - Calibrations[DISCHARGE_CURRENT_ON_3584_PREVALUE_INDEX];
    UpdateInEEPROM ( Calibrations );
    SetSubsystemState ( UZRU10A, UZRU10A_STOP_DISCHARGE_CURRENT_MEASURE_CALIBRATION );
    LoadMyMenu ();
  }
  MenuAddLine ();
  MenuAddTextItem ("---конец-меню---", 16, 16);
}

void LoadBatteryVoltageCoeffMenu () {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadMyMenu;
  MenuAddLine ();
  MenuAddTextItem("*Табл знач Uакб*", 16, 16 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uакб", 4, 4 ); MenuAddMarkerItem ( Arguments[0], 6, 6 ); MenuAddTextItem("=", 1, 1); MenuAddFixedPointNumberEditorItem (&(BatteryVoltageMeasureCalibrations[0]), 5, 2, 100, 0, 0, 100000 );
  for ( i = 1; i < 17; i++ ) {
    MenuAddLine ();
    MenuAddMarkerItem ( "Uакб", 4, 4 ); MenuAddMarkerItem ( Arguments[i+15], 6, 6 ); MenuAddTextItem("=", 1, 1); MenuAddFixedPointNumberEditorItem (&(BatteryVoltageMeasureCalibrations[i]), 5, 2, 100, 0, 0, 100000 );
  }
  MenuAddLine ();
  MenuAddMarkerItem ("Сохранить в ПЗУ", 15, 16 ); MenuAddVoidCommandItem ( SaveCalibrations, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddTextItem ("---конец-меню---", 16, 16);
}

void LoadChargeCurrentCoeffMenu () {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadMyMenu;
  MenuAddLine ();
  MenuAddTextItem("*Табл знач Iзар*", 16, 16 );
  for ( i = 0; i < 32; i++ ) {
    MenuAddLine ();
    MenuAddMarkerItem ( "Iзар", 4, 4 ); MenuAddMarkerItem ( Arguments[i], 6, 6 ); MenuAddTextItem("=", 1, 1); MenuAddFixedPointNumberEditorItem (&(ChargeCurrentMeasureCalibrations[i]), 5, 2, 100, 0, 0, 100000 );
  }
  MenuAddLine ();
  MenuAddMarkerItem ("Сохранить в ПЗУ", 15, 16 ); MenuAddVoidCommandItem ( SaveCalibrations, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddTextItem ("---конец-меню---", 16, 16);
}

void LoadDischargeCurrentCoeffMenu () {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadMyMenu;
  MenuAddLine ();
  MenuAddTextItem("*Табл знач Iрзр*", 16, 16 );
  for ( i = 0; i < 31; i++ ) {
    MenuAddLine ();
    MenuAddMarkerItem ( "Iрзр", 4, 4 ); MenuAddMarkerItem ( DischargeArguments[i], 6, 6 ); MenuAddTextItem("=", 1, 1); MenuAddFixedPointNumberEditorItem (&(DischargeCurrentMeasureCalibrations[i]), 5, 2, 100, 0, 0, 100000 );
  }
  MenuAddLine ();
  MenuAddMarkerItem ("Сохранить в ПЗУ", 15, 16 ); MenuAddVoidCommandItem ( SaveCalibrations, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddTextItem ("---конец-меню---", 16, 16);
}

// int32_t* ChargeVoltageSettings = &(Calibrations[ON_CHARGE_0_00_VDC_TO_DAC_UNITS_INDEX]);
/*
void LoadChargeVoltageSettingsMenu () {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadMyMenu;
  MenuAddLine ();
  MenuAddTextItem("*Табл уст. Uзар*", 16, 16 );
  for ( i = 0; i < 17; i++ ) {
    MenuAddLine ();
    MenuAddMarkerItem ( ChargeVoltageValues[i], 4, 4 ); MenuAddTextItem ( "=U(", 3, 3 ); MenuAddFixedPointNumberEditorItem ( &(ChargeVoltageSettingsCalibrations[i]), 4, 4, 1, 0, 0, 100000 ); MenuAddTextItem(")", 1, 1);
  }
  MenuAddLine ();
  MenuAddMarkerItem ("Сохранить в ПЗУ", 15, 16 ); MenuAddVoidCommandItem ( SaveCalibrations, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddTextItem ("---конец-меню---", 16, 16);
}

int32_t* ChargeCurrentSettings = &(Calibrations[ON_CHARGE_0_ADC_TO_DAC_UNITS_INDEX]);

void LoadChargeCurrentSettingsMenu () {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadMyMenu;
  MenuAddLine ();
  MenuAddTextItem("*Табл уст. Iзар*", 16, 16 );
  for ( i = 0; i < 37; i++ ) {
    MenuAddLine ();
    MenuAddMarkerItem ( ChargeCurrentValues[i], 5, 5 ); MenuAddTextItem ( "=I(", 3, 3 ); MenuAddFixedPointNumberEditorItem (&(ChargeCurrentSettings[i]), 4, 4, 1, 0, 0, 100000 ); MenuAddTextItem(")", 1, 1);
  }
  MenuAddLine ();
  MenuAddMarkerItem ("Сохранить в ПЗУ", 15, 16 ); MenuAddVoidCommandItem ( SaveCalibrations, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddTextItem ("---конец-меню---", 16, 16);
}
*/
void LoadReferencesMenu () {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadMyMenu;
  MenuAddLine ();
  MenuAddTextItem("****Уставки*****", 16, 16);
  MenuAddLine ();
  MenuAddMarkerItem("Пароль: ", 8, 8);
  MenuAddFixedPointNumberEditorItem ( &( References[PASSWORD] ), 8, 8, 1, 0, 0, 2000000000 );
  MenuAddLine ();
  MenuAddTextItem ( "--Заряд-пост-I--", 16, 16 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс1(24)=", 11, 11 ); MenuAddFixedPointNumberEditorItem ( &(References[SIMPLE_CONST_I_CHARGE_24V_1ST_STAGE_MAX_VOLTAGE]), 4, 2, 100, 0, 0, 8000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс2(24)=", 11, 11 ); MenuAddFixedPointNumberEditorItem ( &(References[SIMPLE_CONST_I_CHARGE_24V_2ND_STAGE_MAX_VOLTAGE]), 4, 2, 100, 0, 0, 8000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "dUмакс(24)=", 11, 11 ); MenuAddFixedPointNumberEditorItem ( &(References[SIMPLE_CONST_I_CHARGE_24V_2ND_STAGE_MAX_DELTA_VOLTAGE]), 4, 2, 100, 0, 0, 4000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс1(12)=", 11, 11); MenuAddFixedPointNumberEditorItem ( &(References[SIMPLE_CONST_I_CHARGE_12V_1ST_STAGE_MAX_VOLTAGE]), 4, 2, 100, 0, 0, 8000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс2(12)=", 11, 11 ); MenuAddFixedPointNumberEditorItem ( &(References[SIMPLE_CONST_I_CHARGE_12V_2ND_STAGE_MAX_VOLTAGE]), 4, 2, 100, 0, 0, 8000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "dUмакс(12)=", 11, 11 ); MenuAddFixedPointNumberEditorItem ( &(References[SIMPLE_CONST_I_CHARGE_12V_2ND_STAGE_MAX_DELTA_VOLTAGE]), 4, 2, 100, 0, 0, 4000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "tзар2=", 6, 6 ); MenuAddFixedPointNumberEditorItem ( &(References[SIMPLE_CONST_I_CHARGE_2ND_STAGE_MIN_TIME]), 2, 2, 1, 0, 3, 60 ); MenuAddTextItem (" мин", 4, 4);
  MenuAddLine ();
  MenuAddTextItem ( "--Заряд-пост-U--", 16, 16 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uзар(24)=", 9, 9 ); MenuAddFixedPointNumberEditorItem ( &(References[SIMPLE_CONST_U_CHARGE_24V_CHARGE_VOLTAGE]), 4, 2, 100, 0, 0, 4000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uзар(12)=", 9, 9 ); MenuAddFixedPointNumberEditorItem ( &(References[SIMPLE_CONST_U_CHARGE_12V_CHARGE_VOLTAGE]), 4, 2, 100, 0, 0, 4000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddTextItem ( "--Контр-разряд--", 16, 16 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uразр(21)=", 10, 10 ); MenuAddFixedPointNumberEditorItem ( &(References[SIMPLE_CONTROL_DISCHARGE_21V_MIN_VOLTAGE]), 4, 2, 100, 0, 0, 8000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uразр(20,4)=", 12, 12 ); MenuAddFixedPointNumberEditorItem ( &(References[SIMPLE_CONTROL_DISCHARGE_20_4V_MIN_VOLTAGE]), 4, 2, 100, 0, 0, 8000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uразр(10,5)=", 12, 12 ); MenuAddFixedPointNumberEditorItem ( &(References[SIMPLE_CONTROL_DISCHARGE_10_5V_MIN_VOLTAGE]), 4, 2, 100, 0, 0, 8000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uразр(10,2)=", 12, 12 ); MenuAddFixedPointNumberEditorItem ( &(References[SIMPLE_CONTROL_DISCHARGE_10_2V_MIN_VOLTAGE]), 4, 2, 100, 0, 0, 8000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddTextItem ( "------КТЦ-------", 16, 16 );
  MenuAddLine ();
  MenuAddMarkerItem ( "tпауз=", 6, 6 ); MenuAddTimeEditorItem ( (uint64_t*)(&(References[SIMPLE_CTC_PAUSE_TIME_HW])) );
  MenuAddLine ();
  MenuAddTextItem ( "----Уставки-----", 16, 16 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uаботкл=", 8, 8 ); MenuAddFixedPointNumberEditorItem ( &(References[NO_BATTERY_VOLTAGE]), 4, 2, 100, 0, 0, 8000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uабмин=", 7, 7 ); MenuAddFixedPointNumberEditorItem ( &(References[MIN_BATTERY_VOLTAGE]), 4, 2, 100, 0, 0, 8000 ); MenuAddTextItem (" В", 2, 2);  
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс(24)=", 10, 10 ); MenuAddFixedPointNumberEditorItem ( &(References[MAX_24V_BATTERY_VOLTAGE]), 4, 2, 100, 0, 0, 8000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмин(24)=", 9, 9 ); MenuAddFixedPointNumberEditorItem ( &(References[MIN_24V_BATTERY_VOLTAGE]), 4, 2, 100, 0, 0, 8000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс(12)=", 10, 10 ); MenuAddFixedPointNumberEditorItem ( &(References[MAX_12V_BATTERY_VOLTAGE]), 4, 2, 100, 0, 0, 8000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмин(12)=", 9, 9 ); MenuAddFixedPointNumberEditorItem ( &(References[MIN_12V_BATTERY_VOLTAGE]), 4, 2, 100, 0, 0, 8000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uсетимин=", 9, 9 ); MenuAddFixedPointNumberEditorItem ( &(References[MIN_NETWORK_VOLTAGE]), 4, 2, 100, 0, 0, 8000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uсетимакс=", 10, 10 ); MenuAddFixedPointNumberEditorItem ( &(References[MAX_NETWORK_VOLTAGE]), 4, 2, 100, 0, 0, 8000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uфильтмакс=", 11, 11 ); MenuAddFixedPointNumberEditorItem ( &(References[MAX_SOURCE_DC_CONVERTER_VOLTAGE]), 4, 2, 100, 0, 0, 8000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uфильтмин=", 10, 10 ); MenuAddFixedPointNumberEditorItem ( &(References[MIN_SOURCE_DC_CONVERTER_VOLTAGE]), 4, 2, 100, 0, 0, 8000 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "Tмакс=", 6, 6 ); MenuAddFixedPointNumberEditorItem ( &(References[MAX_RADIATOR_TEMP]), 4, 2, 100, 0, 0, 8000 ); MenuAddTextItem (" °C", 3, 3);
  MenuAddLine ();
  MenuAddMarkerItem ( "Tнорм=", 6, 6 ); MenuAddFixedPointNumberEditorItem ( &(References[NORM_RADIATOR_TEMP]), 4, 2, 100, 0, 0, 8000 ); MenuAddTextItem (" °C", 3, 3);
  MenuAddLine ();
  MenuAddMarkerItem ("Сохранить в ПЗУ", 15, 16 ); MenuAddVoidCommandItem ( SaveReferences, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddTextItem ("---конец-меню---", 16, 16);
}

void LoadParametersMenu () {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadMyMenu;
  MenuAddLine ();
  MenuAddTextItem("***Параметры****", 16, 16);
  MenuAddLine ();
  MenuAddMarkerItem("Rпров=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &(Calibrations[WIRES_RESISTANCE_INDEX]), 5, 1, 1000, 0, 0, 1000 ); MenuAddTextItem (" Ом", 3, 3 );
  MenuAddLine ();
  MenuAddMarkerItem ("Сохранить в ПЗУ", 15, 16 ); MenuAddVoidCommandItem ( SaveCalibrations, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddTextItem ("---конец-меню---", 16, 16);
}

void StartChargeVoltageCalibration () {
  /*
  CalibrationIndex = 1;
  SetSubsystemState ( UZRU10A, UZRU10A_START_CHARGE_VOLTAGE_CALIBRATION );
  LoadChargeVoltageCalibrationMenu ();
  //*/
}

void StartChargeCurrentCalibration () {
  /*
  CalibrationIndex = 1;
  SetSubsystemState ( UZRU10A, UZRU10A_START_CHARGE_CURRENT_CALIBRATION );
  LoadChargeCurrentCalibrationMenu ();
  //*/
}

void LoadMyMenu ( void ) {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadMainMenu;
  MenuAddLine ();
  MenuAddTextItem ( "*Технолог. меню*", 16, 16 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Калибр изм Uакб", 15, 16 );
  MenuAddVoidCommandItem ( StartBatteryVoltageMeasureCalibration, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddMarkerItem ( "Калибр изм Iзар", 15, 16 );
  MenuAddVoidCommandItem ( StartChargeCurrentMeasureCalibration, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddMarkerItem ( "Калибр изм Iразр", 16, 16 );
  MenuAddVoidCommandItem ( StartDischargeCurrentMeasureCalibration, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddMarkerItem ( "Автоподстр Uзар", 15, 16 );
  MenuAddVoidCommandItem ( StartChargeVoltageCalibration, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddMarkerItem ( "Автоподстр Iзар", 15, 16 );
  MenuAddVoidCommandItem ( StartChargeCurrentCalibration, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddMarkerItem ( "Знач измер Uакб", 15, 16 );
  MenuAddVoidCommandItem ( LoadBatteryVoltageCoeffMenu, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddMarkerItem ( "Знач измер Iзар", 15, 16 );
  MenuAddVoidCommandItem ( LoadChargeCurrentCoeffMenu, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddMarkerItem ( "Знач измер Iрзр", 15, 16 );
  MenuAddVoidCommandItem ( LoadDischargeCurrentCoeffMenu, BTN_RIGHT_ONLY );
  MenuAddLine ();
  /*
  MenuAddMarkerItem ( "Знач устав Uзар", 15, 16 );
  MenuAddVoidCommandItem ( LoadChargeVoltageSettingsMenu, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddMarkerItem ( "Знач устав Iзар", 15, 16 );
  MenuAddVoidCommandItem ( LoadChargeCurrentSettingsMenu, BTN_RIGHT_ONLY );
  MenuAddLine ();
  //*/
  MenuAddMarkerItem ( "Параметры", 9, 16 );
  MenuAddVoidCommandItem ( LoadParametersMenu, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddMarkerItem ( "Уставки", 7, 7 );
  MenuAddVoidCommandItem ( LoadReferencesMenu, BTN_RIGHT_ONLY );
  MenuAddLine ();
  MenuAddTextItem (chMenuEnd, 16, 16);
}

void LoadMenuCommands ( void ) {
  tmpUZRU10AState = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
  if ( tmpUZRU10AState == 0 ) {
    MenuAddLine ();
    MenuAddMarkerItem ( chStartCharge, 12, 16 );
    MenuAddVoidCommandItem ( StartMode, BTN_START_ONLY );
  }
  else if ( tmpUZRU10AState == MENU_MODE_WORKS ) {
    MenuAddLine ();
    MenuAddMarkerItem ( "Приостановить", 13, 16 );
    MenuAddVoidCommandItem ( ManualPauseMode, BTN_RIGHT_ONLY );
    MenuAddLine ();
    MenuAddMarkerItem ( "Завершить заряд ", 16, 16 );
    MenuAddVoidCommandItem ( ManualStopMode, BTN_RIGHT_ONLY );
  }
  else if ( tmpUZRU10AState == MENU_MODE_PAUSED ) {
    MenuAddLine ();
    MenuAddMarkerItem ( "Продолжить заряд", 16, 16 );
    MenuAddVoidCommandItem ( ResumeMode, BTN_START_ONLY );
    if ( ModeStopOrPauseReason != OPERATOR ) {
      MenuAddLine ();
      MenuAddMarkerItem ( "Приостановить", 13, 16 );
      MenuAddVoidCommandItem ( ManualPauseMode, BTN_RIGHT_ONLY );
    }
    MenuAddLine ();
    MenuAddMarkerItem ( "Завершить заряд ", 16, 16 );
    MenuAddVoidCommandItem ( ManualStopMode, BTN_RIGHT_ONLY );
  }
  else if ( tmpUZRU10AState == MENU_MODE_STOPPED ) {
    MenuAddLine ();
    MenuAddMarkerItem ( "Сбросить данные", 15, 16 );
    MenuAddVoidCommandItem ( ClearMode, BTN_RIGHT_ONLY );
  }
}

extern int32_t ConstIChargeStage1MaxCurrent;
extern int32_t ConstIChargeStage1MaxVoltage;

void LoadConstIChargeMenuStage1Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &ConstIChargeStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &ConstIChargeStage1MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
}

void LoadConstIChargeMenuStage1Executing ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 ); // Выводим сообщение "Iз=00,00<__,__ А
  MenuAddFixedPointNumberEditorItem ( &ConstIChargeStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 );  MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 2, 100 ); MenuAddTextItem ( "А", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberEditorItem ( &ConstIChargeStage1MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 ); MenuAddTextItem ( "В", 1, 1 );
}

void LoadConstIChargeMenuStage1Statistics ( void ) {
  if ( Stage1Reached ) {
    MenuAddLine ();
    MenuAddTextItem ( "Uзар=", 5, 5 );  // Выводим сообщение "Uз=24,63...26,77 В"
    MenuAddFixedPointNumberPlayerItem ( &Stage1Umin, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Umax, 4, 2, 100 );
    MenuAddTextItem ( chVolts, 2, 3 );
    MenuAddLine ();   // Выводим сообщение "tэтапа=24,63...26,77 В"
    MenuAddTextItem ( chStageTime, 7, 7 ); MenuAddTimePlayerItem ( &Stage1Time );
    MenuAddLine ();   // Выводим сообщение "Qэтапа=532,64 Ач"
    MenuAddTextItem ( chStageVolume, 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
  }
  else {
    MenuAddLine ();
    MenuAddTextItem ( 0, 16, 16 );
  }
}

void LoadConstIChargeMenuStage1Over ( void ) {
   MenuAddLine ();
   MenuAddTextItem ( "Iзар=", 5, 5 );
   MenuAddFixedPointNumberPlayerItem ( &ConstIChargeStage1MaxCurrent, 4, 2, 100 ); // Выводим сообщение "Iзар1=24,64 А" без возможности редактирования значения
   MenuAddTextItem ( chAmpers, 2, 7 );
   MenuAddLine ();
   MenuAddTextItem ( "Uмакс=", 6, 6 );
   MenuAddFixedPointNumberPlayerItem ( &ConstIChargeStage1MaxVoltage, 4, 2, 100 ); // Выводим сообщение "Umax1=24,64 В" без возможности редактирования значения
   MenuAddTextItem ( chVolts, 2, 7 );
}

void LoadConstIChargeMenuStage1 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadConstIChargeMenuStage1Tune ();
    return;
   }
   else if ( tmpChargeControllerStage == MENU_MODE_WORKS ) {
      if ( Stage2Reached ) { // Если достигнут этап 2, то...
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16); // ...то этап 1 завершен,
     LoadConstIChargeMenuStage1Over ();         // Выводим настройки режима без возможности редактирования
     LoadConstIChargeMenuStage1Statistics ();   // Выводим статистику блока
    }
      else if ( Stage1Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadConstIChargeMenuStage1Executing ();
     LoadConstIChargeMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadConstIChargeMenuStage1Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_PAUSED ) {
      if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadConstIChargeMenuStage1Over ();
     LoadConstIChargeMenuStage1Statistics ();
    }
      else if ( Stage1Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап приостановл", 16, 16);
       LoadConstIChargeMenuStage1Tune ();
     LoadConstIChargeMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadConstIChargeMenuStage1Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_STOPPED ) {
      if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadConstIChargeMenuStage1Over ();
     LoadConstIChargeMenuStage1Statistics ();
    }
      else if ( Stage1Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadConstIChargeMenuStage1Over ();
     LoadConstIChargeMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadConstIChargeMenuStage1Over ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
}

extern int32_t ConstIChargeStage2MaxCurrent;
extern int32_t ConstIChargeStage2MaxVoltage;

void LoadConstIChargeMenuStage2Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &ConstIChargeStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( " А", 2, 7 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &ConstIChargeStage2MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
}

void LoadConstIChargeMenuStage2Executing ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 ); // Выводим сообщение "Iз=00,00<__,__ А
  MenuAddFixedPointNumberEditorItem ( &ConstIChargeStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 );  MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 2, 100 ); MenuAddTextItem ( "А", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberEditorItem ( &ConstIChargeStage2MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 ); MenuAddTextItem ( "В", 1, 1 );
}

void LoadConstIChargeMenuStage2Statistics ( void ) {
  if ( Stage2Reached ) {
    MenuAddLine ();
    MenuAddTextItem ( "Uзар=", 5, 5 );  // Выводим сообщение "Uз=24,63...26,77 В"
    MenuAddFixedPointNumberPlayerItem ( &Stage2Umin, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage2Umax, 4, 2, 100 );
    MenuAddTextItem ( chVolts, 2, 3 );
    MenuAddLine ();   // Выводим сообщение "tэтапа=24,63...26,77 В"
    MenuAddTextItem ( chStageTime, 7, 7 ); MenuAddTimePlayerItem ( &Stage2Time );
    MenuAddLine ();   // Выводим сообщение "Qэтапа=532,64 Ач"
    MenuAddTextItem ( chStageVolume, 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &Stage2Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
  }
  else {
    MenuAddLine ();
    MenuAddTextItem ( 0, 16, 16 );
  }
}

void LoadConstIChargeMenuStage2Over ( void ) {
   MenuAddLine ();
   MenuAddTextItem ( "Iзар=", 5, 5 );
   MenuAddFixedPointNumberPlayerItem ( &ConstIChargeStage2MaxCurrent, 4, 2, 100 ); // Выводим сообщение "Iзар1=24,64 А" без возможности редактирования значения
   MenuAddTextItem ( chAmpers, 2, 7 );
   MenuAddLine ();
   MenuAddTextItem ( "Uмакс=", 6, 6 );
   MenuAddFixedPointNumberPlayerItem ( &ConstIChargeStage2MaxVoltage, 4, 2, 100 ); // Выводим сообщение "Umax1=24,64 В" без возможности редактирования значения
   MenuAddTextItem ( chVolts, 2, 7 );
}

void LoadConstIChargeMenuStage2 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadConstIChargeMenuStage2Tune ();
    return;
   }
   else if ( tmpChargeControllerStage == MENU_MODE_WORKS ) {
      if ( Stage3Reached ) { // Если достигнут этап 2, то...
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16); // ...то этап 1 завершен,
     LoadConstIChargeMenuStage2Over ();         // Выводим настройки режима без возможности редактирования
     LoadConstIChargeMenuStage2Statistics ();   // Выводим статистику блока
    }
      else if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadConstIChargeMenuStage2Executing ();
     LoadConstIChargeMenuStage2Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadConstIChargeMenuStage2Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_PAUSED ) {
      if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadConstIChargeMenuStage2Over ();
     LoadConstIChargeMenuStage2Statistics ();
    }
      else if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап приостановл", 16, 16);
       LoadConstIChargeMenuStage2Tune ();
     LoadConstIChargeMenuStage2Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadConstIChargeMenuStage2Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_STOPPED ) {
      if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadConstIChargeMenuStage2Over ();
     LoadConstIChargeMenuStage2Statistics ();
    }
      else if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadConstIChargeMenuStage2Over ();
     LoadConstIChargeMenuStage2Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadConstIChargeMenuStage2Over ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
}

void LoadConstIChargeMenuStatistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "tобщ = ", 7, 7 ); MenuAddTimePlayerItem ( &TotalTime );
  MenuAddLine ();
  MenuAddTextItem ( "Qобщ = ", 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &TotalVolume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadConstIChargeMenu ( void ) {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadDifficultModesMenu;
  MenuIndex = CONST_I_CHARGE_MENU;
  MenuAddLine ();
  MenuAddTextItem ("**Заряд пост. I*", 16, 16);
  tmpSubsystemState = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
  if ( tmpSubsystemState == 0 ) {
    MenuAddLine ();
    MenuAddTextItem ( "Настройка режима", 16, 16 );
  }
  else if ( tmpSubsystemState == MENU_MODE_WORKS ) {
      // Одна из ступеней режима заряда постоянным током
      if ( Stage3Reached ) { // Если достигнута последняя ступень, то выводим соответствующее сообщение
      MenuAddLine ();
      MenuAddTextItem ("Идёт завершение", 15, 16 );
      MenuAddLine ();
      MenuAddTextItem ("заряда…", 7, 16 );
    }
    else {
      MenuAddLine ();
        MenuAddTextItem ( "Идёт заряд током", 16, 16 );
      if (Stage2Reached ) { // Если последняя ступень не достигнута, но достигнута вторая ступень, то выводим соответствующее сообщение
        MenuAddLine ();
        MenuAddTextItem ("второй ступени…", 15, 16 );
      }
      else if ( Stage1Reached ) { // Если вторая ступень не достигнута, но достигнута первая ступень, то выводим соответствующее сообщение
        MenuAddLine ();
        MenuAddTextItem ("первой ступени…", 15, 16 );
      }
    }
  }
  else {
    if ( tmpSubsystemState == MENU_MODE_PAUSED ) {
      MenuAddLine ();
      MenuAddTextItem ("Приостановлен по", 16, 16 );
    }
    else if ( tmpSubsystemState == MENU_MODE_STOPPED ) {
        MenuAddLine ();
      MenuAddTextItem ("Завершен по", 11, 16 );
    }
    MenuAddLine ();
    if (ModeStopOrPauseReason == OPERATOR ) MenuAddTextItem ("решен. оператора", 16, 16);
    else if ( ModeStopOrPauseReason == OVER ) MenuAddTextItem ("готовности", 10, 16 );
    else if ( ModeStopOrPauseReason == OVERHEATING ) MenuAddTextItem ("перегреву модуля", 15, 16);
    else if ( ModeStopOrPauseReason == FAULT ) MenuAddTextItem ("аварии", 6, 16 );
  }
  MenuAddLine ();
  MenuAddTextItem ( "1. Зар 10-час I", 15, 16 );
  LoadConstIChargeMenuStage1 ();
  MenuAddLine ();
  MenuAddTextItem ( "2. Зар 20-час I", 15, 16 );
  LoadConstIChargeMenuStage2 ();
  if ( tmpSubsystemState == 0 ) {
    MenuAddLine ();
    MenuAddTextItem ( "3. Управление", 13, 16 );
  }
  else {
    MenuAddLine ();
    MenuAddTextItem ("3. Статистика", 13, 16 );
    LoadConstIChargeMenuStatistics ();
    MenuAddLine ();
    MenuAddTextItem ("4. Управление", 13, 16 );
  }
  LoadMenuCommands ();
  MenuAddLine ();
  MenuAddTextItem (chMenuEnd, 16, 16);
}

extern int32_t ConstUChargeChargeVoltage;
extern int32_t ConstUChargeMaxCurrent;
extern int32_t ConstUChargeMinCurrent;

void LoadConstUChargeMenuStage1Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Uзар=", 5, 5 ); 
  MenuAddFixedPointNumberEditorItem ( &ConstUChargeChargeVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[BATTERY_VOLTAGE], 4, 2, 100 ); MenuAddTextItem ( chVolts, 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмакс=", 6, 6 ); MenuAddFixedPointNumberEditorItem ( &ConstUChargeMaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( " А", 2, 5 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмин=", 5, 5 ); MenuAddFixedPointNumberEditorItem ( &ConstUChargeMinCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( " А", 2, 6 );
}

void LoadConstUChargeMenuStage1Executing ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Uзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &ConstUChargeChargeVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[BATTERY_VOLTAGE], 4, 2, 100 );
  // MenuAddTextItem ( chVolts, 2, 3 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &ConstUChargeMaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[CHARGE_CURRENT], 4, 2, 100 );
  // MenuAddTextItem ( " А", 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмин=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &ConstUChargeMinCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( "<", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[CHARGE_CURRENT], 4, 2, 100 );
  // MenuAddTextItem ( " А", 2, 2 );
}

void LoadConstUChargeMenuStage1Statistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Iзар=", 5, 5 );  // Выводим сообщение "Uз=24,63...26,77 В"
  MenuAddFixedPointNumberPlayerItem ( &Stage1Imax, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Imin, 4, 2, 100 );
  MenuAddTextItem ( chAmpers, 2, 2 );
}

void LoadConstUChargeMenuStatistics ( void ) {
  MenuAddLine ();   // Выводим сообщение "tэтапа=24,63...26,77 В"
  MenuAddTextItem ( chStageTime, 7, 7 ); MenuAddTimePlayerItem ( &Stage1Time );
  MenuAddLine ();   // Выводим сообщение "Qэтапа=532,64 Ач"
  MenuAddTextItem ( chStageVolume, 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadConstUChargeMenuStage1Over ( void ) {
   MenuAddLine ();
   MenuAddTextItem ( "Uзар=", 5, 5 );
   MenuAddFixedPointNumberPlayerItem ( &ConstUChargeChargeVoltage, 4, 2, 100 ); // Выводим сообщение "Iзар1=24,64 А" без возможности редактирования значения
   MenuAddTextItem ( chVolts, 2, 6 );
   MenuAddLine ();
   MenuAddTextItem ( "Iмакс=", 6, 6 );
   MenuAddFixedPointNumberPlayerItem ( &ConstUChargeMaxCurrent, 4, 2, 100 ); // Выводим сообщение "Umax1=24,64 В" без возможности редактирования значения
   MenuAddTextItem ( chAmpers, 2, 2 );
   MenuAddLine ();
   MenuAddTextItem ( "Iмин=", 5, 5 );
   MenuAddFixedPointNumberPlayerItem ( &ConstUChargeMinCurrent, 4, 2, 100 ); // Выводим сообщение "Umax1=24,64 В" без возможности редактирования значения
   MenuAddTextItem ( chAmpers, 2, 6 );
}

void LoadConstUChargeMenuStage1 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadConstUChargeMenuStage1Tune ();
    return;
   }
   else if ( tmpChargeControllerStage == MENU_MODE_WORKS ) {
     if ( Stage2Reached ) { // Если достигнут этап 2, то...
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16); // ...то этап 1 завершен,
       LoadConstUChargeMenuStage1Over ();         // Выводим настройки режима без возможности редактирования
       LoadConstUChargeMenuStage1Statistics ();   // Выводим статистику блока
     }
     else if ( Stage1Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadConstUChargeMenuStage1Executing ();
     LoadConstUChargeMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadConstUChargeMenuStage1Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_PAUSED ) {
      if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadConstUChargeMenuStage1Over ();
     LoadConstUChargeMenuStage1Statistics ();
    }
      else if ( Stage1Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап приостановл", 16, 16);
       LoadConstUChargeMenuStage1Tune ();
     LoadConstUChargeMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadConstUChargeMenuStage1Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_STOPPED ) {
      if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadConstUChargeMenuStage1Over ();
     LoadConstUChargeMenuStage1Statistics ();
    }
      else if ( Stage1Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadConstUChargeMenuStage1Over ();
     LoadConstUChargeMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadConstUChargeMenuStage1Over ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
}

void LoadConstUChargeMenu ( void ) {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadDifficultModesMenu;
  MenuIndex = CONST_U_CHARGE_MENU;
  MenuAddLine ();
  MenuAddTextItem ("**Заряд пост. U*", 16, 16);
  tmpSubsystemState = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
  if ( tmpSubsystemState == 0 ) {
    MenuAddLine ();
    MenuAddTextItem ( "Настройка режима", 16, 16 );
  }
  else if ( tmpSubsystemState == MENU_MODE_WORKS ) {
      // Одна из ступеней режима заряда постоянным током
      if ( Stage3Reached ) { // Если достигнута последняя ступень, то выводим соответствующее сообщение
      MenuAddLine ();
      MenuAddTextItem ("Идёт завершение", 15, 16 );
      MenuAddLine ();
      MenuAddTextItem ("заряда…", 7, 16 );
    }
    else {
      MenuAddLine ();
        MenuAddTextItem ( "Идёт заряд…", 11, 11 );
    }
  }
  else {
    if ( tmpSubsystemState == MENU_MODE_PAUSED ) {
      MenuAddLine ();
      MenuAddTextItem ("Приостановлен по", 16, 16 );
    }
    else if ( tmpSubsystemState == MENU_MODE_STOPPED ) {
        MenuAddLine ();
      MenuAddTextItem ("Завершен по", 11, 16 );
    }
    MenuAddLine ();
    if (ModeStopOrPauseReason == OPERATOR ) MenuAddTextItem ("решен. оператора", 16, 16);
    else if ( ModeStopOrPauseReason == OVER ) MenuAddTextItem ("готовности", 10, 16 );
    else if ( ModeStopOrPauseReason == OVERHEATING ) MenuAddTextItem ("перегреву модуля", 16, 16);
    else if ( ModeStopOrPauseReason == FAULT ) MenuAddTextItem ("аварии", 6, 16 );
  }
  MenuAddLine ();
  MenuAddTextItem ( "1. Заряд пост U", 15, 16 );
  LoadConstUChargeMenuStage1 ();
  if ( tmpSubsystemState == 0 ) {
    MenuAddLine ();
    MenuAddTextItem ( "2. Управление", 13, 16 );
  }
  else {
    MenuAddLine ();
    MenuAddTextItem ("2. Статистика", 13, 16 );
    LoadConstUChargeMenuStatistics ();
    MenuAddLine ();
    MenuAddTextItem ("3. Управление", 13, 16 );
  }
  LoadMenuCommands ();
  MenuAddLine ();
  MenuAddTextItem (chMenuEnd, 16, 16);
}

extern int32_t FastCombineChargeStage1MaxVoltage;
extern int32_t FastCombineChargeStage1MaxCurrent;
extern int32_t FastCombineChargeStage1MinCurrent;


void LoadFastCombineChargeMenuStage1Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Uзар=", 5, 5 ); 
  MenuAddFixedPointNumberEditorItem ( &FastCombineChargeStage1MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[BATTERY_VOLTAGE], 4, 2, 100 ); MenuAddTextItem ( chVolts, 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмакс=", 6, 6 ); MenuAddFixedPointNumberEditorItem ( &FastCombineChargeStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( " А", 2, 6 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмин=", 5, 5 ); MenuAddFixedPointNumberEditorItem ( &FastCombineChargeStage1MinCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( " А", 2, 6 );
}

void LoadFastCombineChargeMenuStage1Executing ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Uзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &FastCombineChargeStage1MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[BATTERY_VOLTAGE], 4, 2, 100 );
  MenuAddTextItem ( chVolts, 2, 3 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &FastCombineChargeStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[CHARGE_CURRENT], 4, 2, 100 );
  MenuAddTextItem ( "А", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмин=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &FastCombineChargeStage1MinCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( "<", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[CHARGE_CURRENT], 4, 2, 100 );
  MenuAddTextItem ( "А", 1, 1 );
}

void LoadFastCombineChargeMenuStage1Statistics ( void ) {
    MenuAddLine ();
    MenuAddTextItem ( "Iзар=", 5, 5 );  // Выводим сообщение "Uз=24,63...26,77 В"
    MenuAddFixedPointNumberPlayerItem ( &Stage1Imax, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Imin, 4, 2, 100 );
    MenuAddTextItem ( chAmpers, 2, 2 );
}

void LoadFastCombineChargeMenuStage1Over ( void ) {
   MenuAddLine ();
   MenuAddTextItem ( "Uзар=", 5, 5 );
   MenuAddFixedPointNumberPlayerItem ( &FastCombineChargeStage1MaxVoltage, 4, 2, 100 ); // Выводим сообщение "Iзар1=24,64 А" без возможности редактирования значения
   MenuAddTextItem ( chVolts, 2, 6 );
   MenuAddLine ();
   MenuAddTextItem ( "Iмакс=", 6, 6 );
   MenuAddFixedPointNumberPlayerItem ( &FastCombineChargeStage1MaxCurrent, 4, 2, 100 ); // Выводим сообщение "Umax1=24,64 В" без возможности редактирования значения
   MenuAddTextItem ( chAmpers, 2, 2 );
   MenuAddLine ();
   MenuAddTextItem ( "Iмин=", 5, 5 );
   MenuAddFixedPointNumberPlayerItem ( &FastCombineChargeStage1MinCurrent, 4, 2, 100 ); // Выводим сообщение "Umax1=24,64 В" без возможности редактирования значения
   MenuAddTextItem ( chAmpers, 2, 6 );
}

void LoadFastCombineChargeMenuStage1 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadFastCombineChargeMenuStage1Tune ();
    return;
   }
   else if ( tmpChargeControllerStage == MENU_MODE_WORKS ) {
      if ( Stage2Reached ) { // Если достигнут этап 2, то...
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16); // ...то этап 1 завершен,
     LoadFastCombineChargeMenuStage1Over ();         // Выводим настройки режима без возможности редактирования
     LoadFastCombineChargeMenuStage1Statistics ();   // Выводим статистику блока
    }
      else if ( Stage1Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadFastCombineChargeMenuStage1Executing ();
     LoadFastCombineChargeMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadFastCombineChargeMenuStage1Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_PAUSED ) {
      if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadFastCombineChargeMenuStage1Over ();
     LoadFastCombineChargeMenuStage1Statistics ();
    }
      else if ( Stage1Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап приостановл", 16, 16);
       LoadFastCombineChargeMenuStage1Tune ();
     LoadFastCombineChargeMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadFastCombineChargeMenuStage1Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_STOPPED ) {
      if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadFastCombineChargeMenuStage1Over ();
     LoadFastCombineChargeMenuStage1Statistics ();
    }
      else if ( Stage1Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadFastCombineChargeMenuStage1Over ();
     LoadFastCombineChargeMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadFastCombineChargeMenuStage1Over ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
}

extern int32_t FastCombineChargeStage2MaxCurrent;
extern int32_t FastCombineChargeStage2MaxVoltage;

void LoadFastCombineChargeMenuStage2Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &FastCombineChargeStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 7 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &FastCombineChargeStage2MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
}

void LoadFastCombineChargeMenuStage2Executing ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 ); // Выводим сообщение "Iз=00,00<__,__ А
  MenuAddFixedPointNumberEditorItem ( &FastCombineChargeStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 );  MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 2, 100 ); MenuAddTextItem ( "А", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberEditorItem ( &FastCombineChargeStage2MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 ); MenuAddTextItem ( "В", 1, 1 );
}

void LoadFastCombineChargeMenuStage2Statistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Uзар=", 5, 5 );  // Выводим сообщение "Uз=24,63...26,77 В"
  MenuAddFixedPointNumberPlayerItem ( &Stage2Umin, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage2Umax, 4, 2, 100 );
  MenuAddTextItem ( chVolts, 2, 3 );
  MenuAddLine ();   // Выводим сообщение "tэтапа=24,63...26,77 В"
  MenuAddTextItem ( chStageTime, 7, 7 ); MenuAddTimePlayerItem ( &Stage2Time );
  MenuAddLine ();   // Выводим сообщение "Qэтапа=532,64 Ач"
  MenuAddTextItem ( chStageVolume, 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &Stage2Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadFastCombineChargeMenuStage2Over ( void ) {
   MenuAddLine ();
   MenuAddTextItem ( "Iзар=", 5, 5 );
   MenuAddFixedPointNumberPlayerItem ( &FastCombineChargeStage2MaxCurrent, 4, 2, 100 ); // Выводим сообщение "Iзар1=24,64 А" без возможности редактирования значения
   MenuAddTextItem ( chAmpers, 2, 7 );
   MenuAddLine ();
   MenuAddTextItem ( "Uмакс=", 6, 6 );
   MenuAddFixedPointNumberPlayerItem ( &FastCombineChargeStage2MaxVoltage, 4, 2, 100 ); // Выводим сообщение "Umax1=24,64 В" без возможности редактирования значения
   MenuAddTextItem ( chVolts, 2, 7 );
}

void LoadFastCombineChargeMenuStage2 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadFastCombineChargeMenuStage2Tune ();
    return;
   }
   else if ( tmpChargeControllerStage == MENU_MODE_WORKS ) {
     if ( Stage3Reached ) { // Если достигнут этап 2, то...
      MenuAddLine ();
      MenuAddTextItem ("этап завершен", 13, 16); // ...то этап 1 завершен,
      LoadFastCombineChargeMenuStage2Over ();         // Выводим настройки режима без возможности редактирования
      LoadFastCombineChargeMenuStage2Statistics ();   // Выводим статистику блока
    }
      else if ( Stage2Reached ) {
        MenuAddLine ();
        MenuAddTextItem ("этап выполняется", 16, 16);
        LoadFastCombineChargeMenuStage2Executing ();
     LoadFastCombineChargeMenuStage2Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadFastCombineChargeMenuStage2Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_PAUSED ) {
      if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadFastCombineChargeMenuStage2Over ();
     LoadFastCombineChargeMenuStage2Statistics ();
    }
      else if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап приостановл", 16, 16);
       LoadFastCombineChargeMenuStage2Tune ();
     LoadFastCombineChargeMenuStage2Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadFastCombineChargeMenuStage2Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_STOPPED ) {
      if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadFastCombineChargeMenuStage2Over ();
     LoadFastCombineChargeMenuStage2Statistics ();
    }
      else if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadFastCombineChargeMenuStage2Over ();
     LoadFastCombineChargeMenuStage2Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadFastCombineChargeMenuStage2Over ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
}

extern int32_t FastCombineChargeStage3MaxCurrent;
extern int32_t FastCombineChargeStage3MaxVoltage;
extern int32_t FastCombineChargeStage3MinVoltageIntervalPerHour;

void LoadFastCombineChargeMenuStage3Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &FastCombineChargeStage3MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 7 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &FastCombineChargeStage3MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
}

void LoadFastCombineChargeMenuStage3Executing ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 ); // Выводим сообщение "Iз=00,00<__,__ А
  MenuAddFixedPointNumberEditorItem ( &FastCombineChargeStage3MaxCurrent, 4, 2, 100, 0, 100, 9000 );  MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 2, 100 ); MenuAddTextItem ( "А", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberEditorItem ( &FastCombineChargeStage3MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 ); MenuAddTextItem ( "В", 1, 1 );
}

void LoadFastCombineChargeMenuStage3Statistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Uзар=", 5, 5 );  // Выводим сообщение "Uз=24,63...26,77 В"
  MenuAddFixedPointNumberPlayerItem ( &Stage3Umin, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage3Umax, 4, 2, 100 );
  MenuAddTextItem ( chVolts, 2, 3 );
  MenuAddLine ();   // Выводим сообщение "tэтапа=24,63...26,77 В"
  MenuAddTextItem ( chStageTime, 7, 7 ); MenuAddTimePlayerItem ( &Stage3Time );
  MenuAddLine ();   // Выводим сообщение "Qэтапа=532,64 Ач"
  MenuAddTextItem ( chStageVolume, 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &Stage3Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadFastCombineChargeMenuStage3Over ( void ) {
   MenuAddLine ();
   MenuAddTextItem ( "Iзар=", 5, 5 );
   MenuAddFixedPointNumberPlayerItem ( &FastCombineChargeStage3MaxCurrent, 4, 2, 100 ); // Выводим сообщение "Iзар1=24,64 А" без возможности редактирования значения
   MenuAddTextItem ( chAmpers, 2, 7 );
   MenuAddLine ();
   MenuAddTextItem ( "Uмакс=", 6, 6 );
   MenuAddFixedPointNumberPlayerItem ( &FastCombineChargeStage3MaxVoltage, 4, 2, 100 ); // Выводим сообщение "Umax1=24,64 В" без возможности редактирования значения
   MenuAddTextItem ( chVolts, 2, 7 );
}

void LoadFastCombineChargeMenuStage3 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadFastCombineChargeMenuStage3Tune ();
    return;
   }
   else if ( tmpChargeControllerStage == MENU_MODE_WORKS ) {
      if ( Stage4Reached ) { // Если достигнут этап 2, то...
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16); // ...то этап 1 завершен,
     LoadFastCombineChargeMenuStage3Over ();         // Выводим настройки режима без возможности редактирования
     LoadFastCombineChargeMenuStage3Statistics ();   // Выводим статистику блока
    }
      else if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadFastCombineChargeMenuStage3Executing ();
     LoadFastCombineChargeMenuStage3Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadFastCombineChargeMenuStage3Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_PAUSED ) {
      if ( Stage4Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadFastCombineChargeMenuStage3Over ();
     LoadFastCombineChargeMenuStage3Statistics ();
    }
      else if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап приостановл", 16, 16);
       LoadFastCombineChargeMenuStage3Tune ();
     LoadFastCombineChargeMenuStage3Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadFastCombineChargeMenuStage3Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_STOPPED ) {
      if ( Stage4Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadFastCombineChargeMenuStage3Over ();
     LoadFastCombineChargeMenuStage3Statistics ();
    }
      else if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadFastCombineChargeMenuStage3Over ();
     LoadFastCombineChargeMenuStage3Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadFastCombineChargeMenuStage3Over ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
}

void LoadFastCombineChargeMenuStatistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "tобщ = ", 7, 7 ); MenuAddTimePlayerItem ( &TotalTime );
  MenuAddLine ();
  MenuAddTextItem ( "Qобщ = ", 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &TotalVolume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadFastCombineChargeMenu ( void ) {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadDifficultModesMenu;
  MenuIndex = FAST_COMBINE_CHARGE_MENU;
  MenuAddLine ();
  MenuAddTextItem ("*Ускор комб зар*", 16, 16);
  tmpSubsystemState = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
  if ( tmpSubsystemState == 0 ) {
    MenuAddLine ();
    MenuAddTextItem ( "Настройка режима", 16, 16 );
  }
  else if ( tmpSubsystemState == MENU_MODE_WORKS ) {
      // Одна из ступеней режима заряда постоянным током
      if ( Stage4Reached ) { // Если достигнута последняя ступень, то выводим соответствующее сообщение
      MenuAddLine ();
      MenuAddTextItem ("Идёт завершение", 15, 16 );
      MenuAddLine ();
      MenuAddTextItem ("заряда…", 7, 16 );
    }
    else if ( Stage2Reached ) {
      MenuAddLine ();
        MenuAddTextItem ( "Идёт заряд током", 16, 16 );
      if (Stage3Reached ) { // Если последняя ступень не достигнута, но достигнута вторая ступень, то выводим соответствующее сообщение
        MenuAddLine ();
        MenuAddTextItem ("второй ступени…", 15, 16 );
      }
      else { // Если вторая ступень не достигнута, но достигнута первая ступень, то выводим соответствующее сообщение
        MenuAddLine ();
        MenuAddTextItem ("первой ступени…", 15, 16 );
      }
    }
    else {
      MenuAddLine ();
      MenuAddTextItem ("Идёт заряд", 10, 16 );
      MenuAddLine ();
      MenuAddTextItem ("напряжением…", 12, 16 );    
    }
  }
  else {
    if ( tmpSubsystemState == MENU_MODE_PAUSED ) {
      MenuAddLine ();
      MenuAddTextItem ("Приостановлен по", 16, 16 );
    }
    else if ( tmpSubsystemState == MENU_MODE_STOPPED ) {
        MenuAddLine ();
      MenuAddTextItem ("Завершен по", 11, 16 );
    }
    MenuAddLine ();
    if (ModeStopOrPauseReason == OPERATOR ) MenuAddTextItem ("решен. оператора", 16, 16);
    else if ( ModeStopOrPauseReason == OVER ) MenuAddTextItem ("готовности", 10, 16 );
    else if ( ModeStopOrPauseReason == OVERHEATING ) MenuAddTextItem ("перегреву модуля", 16, 16);
    else if ( ModeStopOrPauseReason == FAULT ) MenuAddTextItem ("аварии", 6, 16 );
  }
  MenuAddLine ();
  MenuAddTextItem ( "1. Заряд пост U", 15, 16 );
  LoadFastCombineChargeMenuStage1 ();
  MenuAddLine ();
  MenuAddTextItem ( "2. Зар 10-час I", 15, 16 );
  LoadFastCombineChargeMenuStage2 ();
  MenuAddLine ();
  MenuAddTextItem ( "3. Зар 20-час I", 15, 16 );
  LoadFastCombineChargeMenuStage3 ();
  if ( tmpSubsystemState == 0 ) {
    MenuAddLine ();
    MenuAddTextItem ( "4. Управление", 13, 16 );
  }
  else {
    MenuAddLine ();
    MenuAddTextItem ("4. Статистика", 13, 16 );
    LoadFastCombineChargeMenuStatistics ();
    MenuAddLine ();
    MenuAddTextItem ("5. Управление", 13, 16 );
  }
  LoadMenuCommands ();
  MenuAddLine ();
  MenuAddTextItem (chMenuEnd, 16, 16);
}

int32_t SimpleConstIChargeBatteryVoltageIndex = 0;
int32_t SimpleConstIChargeBatteryVoltageComboBox;

extern int32_t* pMaxFaultBatteryVoltage;
extern int32_t* pMinFaultBatteryVoltage;

void SimpleConstIChargeMenuOnBatteryVoltageSelect ( int32_t BatteryVoltageIndex ) {
  if ( BatteryVoltageIndex == 0 ) {
    SimpleConstIChargeBatteryVoltageIndex = 0;
    SimpleConstIChargeStage1MaxVoltage = References[SIMPLE_CONST_I_CHARGE_24V_1ST_STAGE_MAX_VOLTAGE];
    SimpleConstIChargeStage2MaxVoltage = References[SIMPLE_CONST_I_CHARGE_24V_2ND_STAGE_MAX_VOLTAGE];
    SimpleConstIChargeStage2MaxVoltageIntervalPerHour = References[SIMPLE_CONST_I_CHARGE_24V_2ND_STAGE_MAX_DELTA_VOLTAGE];
    pMaxFaultBatteryVoltage = &(References[MAX_24V_BATTERY_VOLTAGE]);
    pMinFaultBatteryVoltage = &(References[MIN_24V_BATTERY_VOLTAGE]);
  }
  else if ( BatteryVoltageIndex == 1 ) {
    SimpleConstIChargeBatteryVoltageIndex = 1;
    SimpleConstIChargeStage1MaxVoltage = References[SIMPLE_CONST_I_CHARGE_12V_1ST_STAGE_MAX_VOLTAGE];
    SimpleConstIChargeStage2MaxVoltage = References[SIMPLE_CONST_I_CHARGE_12V_2ND_STAGE_MAX_VOLTAGE];
    SimpleConstIChargeStage2MaxVoltageIntervalPerHour = References[SIMPLE_CONST_I_CHARGE_12V_2ND_STAGE_MAX_DELTA_VOLTAGE];
    pMaxFaultBatteryVoltage = &(References[MAX_12V_BATTERY_VOLTAGE]);
    pMinFaultBatteryVoltage = &(References[MIN_12V_BATTERY_VOLTAGE]);
  }
}

void LoadSimpleConstIChargeMenuStage1Tune ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Настройка режима", 16, 16 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uакб=", 5, 5 );
  SimpleConstIChargeBatteryVoltageComboBox = MenuAddComboBoxItem ( 2, SimpleConstIChargeMenuOnBatteryVoltageSelect );
  MenuAddComboBoxItemElement ( SimpleConstIChargeBatteryVoltageComboBox, "24 В", 4 );
  MenuAddComboBoxItemElement ( SimpleConstIChargeBatteryVoltageComboBox, "12 В", 4 );
  ComboBoxSetElementIndex ( SimpleConstIChargeBatteryVoltageComboBox, SimpleConstIChargeBatteryVoltageIndex );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар1=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &SimpleConstIChargeStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар2=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &SimpleConstIChargeStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 2 );
}

void LoadSimpleConstIChargeMenuStage1Executing ( void ) {
  MenuAddLine ();
  MenuAddTextItem ("Идёт заряд током", 16, 16);
  MenuAddLine ();
  MenuAddTextItem ("первой ступени…", 15, 16);
  MenuAddLine ();
  MenuAddTextItem ( "Uмакс=", 6, 6 ); // Выводим сообщение "Iз=00,00<__,__ А
  MenuAddFixedPointNumberPlayerItem ( &SimpleConstIChargeStage1MaxVoltage, 4, 2, 100 ); MenuAddTextItem ( ">", 1, 1 );  MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 ); MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар1=", 6, 6 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberEditorItem ( &SimpleConstIChargeStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( "~", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 2, 100 ); MenuAddTextItem ( " А", 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар2=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &SimpleConstIChargeStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();
  MenuAddTextItem ("tзар=", 5, 5); MenuAddTimePlayerItem ( &TotalTime );
  MenuAddLine ();
  MenuAddTextItem ("Qзар=", 5, 5); MenuAddFixedPointNumberPlayerItem ( &TotalVolume, 6, 100, 5 );  MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleConstIChargeMenuStage2Executing ( void ) {
  MenuAddLine ();
  MenuAddTextItem ("Идёт заряд током", 16, 16);
  MenuAddLine ();
  MenuAddTextItem ("второй ступени…", 15, 16);
  MenuAddLine ();
  MenuAddTextItem ( "Uмакс=", 6, 6 ); // Выводим сообщение "Iз=00,00<__,__ А
  MenuAddFixedPointNumberPlayerItem ( &SimpleConstIChargeStage2MaxVoltage, 4, 2, 100 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 ); MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар2=", 6, 6 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberEditorItem ( &SimpleConstIChargeStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( "~", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 2, 100 ); MenuAddTextItem ( " А", 2, 2 );
  MenuAddLine ();
  MenuAddTextItem ("tзар=", 5, 5); MenuAddTimePlayerItem ( &TotalTime );
  MenuAddLine ();
  MenuAddTextItem ("Qзар=", 5, 5); MenuAddFixedPointNumberPlayerItem ( &TotalVolume, 6, 100, 5 );  MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleConstIChargeMenuStage1Paused ( void ) {
  MenuAddLine ();
  MenuAddTextItem ("Заряд приост по", 15, 16);
  MenuAddLine ();
  if ( ModeStopOrPauseReason == OVER ) { MenuAddTextItem ("готовности", 10, 16); }
  else if ( ModeStopOrPauseReason == FAULT ) { MenuAddTextItem ("аварии", 6, 16); }
  else if ( ModeStopOrPauseReason == OPERATOR ) { MenuAddTextItem ("решен оператора", 15, 16); }
  else if ( ModeStopOrPauseReason == OVERHEATING ) { MenuAddTextItem ("перегреву модуля", 16, 16); }
  MenuAddLine ();
  MenuAddTextItem ( "Uмакс=", 6, 6 ); // Выводим сообщение "Iз=00,00<__,__ А
  MenuAddFixedPointNumberPlayerItem ( &SimpleConstIChargeStage1MaxVoltage, 4, 2, 100 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 ); MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар1=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &SimpleConstIChargeStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар2=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &SimpleConstIChargeStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();
  MenuAddTextItem ("tзар=", 5, 5); MenuAddTimePlayerItem ( &TotalTime );
  MenuAddLine ();
  MenuAddTextItem ("Qзар=", 5, 5); MenuAddFixedPointNumberPlayerItem ( &TotalVolume, 6, 100, 5 );  MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleConstIChargeMenuStage2Paused ( void ) {
  MenuAddLine ();
  MenuAddTextItem ("Заряд приост по", 15, 16);
  MenuAddLine ();
  if ( ModeStopOrPauseReason == OVER ) { MenuAddTextItem ("готовности", 10, 16); }
  else if ( ModeStopOrPauseReason == FAULT ) { MenuAddTextItem ("аварии", 6, 16); }
  else if ( ModeStopOrPauseReason == OPERATOR ) { MenuAddTextItem ("решен оператора", 15, 16); }
  else if ( ModeStopOrPauseReason == OVERHEATING ) { MenuAddTextItem ("перегреву модуля", 16, 16); }
  MenuAddLine ();
  MenuAddTextItem ( "Uмакс=", 6, 6 ); // Выводим сообщение "Iз=00,00<__,__ А
  MenuAddFixedPointNumberPlayerItem ( &SimpleConstIChargeStage2MaxVoltage, 4, 2, 100 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 ); MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар2=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &SimpleConstIChargeStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();
  MenuAddTextItem ("tзар=", 5, 5); MenuAddTimePlayerItem ( &TotalTime );
  MenuAddLine ();
  MenuAddTextItem ("Qзар=", 5, 5); MenuAddFixedPointNumberPlayerItem ( &TotalVolume, 6, 100, 5 );  MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleConstIChargeMenuStopped ( void ) {
  MenuAddLine ();
  MenuAddTextItem ("Заряд заверш. по", 16, 16);
  MenuAddLine ();
  if ( ModeStopOrPauseReason == OVER ) { MenuAddTextItem ("готовности", 10, 16); }
  else if ( ModeStopOrPauseReason == FAULT ) { MenuAddTextItem ("аварии", 6, 16); }
  else if ( ModeStopOrPauseReason == OPERATOR ) { MenuAddTextItem ("решен оператора", 15, 16); }
  else if ( ModeStopOrPauseReason == OVERHEATING ) { MenuAddTextItem ("перегреву модуля", 16, 16); }
  MenuAddLine ();
  MenuAddTextItem ( "Uакб=", 5, 5 ); // Выводим сообщение "Iз=00,00<__,__ А
  if ( SimpleConstIChargeBatteryVoltageIndex ) { MenuAddTextItem ( "12 В", 4, 4 ); } else { MenuAddTextItem ( "24 В", 4, 4 ); }
  MenuAddLine ();
  MenuAddTextItem ( "Iзар1=", 6, 6 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberPlayerItem ( &SimpleConstIChargeStage1MaxCurrent, 4, 2, 100 ); MenuAddTextItem ( " А", 2, 2 );
  MenuAddLine ();
  MenuAddTextItem ( "Iзар2=", 6, 6 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberPlayerItem ( &SimpleConstIChargeStage2MaxCurrent, 4, 2, 100 ); MenuAddTextItem ( " А", 2, 2 );
  MenuAddLine ();
  MenuAddTextItem ("tзар=", 5, 5); MenuAddTimePlayerItem ( &TotalTime );
  MenuAddLine ();
  MenuAddTextItem ("Qзар=", 5, 5); MenuAddFixedPointNumberPlayerItem ( &TotalVolume, 6, 100, 5 );  MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleConstIChargeMenu ( void ) {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadMainMenu;
  MenuIndex = SIMPLE_CONST_I_CHARGE_MENU;
  MenuAddLine ();
  MenuAddTextItem ("**Заряд пост. I*", 16, 16);
  tmpSubsystemState = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
  if ( tmpSubsystemState == 0 ) {
    LoadSimpleConstIChargeMenuStage1Tune ();
  }
  else if ( tmpSubsystemState == MENU_MODE_WORKS ) {
      // Одна из ступеней режима заряда постоянным током
    if ( Stage2Reached ) { // Если достигнута вторая ступень, то выводим соответствующее сообщение
      LoadSimpleConstIChargeMenuStage2Executing ();
    }
    else {
      LoadSimpleConstIChargeMenuStage1Executing ();
    }
  }
  else if ( tmpSubsystemState == MENU_MODE_PAUSED ) {
    if ( Stage2Reached ) { // Если достигнута вторая ступень, то выводим соответствующее сообщение
      LoadSimpleConstIChargeMenuStage2Paused ();
    }
    else {
      LoadSimpleConstIChargeMenuStage1Paused ();
    }
  }
  else if ( tmpSubsystemState == MENU_MODE_STOPPED ) {
    LoadSimpleConstIChargeMenuStopped ();
  }
  LoadMenuCommands ();
  MenuAddLine ();
  MenuAddTextItem (chMenuEnd, 16, 16);
}

extern int32_t SimpleConstUChargeChargeVoltage;
extern int32_t SimpleConstUChargeMaxCurrent;
extern int32_t SimpleConstUChargeMinCurrent;

int32_t SimpleConstUChargeBatteryVoltageComboBox;
int32_t SimpleConstUChargeBatteryVoltageIndex = 0;

void SimpleConstUChargeMenuOnBatteryVoltageSelect ( int32_t BatteryVoltageIndex ) {
  if ( BatteryVoltageIndex == 0 ) {
    SimpleConstUChargeBatteryVoltageIndex = 0;
    SimpleConstUChargeChargeVoltage = References[SIMPLE_CONST_U_CHARGE_24V_CHARGE_VOLTAGE];
    pMaxFaultBatteryVoltage = &(References[MAX_24V_BATTERY_VOLTAGE]);
    pMinFaultBatteryVoltage = &(References[MIN_24V_BATTERY_VOLTAGE]);
  }
  else if ( BatteryVoltageIndex == 1 ) {
    SimpleConstUChargeBatteryVoltageIndex = 1;
    SimpleConstUChargeChargeVoltage = References[SIMPLE_CONST_U_CHARGE_12V_CHARGE_VOLTAGE];
    pMaxFaultBatteryVoltage = &(References[MAX_12V_BATTERY_VOLTAGE]);
    pMinFaultBatteryVoltage = &(References[MIN_12V_BATTERY_VOLTAGE]);
  }
}

void LoadSimpleConstUChargeMenuStage1Tune ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Настройка режима", 16, 16 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uакб=", 5, 5 );
  SimpleConstUChargeBatteryVoltageComboBox = MenuAddComboBoxItem ( 2, SimpleConstUChargeMenuOnBatteryVoltageSelect );
  MenuAddComboBoxItemElement ( SimpleConstUChargeBatteryVoltageComboBox, "24 В", 4 );
  MenuAddComboBoxItemElement ( SimpleConstUChargeBatteryVoltageComboBox, "12 В", 4 );
  ComboBoxSetElementIndex ( SimpleConstUChargeBatteryVoltageComboBox, SimpleConstUChargeBatteryVoltageIndex );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &SimpleConstUChargeMaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмин=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &SimpleConstUChargeMinCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 2 );
}

void LoadSimpleConstUChargeMenuStage1Executing ( void ) {
  MenuAddLine ();
  MenuAddTextItem ("Идёт заряд...", 13, 16);
  MenuAddLine ();
  MenuAddTextItem ( "Uзар=", 5, 5 ); // Выводим сообщение "Iз=00,00<__,__ А
  MenuAddFixedPointNumberPlayerItem ( &SimpleConstUChargeChargeVoltage, 4, 2, 100 ); MenuAddTextItem ( "~", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 ); MenuAddTextItem ( " В", 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмакс=", 6, 6 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberEditorItem ( &SimpleConstUChargeMaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 2, 100 ); MenuAddTextItem ( "А", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмин=", 5, 5 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberEditorItem ( &SimpleConstUChargeMinCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( "<", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 2, 100 ); MenuAddTextItem ( " А", 2, 2 );
  MenuAddLine ();
  MenuAddTextItem ("tзар=", 5, 5); MenuAddTimePlayerItem ( &TotalTime );
  MenuAddLine ();
  MenuAddTextItem ("Qзар=", 5, 5); MenuAddFixedPointNumberPlayerItem ( &TotalVolume, 6, 100, 5 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleConstUChargeMenuStage1Paused ( void ) {
  MenuAddLine ();
  MenuAddTextItem ("Заряд приост по", 15, 16);
  MenuAddLine ();
  if ( ModeStopOrPauseReason == OVER ) { MenuAddTextItem ("готовности", 10, 16); }
  else if ( ModeStopOrPauseReason == FAULT ) { MenuAddTextItem ("аварии", 6, 16); }
  else if ( ModeStopOrPauseReason == OPERATOR ) { MenuAddTextItem ("решен оператора", 15, 16); }
  else if ( ModeStopOrPauseReason == OVERHEATING ) { MenuAddTextItem ("перегреву модуля", 16, 16); }
  MenuAddLine ();
  MenuAddTextItem ( "Uакб=", 5, 5 ); // Выводим сообщение "Iз=00,00<__,__ А
  if ( SimpleConstUChargeBatteryVoltageIndex == 0 ) { MenuAddTextItem ( "24 В", 4, 4); } else { MenuAddTextItem ( "12 В", 4, 4); }
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмакс=", 6, 6 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberEditorItem ( &SimpleConstUChargeMaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( " А", 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмин=", 5, 5 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberEditorItem ( &SimpleConstUChargeMinCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( " А", 2, 2 );
  MenuAddLine ();
  MenuAddTextItem ("tзар=", 5, 5); MenuAddTimePlayerItem ( &TotalTime );
  MenuAddLine ();
  MenuAddTextItem ("Qзар=", 5, 5); MenuAddFixedPointNumberPlayerItem ( &TotalVolume, 6, 100, 5 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleConstUChargeMenuStopped ( void ) {
  MenuAddLine ();
  MenuAddTextItem ("Заряд заверш. по", 16, 16);
  MenuAddLine ();
  if ( ModeStopOrPauseReason == OVER ) { MenuAddTextItem ("готовности", 10, 16); }
  else if ( ModeStopOrPauseReason == FAULT ) { MenuAddTextItem ("аварии", 6, 16); }
  else if ( ModeStopOrPauseReason == OPERATOR ) { MenuAddTextItem ("решен оператора", 15, 16); }
  else if ( ModeStopOrPauseReason == OVERHEATING ) { MenuAddTextItem ("перегреву модуля", 16, 16); }
  MenuAddLine ();
  MenuAddTextItem ( "Uакб=", 5, 5 ); // Выводим сообщение "Iз=00,00<__,__ А
  if ( SimpleConstUChargeBatteryVoltageIndex ) { MenuAddTextItem ( "12 В", 4, 4 ); } else { MenuAddTextItem ( "24 В", 4, 4 ); }
  MenuAddLine ();
  MenuAddTextItem ( "Iмакс=", 6, 6 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberPlayerItem ( &SimpleConstUChargeMaxCurrent, 4, 2, 100 ); MenuAddTextItem ( " А", 2, 2 );
  MenuAddLine ();
  MenuAddTextItem ( "Iмин=", 5, 5 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberPlayerItem ( &SimpleConstUChargeMinCurrent, 4, 2, 100 ); MenuAddTextItem ( " А", 2, 2 );
  MenuAddLine ();
  MenuAddTextItem ("tзар=", 5, 5); MenuAddTimePlayerItem ( &TotalTime );
  MenuAddLine ();
  MenuAddTextItem ("Qзар=", 5, 5); MenuAddFixedPointNumberPlayerItem ( &TotalVolume, 6, 100, 5 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleConstUChargeMenu ( void ) {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadMainMenu;
  MenuIndex = SIMPLE_CONST_U_CHARGE_MENU;
  MenuAddLine ();
  MenuAddTextItem ("**Заряд пост. U*", 16, 16);
  tmpSubsystemState = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
  if ( tmpSubsystemState == 0 ) {
    LoadSimpleConstUChargeMenuStage1Tune ();
  }
  else if ( tmpSubsystemState == MENU_MODE_WORKS ) {
      LoadSimpleConstUChargeMenuStage1Executing ();
  }
  else if ( tmpSubsystemState == MENU_MODE_PAUSED ) {
      LoadSimpleConstUChargeMenuStage1Paused ();
  }
  else if ( tmpSubsystemState == MENU_MODE_STOPPED ) {
    LoadSimpleConstUChargeMenuStopped ();
  }
  LoadMenuCommands ();
  MenuAddLine ();
  MenuAddTextItem (chMenuEnd, 16, 16);
}




extern int32_t SimpleFastCombineChargeStage1MaxVoltage;
extern int32_t SimpleFastCombineChargeStage1MaxCurrent;
extern int32_t SimpleFastCombineChargeStage1MinCurrent;
extern int32_t SimpleFastCombineChargeStage2MaxVoltage;
extern int32_t SimpleFastCombineChargeStage3MaxVoltage;
extern int32_t SimpleFastCombineChargeStage2MaxCurrent;
extern int32_t SimpleFastCombineChargeStage3MaxCurrent;
extern int32_t SimpleFastCombineChargeStage2MaxVoltageIntervalPerHour;

int32_t SimpleFastCombineChargeBatteryVoltageComboBox;
int32_t SimpleFastCombineChargeBatteryVoltageIndex = 0;

void SimpleFastCombineChargeMenuOnBatteryVoltageSelect ( int32_t BatteryVoltageIndex ) {
  if ( BatteryVoltageIndex == 0 ) {
    SimpleFastCombineChargeBatteryVoltageIndex = 0;
    SimpleFastCombineChargeStage1MaxVoltage = References[SIMPLE_CONST_U_CHARGE_24V_CHARGE_VOLTAGE];
    SimpleFastCombineChargeStage2MaxVoltage = References[SIMPLE_CONST_I_CHARGE_24V_2ND_STAGE_MAX_VOLTAGE];
    pMaxFaultBatteryVoltage = &(References[MAX_24V_BATTERY_VOLTAGE]);
    pMinFaultBatteryVoltage = &(References[MIN_24V_BATTERY_VOLTAGE]);
	SimpleFastCombineChargeStage2MaxVoltageIntervalPerHour = References[SIMPLE_CONST_I_CHARGE_24V_2ND_STAGE_MAX_DELTA_VOLTAGE];
  }
  else if ( BatteryVoltageIndex == 1 ) {
    SimpleFastCombineChargeBatteryVoltageIndex = 1;
    SimpleFastCombineChargeStage1MaxVoltage = References[SIMPLE_CONST_U_CHARGE_12V_CHARGE_VOLTAGE];
    SimpleFastCombineChargeStage2MaxVoltage = References[SIMPLE_CONST_I_CHARGE_12V_2ND_STAGE_MAX_VOLTAGE];
    pMaxFaultBatteryVoltage = &(References[MAX_12V_BATTERY_VOLTAGE]);
    pMinFaultBatteryVoltage = &(References[MIN_12V_BATTERY_VOLTAGE]);
	SimpleFastCombineChargeStage2MaxVoltageIntervalPerHour = References[SIMPLE_CONST_I_CHARGE_12V_2ND_STAGE_MAX_DELTA_VOLTAGE];
  }
}

void LoadSimpleFastCombineChargeMenuStage1Tune ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Настройка режима", 16, 16 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uакб=", 5, 5 );
  SimpleFastCombineChargeBatteryVoltageComboBox = MenuAddComboBoxItem ( 2, SimpleFastCombineChargeMenuOnBatteryVoltageSelect );
  MenuAddComboBoxItemElement ( SimpleFastCombineChargeBatteryVoltageComboBox, "24 В", 4 );
  MenuAddComboBoxItemElement ( SimpleFastCombineChargeBatteryVoltageComboBox, "12 В", 4 );
  ComboBoxSetElementIndex ( SimpleFastCombineChargeBatteryVoltageComboBox, SimpleFastCombineChargeBatteryVoltageIndex );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &SimpleFastCombineChargeStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &SimpleFastCombineChargeStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 2 );
}

void LoadSimpleFastCombineChargeMenuStage1Executing ( void ) {
  MenuAddLine ();
  MenuAddTextItem ("Идёт заряд", 10, 16);
  MenuAddLine ();
  MenuAddTextItem ("напряжением…", 12, 16);
  MenuAddLine ();
  MenuAddTextItem ( "Uзар=", 5, 5 );
  MenuAddFixedPointNumberPlayerItem ( &SimpleFastCombineChargeStage1MaxVoltage, 4, 2, 100 ); MenuAddTextItem ( "~", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[BATTERY_VOLTAGE], 4, 2, 100 ); MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &SimpleFastCombineChargeStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[CHARGE_CURRENT], 4, 2, 100 ); MenuAddTextItem ( "А", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &SimpleFastCombineChargeStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();
  MenuAddTextItem ("tзар=", 5, 5); MenuAddTimePlayerItem ( &TotalTime );
  MenuAddLine ();
  MenuAddTextItem ("Qзар=", 5, 5); MenuAddFixedPointNumberPlayerItem ( &TotalVolume, 6, 100, 5 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleFastCombineChargeMenuStage2Executing ( void ) {
  MenuAddLine ();
  MenuAddTextItem ("Идёт заряд", 10, 16);
  MenuAddLine ();
  MenuAddTextItem ("током…", 6, 16);
  MenuAddLine ();
  MenuAddTextItem ( "Uмакс=", 6, 6 ); // Выводим сообщение "Iз=00,00<__,__ А
  MenuAddFixedPointNumberPlayerItem ( &SimpleFastCombineChargeStage2MaxVoltage, 4, 2, 100 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 ); MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &SimpleFastCombineChargeStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( "~", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 2, 100 ); MenuAddTextItem ( "А", 1, 1 );
  MenuAddLine ();
  MenuAddTextItem ("tзар=", 5, 5); MenuAddTimePlayerItem ( &TotalTime );
  MenuAddLine ();
  MenuAddTextItem ("Qзар=", 5, 5); MenuAddFixedPointNumberPlayerItem ( &TotalVolume, 6, 100, 5 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleFastCombineChargeMenuStage1Paused ( void ) {
  MenuAddLine ();
  MenuAddTextItem ("Приостановлен по", 16, 16);
  MenuAddLine ();
  if ( ModeStopOrPauseReason == OVER ) { MenuAddTextItem ("готовности", 10, 16); }
  else if ( ModeStopOrPauseReason == FAULT ) { MenuAddTextItem ("аварии", 6, 16); }
  else if ( ModeStopOrPauseReason == OPERATOR ) { MenuAddTextItem ("решен оператора", 15, 16); }
  else if ( ModeStopOrPauseReason == OVERHEATING ) { MenuAddTextItem ("перегреву модуля", 16, 16); }
  MenuAddLine ();
  MenuAddTextItem ( "Uзар=", 5, 5 );
  MenuAddFixedPointNumberPlayerItem ( &SimpleFastCombineChargeStage1MaxVoltage, 4, 2, 100 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[BATTERY_VOLTAGE], 4, 2, 100 );
  MenuAddTextItem ( chVolts, 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &SimpleFastCombineChargeStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &SimpleFastCombineChargeStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();
  MenuAddTextItem ("tзар=", 5, 5); MenuAddTimePlayerItem ( &TotalTime );
  MenuAddLine ();
  MenuAddTextItem ("Qзар=", 5, 5); MenuAddFixedPointNumberPlayerItem ( &TotalVolume, 6, 100, 5 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleFastCombineChargeMenuStage2Paused ( void ) {
  MenuAddLine ();
  MenuAddTextItem ("Приостановлен по", 16, 16);
  MenuAddLine ();
  if ( ModeStopOrPauseReason == OVER ) { MenuAddTextItem ("готовности", 10, 16); }
  else if ( ModeStopOrPauseReason == FAULT ) { MenuAddTextItem ("аварии", 6, 16); }
  else if ( ModeStopOrPauseReason == OPERATOR ) { MenuAddTextItem ("решен оператора", 15, 16); }
  else if ( ModeStopOrPauseReason == OVERHEATING ) { MenuAddTextItem ("перегреву модуля", 16, 16); }
  MenuAddLine ();
  MenuAddTextItem ( "Uмакс=", 6, 6 ); // Выводим сообщение "Iз=00,00<__,__ А
  MenuAddFixedPointNumberPlayerItem ( &SimpleFastCombineChargeStage2MaxVoltage, 4, 2, 100 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 ); MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &SimpleFastCombineChargeStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( " А", 2, 2 );
  MenuAddLine ();
  MenuAddTextItem ("tзар=", 5, 5); MenuAddTimePlayerItem ( &TotalTime );
  MenuAddLine ();
  MenuAddTextItem ("Qзар=", 5, 5); MenuAddFixedPointNumberPlayerItem ( &TotalVolume, 6, 100, 5 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleFastCombineChargeMenuStage1Over ( void ) {
  MenuAddLine ();
  MenuAddTextItem ("Завершен по", 11, 16);
  MenuAddLine ();
  if ( ModeStopOrPauseReason == OVER ) { MenuAddTextItem ("готовности", 10, 16); }
  else if ( ModeStopOrPauseReason == FAULT ) { MenuAddTextItem ("аварии", 6, 16); }
  else if ( ModeStopOrPauseReason == OPERATOR ) { MenuAddTextItem ("решен оператора", 15, 16); }
  else if ( ModeStopOrPauseReason == OVERHEATING ) { MenuAddTextItem ("перегреву модуля", 16, 16); }
  MenuAddLine ();
  MenuAddTextItem ( "Uакб=", 5, 5 );
  if ( SimpleFastCombineChargeBatteryVoltageIndex == 0 ) { MenuAddTextItem ("24 В", 4, 4); } else { MenuAddTextItem ("12 В", 4, 4); }
  MenuAddLine ();
  MenuAddTextItem ( "Iмакс=", 6, 6 );
  MenuAddFixedPointNumberPlayerItem ( &SimpleFastCombineChargeStage1MaxCurrent, 4, 2, 100 );
  MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();
  MenuAddTextItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberPlayerItem ( &SimpleFastCombineChargeStage2MaxCurrent, 4, 2, 100 );
  MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();
  MenuAddTextItem ("tзар=", 5, 5); MenuAddTimePlayerItem ( &TotalTime );
  MenuAddLine ();
  MenuAddTextItem ("Qзар=", 5, 5); MenuAddFixedPointNumberPlayerItem ( &TotalVolume, 6, 100, 5 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleFastCombineChargeMenu ( void ) {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadMainMenu;
  MenuIndex = SIMPLE_FAST_COMBINE_CHARGE_MENU;
  MenuAddLine ();
  MenuAddTextItem ("*Ускор комб зар*", 16, 16);
  tmpSubsystemState = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
  if ( tmpSubsystemState == 0 ) {
    LoadSimpleFastCombineChargeMenuStage1Tune ();
  }
  else if ( tmpSubsystemState == MENU_MODE_WORKS ) {
    if ( Stage2Reached ) {
      LoadSimpleFastCombineChargeMenuStage2Executing ();
    }
    else { // Если вторая ступень не достигнута, но достигнута первая ступень, то выводим соответствующее сообщение
      LoadSimpleFastCombineChargeMenuStage1Executing ();
    }
  }
  else if ( tmpSubsystemState == MENU_MODE_PAUSED ) {
    if ( Stage2Reached ) {
      LoadSimpleFastCombineChargeMenuStage2Paused ();
    }
    else { // Если вторая ступень не достигнута, но достигнута первая ступень, то выводим соответствующее сообщение
      LoadSimpleFastCombineChargeMenuStage1Paused ();
    }
  }
  else if ( tmpSubsystemState == MENU_MODE_STOPPED ) {
    LoadSimpleFastCombineChargeMenuStage1Over ();
  }
  LoadMenuCommands ();
  MenuAddLine ();
  MenuAddTextItem (chMenuEnd, 16, 16);
}

extern int32_t SlowCombineChargeStage1MaxCurrent;
extern int32_t SlowCombineChargeStage1MaxVoltage;

void LoadSlowCombineChargeMenuStage1Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &SlowCombineChargeStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &SlowCombineChargeStage1MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
}

void LoadSlowCombineChargeMenuStage1Executing ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 ); // Выводим сообщение "Iз=00,00<__,__ А
  MenuAddFixedPointNumberEditorItem ( &SlowCombineChargeStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 );  MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 2, 100 ); MenuAddTextItem ( "А", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberEditorItem ( &SlowCombineChargeStage1MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 ); MenuAddTextItem ( "В", 1, 1 );
}

void LoadSlowCombineChargeMenuStage1Statistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Uзар=", 5, 5 );  // Выводим сообщение "Uз=24,63...26,77 В"
  MenuAddFixedPointNumberPlayerItem ( &Stage1Umin, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Umax, 4, 2, 100 );
  MenuAddTextItem ( chVolts, 2, 3 );
  MenuAddLine ();   // Выводим сообщение "tэтапа=24,63...26,77 В"
  MenuAddTextItem ( chStageTime, 7, 7 ); MenuAddTimePlayerItem ( &Stage1Time );
  MenuAddLine ();   // Выводим сообщение "Qэтапа=532,64 Ач"
  MenuAddTextItem ( chStageVolume, 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSlowCombineChargeMenuStage1Over ( void ) {
   MenuAddLine ();
   MenuAddTextItem ( "Iзар=", 5, 5 );
   MenuAddFixedPointNumberPlayerItem ( &SlowCombineChargeStage1MaxCurrent, 4, 2, 100 ); // Выводим сообщение "Iзар1=24,64 А" без возможности редактирования значения
   MenuAddTextItem ( chAmpers, 2, 7 );
   MenuAddLine ();
   MenuAddTextItem ( "Uмакс=", 6, 6 );
   MenuAddFixedPointNumberPlayerItem ( &SlowCombineChargeStage1MaxVoltage, 4, 2, 100 ); // Выводим сообщение "Umax1=24,64 В" без возможности редактирования значения
   MenuAddTextItem ( chVolts, 2, 7 );
}

void LoadSlowCombineChargeMenuStage1 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadSlowCombineChargeMenuStage1Tune ();
    return;
   }
   else if ( tmpChargeControllerStage == MENU_MODE_WORKS ) {
      if ( Stage2Reached ) { // Если достигнут этап 2, то...
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16); // ...то этап 1 завершен,
     LoadSlowCombineChargeMenuStage1Over ();         // Выводим настройки режима без возможности редактирования
     LoadSlowCombineChargeMenuStage1Statistics ();   // Выводим статистику блока
    }
      else if ( Stage1Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadSlowCombineChargeMenuStage1Executing ();
     LoadSlowCombineChargeMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadSlowCombineChargeMenuStage1Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_PAUSED ) {
      if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadSlowCombineChargeMenuStage1Over ();
     LoadSlowCombineChargeMenuStage1Statistics ();
    }
      else if ( Stage1Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап приостановл", 16, 16);
       LoadSlowCombineChargeMenuStage1Tune ();
     LoadSlowCombineChargeMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadSlowCombineChargeMenuStage1Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_STOPPED ) {
      if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadSlowCombineChargeMenuStage1Over ();
     LoadSlowCombineChargeMenuStage1Statistics ();
    }
      else if ( Stage1Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadSlowCombineChargeMenuStage1Over ();
     LoadSlowCombineChargeMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadSlowCombineChargeMenuStage1Over ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
}

extern int32_t SlowCombineChargeStage2MaxVoltage;
extern int32_t SlowCombineChargeStage2MaxCurrent;
extern int32_t SlowCombineChargeStage2MinCurrent;

void LoadSlowCombineChargeMenuStage2Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Uзар=", 5, 5 ); 
  MenuAddFixedPointNumberEditorItem ( &SlowCombineChargeStage2MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[BATTERY_VOLTAGE], 4, 2, 100 ); MenuAddTextItem ( chVolts, 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмакс=", 6, 6 ); MenuAddFixedPointNumberEditorItem ( &SlowCombineChargeStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( " А", 2, 5 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмин=", 5, 5 ); MenuAddFixedPointNumberEditorItem ( &SlowCombineChargeStage2MinCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( " А", 2, 6 );
}

void LoadSlowCombineChargeMenuStage2Executing ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Uзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &SlowCombineChargeStage2MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[BATTERY_VOLTAGE], 4, 2, 100 );
  // MenuAddTextItem ( chVolts, 2, 3 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &SlowCombineChargeStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[CHARGE_CURRENT], 4, 2, 100 );
  // MenuAddTextItem ( " А", 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмин=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &SlowCombineChargeStage2MinCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( "<", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[CHARGE_CURRENT], 4, 2, 100 );
  // MenuAddTextItem ( " А", 2, 2 );
}

void LoadSlowCombineChargeMenuStage2Statistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Iзар=", 5, 5 );  // Выводим сообщение "Uз=24,63...26,77 В"
  MenuAddFixedPointNumberPlayerItem ( &Stage2Imax, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage2Imin, 4, 2, 100 );
  MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();   // Выводим сообщение "tэтапа=24,63...26,77 В"
  MenuAddTextItem ( chStageTime, 7, 7 ); MenuAddTimePlayerItem ( &Stage2Time );
  MenuAddLine ();   // Выводим сообщение "Qэтапа=532,64 Ач"
  MenuAddTextItem ( chStageVolume, 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &Stage2Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSlowCombineChargeMenuStatistics ( void ) {
  
}

void LoadSlowCombineChargeMenuStage2Over ( void ) {
   MenuAddLine ();
   MenuAddTextItem ( "Uзар=", 5, 5 );
   MenuAddFixedPointNumberPlayerItem ( &SlowCombineChargeStage2MaxVoltage, 4, 2, 100 ); // Выводим сообщение "Iзар1=24,64 А" без возможности редактирования значения
   MenuAddTextItem ( chVolts, 2, 6 );
   MenuAddLine ();
   MenuAddTextItem ( "Iмакс=", 6, 6 );
   MenuAddFixedPointNumberPlayerItem ( &SlowCombineChargeStage2MaxCurrent, 4, 2, 100 ); // Выводим сообщение "Umax1=24,64 В" без возможности редактирования значения
   MenuAddTextItem ( chAmpers, 2, 2 );
   MenuAddLine ();
   MenuAddTextItem ( "Iмин=", 5, 5 );
   MenuAddFixedPointNumberPlayerItem ( &SlowCombineChargeStage2MinCurrent, 4, 2, 100 ); // Выводим сообщение "Umax1=24,64 В" без возможности редактирования значения
   MenuAddTextItem ( chAmpers, 2, 6 );
}

void LoadSlowCombineChargeMenuStage2 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadSlowCombineChargeMenuStage2Tune ();
    return;
   }
   else if ( tmpChargeControllerStage == MENU_MODE_WORKS ) {
      if ( Stage3Reached ) { // Если достигнут этап 2, то...
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16); // ...то этап 1 завершен,
     LoadSlowCombineChargeMenuStage2Over ();         // Выводим настройки режима без возможности редактирования
     LoadSlowCombineChargeMenuStage2Statistics ();   // Выводим статистику блока
    }
      else if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadSlowCombineChargeMenuStage2Executing ();
     LoadSlowCombineChargeMenuStage2Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadSlowCombineChargeMenuStage2Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_PAUSED ) {
      if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadSlowCombineChargeMenuStage2Over ();
     LoadSlowCombineChargeMenuStage2Statistics ();
    }
      else if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап приостановл", 16, 16);
       LoadSlowCombineChargeMenuStage2Tune ();
     LoadSlowCombineChargeMenuStage2Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadSlowCombineChargeMenuStage2Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_STOPPED ) {
      if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadSlowCombineChargeMenuStage2Over ();
     LoadSlowCombineChargeMenuStage2Statistics ();
    }
      else if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadSlowCombineChargeMenuStage2Over ();
     LoadSlowCombineChargeMenuStage2Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadSlowCombineChargeMenuStage2Over ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
}

extern int32_t SlowCombineChargeStage3MaxCurrent;
extern int32_t SlowCombineChargeStage3MaxVoltage;

void LoadSlowCombineChargeMenuStage3Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &SlowCombineChargeStage3MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( " А", 2, 7 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &SlowCombineChargeStage3MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "tзар=", 5, 5 );
  MenuAddTimeEditorItem ( &Stage3MaxTime );
}

void LoadSlowCombineChargeMenuStage3Executing ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &SlowCombineChargeStage3MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 2, 100 );
  MenuAddTextItem ( "А", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &SlowCombineChargeStage3MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddTextItem ( "t=", 2, 2 );
  MenuAddTimePlayerItem ( &Stage3Time );
  MenuAddLine ();
  MenuAddMarkerItem ( "    из ", 7, 7 );
  MenuAddTimeEditorItem ( &Stage3MaxTime );
}

void LoadSlowCombineChargeMenuStage3Statistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Uзар=", 5, 5 );  // Выводим сообщение "Uз=24,63...26,77 В"
  MenuAddFixedPointNumberPlayerItem ( &Stage3Umin, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage3Umax, 4, 2, 100 );
  MenuAddTextItem ( chVolts, 2, 2 );
  MenuAddLine ();   // Выводим сообщение "Qэтапа=532,64 Ач"
  MenuAddTextItem ( chStageVolume, 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &Stage3Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSlowCombineChargeMenuStage3Over ( void ) {
    MenuAddLine ();
  MenuAddTextItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberPlayerItem ( &SlowCombineChargeStage3MaxCurrent, 4, 2, 100 );
  MenuAddTextItem ( " А", 2, 7 );
  MenuAddLine ();
  MenuAddTextItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberPlayerItem ( &SlowCombineChargeStage3MaxVoltage, 4, 2, 100 );
  MenuAddTextItem ( " В", 1, 7 );
    MenuAddLine ();
  MenuAddTextItem ( "tзар=", 5, 5 ); // Выводим сообщение "Iз=00,00<__,__ А
  MenuAddTimePlayerItem ( &Stage3Time );
  MenuAddTextItem ( "", 0, 4 );
  MenuAddLine ();
    MenuAddTextItem ( "  из ", 5, 5 ); MenuAddTimePlayerItem ( &Stage3MaxTime ); MenuAddTextItem ( "", 0, 2 );
}

void LoadSlowCombineChargeMenuStage3 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadSlowCombineChargeMenuStage3Tune ();
    return;
   }
   else if ( tmpChargeControllerStage == MENU_MODE_WORKS ) {
      if ( Stage4Reached ) { // Если достигнут этап 2, то...
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16); // ...то этап 1 завершен,
     LoadSlowCombineChargeMenuStage3Over ();         // Выводим настройки режима без возможности редактирования
     LoadSlowCombineChargeMenuStage3Statistics ();   // Выводим статистику блока
    }
      else if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadSlowCombineChargeMenuStage3Executing ();
     LoadSlowCombineChargeMenuStage3Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadSlowCombineChargeMenuStage3Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_PAUSED ) {
      if ( Stage4Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 16, 16);
     LoadSlowCombineChargeMenuStage3Over ();
     LoadSlowCombineChargeMenuStage3Statistics ();
    }
      else if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап приостановл", 16, 16);
       LoadSlowCombineChargeMenuStage3Tune ();
     LoadSlowCombineChargeMenuStage3Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadSlowCombineChargeMenuStage3Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_STOPPED ) {
      if ( Stage4Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadSlowCombineChargeMenuStage3Over ();
     LoadSlowCombineChargeMenuStage3Statistics ();
    }
      else if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadSlowCombineChargeMenuStage3Over ();
     LoadSlowCombineChargeMenuStage3Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadSlowCombineChargeMenuStage3Over ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
}

void LoadSlowCombineChargeMenu ( void ) {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadDifficultModesMenu;
  MenuIndex = SLOW_COMBINE_CHARGE_MENU;
  MenuAddLine ();
  MenuAddTextItem ("*Медл комб зар**", 16, 16);
  tmpSubsystemState = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
  if ( tmpSubsystemState == 0 ) {
    MenuAddLine ();
    MenuAddTextItem ( "Настройка режима", 16, 16 );
  }
  else if ( tmpSubsystemState == MENU_MODE_WORKS ) {
      // Одна из ступеней режима заряда постоянным током
      if ( Stage3Reached ) { // Если достигнута последняя ступень, то выводим соответствующее сообщение
      MenuAddLine ();
      MenuAddTextItem ("Идёт завершение", 15, 16 );
      MenuAddLine ();
      MenuAddTextItem ("заряда…", 7, 16 );
    }
    else {
      MenuAddLine ();
        MenuAddTextItem ( "Идёт заряд током", 16, 16 );
      if (Stage2Reached ) { // Если последняя ступень не достигнута, но достигнута вторая ступень, то выводим соответствующее сообщение
        MenuAddLine ();
        MenuAddTextItem ("второй ступени…", 15, 16 );
      }
      else if ( Stage1Reached ) { // Если вторая ступень не достигнута, но достигнута первая ступень, то выводим соответствующее сообщение
        MenuAddLine ();
        MenuAddTextItem ("первой ступени…", 15, 16 );
      }
    }
  }
  else {
    if ( tmpSubsystemState == MENU_MODE_PAUSED ) {
      MenuAddLine ();
      MenuAddTextItem ("Приостановлен по", 16, 16 );
    }
    else if ( tmpSubsystemState == MENU_MODE_STOPPED ) {
        MenuAddLine ();
      MenuAddTextItem ("Завершен по", 11, 16 );
    }
    MenuAddLine ();
    if (ModeStopOrPauseReason == OPERATOR ) MenuAddTextItem ("решен. оператора", 16, 16);
    else if ( ModeStopOrPauseReason == OVER ) MenuAddTextItem ("готовности", 10, 16 );
    else if ( ModeStopOrPauseReason == OVERHEATING ) MenuAddTextItem ("перегреву модуля", 16, 16);
    else if ( ModeStopOrPauseReason == FAULT ) MenuAddTextItem ("аварии", 6, 16 );
  }
  MenuAddLine ();
  MenuAddTextItem ( "1. Зар 10-час I", 15, 16 );
  LoadSlowCombineChargeMenuStage1 ();
  MenuAddLine ();
  MenuAddTextItem ( "2. Заряд пост U", 15, 16 );
  LoadSlowCombineChargeMenuStage2 ();
  MenuAddLine ();
  MenuAddTextItem ( "3. Зар 20-час I", 15, 16 );
  LoadSlowCombineChargeMenuStage3 ();
  if ( tmpSubsystemState == 0 ) {
    MenuAddLine ();
    MenuAddTextItem ( "4. Управление", 13, 16 );
  }
  else {
    MenuAddLine ();
    MenuAddTextItem ("4. Статистика", 13, 16 );
    LoadSlowCombineChargeMenuStatistics ();
    MenuAddLine ();
    MenuAddTextItem ("5. Управление", 13, 16 );
  }
  LoadMenuCommands ();
  MenuAddLine ();
  MenuAddTextItem (chMenuEnd, 16, 16);
}

uint64_t FiveMinutes = (uint64_t)((uint64_t)5 * (uint64_t)60 * (uint64_t)96000000);
uint64_t FourtySeconds = (uint64_t)((uint64_t)40 * (uint64_t)96000000);

extern int32_t AsimmChargeStage1MaxCurrent;
extern int32_t AsimmChargeStage1MaxVoltage;
extern int32_t AsimmChargeStage2MaxCurrent;

void CalculateDischargeCurrent ( int32_t ChargeCurrent ) {
   AsimmChargeStage2MaxCurrent = AsimmChargeStage1MaxCurrent/5;
}

void LoadAsimmChargeMenuStage1Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &AsimmChargeStage1MaxCurrent, 4, 2, 100, CalculateDischargeCurrent, 1000, 9000 );
  MenuAddTextItem ( " А", 2, 8 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &AsimmChargeStage1MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddTextItem ( "tзар=", 5, 5 ); MenuAddTimePlayerItem ( &FiveMinutes );
}

void LoadAsimmChargeMenuStage1Executing ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Iзар=", 5, 5 ); // Выводим сообщение "Iз=00,00<__,__ А
  MenuAddFixedPointNumberEditorItem ( &AsimmChargeStage1MaxCurrent, 4, 2, 100, 0, 1000, 9000 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 2, 100 );
  MenuAddTextItem ( " А", 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &AsimmChargeStage1MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddTextItem ( "tзар=", 5, 5 ); MenuAddTimePlayerItem ( &Stage1Time );
  MenuAddLine ();
  MenuAddTextItem ( "     из", 7, 7 ); MenuAddTimePlayerItem ( &FiveMinutes );
}

void LoadAsimmChargeMenuStage1Statistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Uзар=", 5, 5 );  // Выводим сообщение "Uз=24,63...26,77 В"
  MenuAddFixedPointNumberPlayerItem ( &Stage1Umin, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Umax, 4, 2, 100 );
  MenuAddTextItem ( chVolts, 2, 3 );
  MenuAddLine ();   // Выводим сообщение "tэтапа=24,63...26,77 В"
  MenuAddTextItem ( chStageTime, 7, 7 ); MenuAddTimePlayerItem ( &Stage1Time );
  MenuAddLine ();   // Выводим сообщение "Qэтапа=532,64 Ач"
  MenuAddTextItem ( chStageVolume, 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadAsimmChargeMenuStage1Over ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Iзар=", 5, 5 ); // Выводим сообщение "Iз=00,00<__,__ А
  MenuAddFixedPointNumberPlayerItem ( &AsimmChargeStage1MaxCurrent, 4, 2, 100 );
  MenuAddTextItem ( " А", 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberPlayerItem ( &AsimmChargeStage1MaxVoltage, 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
}

void LoadAsimmChargeMenuStage1 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
     LoadAsimmChargeMenuStage1Tune ();
     return;
   }
   else if ( tmpChargeControllerStage == MENU_MODE_WORKS ) {
      if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadAsimmChargeMenuStage1Over ();
     LoadAsimmChargeMenuStage1Statistics ();
    }
    else if ( Stage8Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап отложен", 12, 12);
       LoadAsimmChargeMenuStage1Tune ();
     LoadAsimmChargeMenuStage1Statistics ();
    }
      else if (Stage1Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadAsimmChargeMenuStage1Executing ();
     LoadAsimmChargeMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadAsimmChargeMenuStage1Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_PAUSED ) {
      if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadAsimmChargeMenuStage1Over ();
     LoadAsimmChargeMenuStage1Statistics ();
    }
    else if ( Stage8Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап отложен", 12, 12);
       LoadAsimmChargeMenuStage1Tune ();
     LoadAsimmChargeMenuStage1Statistics ();
    }
      else if (Stage1Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadAsimmChargeMenuStage1Tune ();
     LoadAsimmChargeMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadAsimmChargeMenuStage1Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_STOPPED ) {
      if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadAsimmChargeMenuStage1Over ();
     LoadAsimmChargeMenuStage1Statistics ();
    }
      else if (Stage1Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadAsimmChargeMenuStage1Over ();
     LoadAsimmChargeMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadAsimmChargeMenuStage1Over ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
}

extern int32_t AsimmChargeStage2MinVoltage;

void LoadAsimmChargeMenuStage2Tune ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Iразр=", 6, 6 );
  MenuAddFixedPointNumberPlayerItem ( &AsimmChargeStage2MaxCurrent, 4, 2, 100 );
  MenuAddTextItem ( " А", 2, 8 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмин=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &AsimmChargeStage2MinVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( "<", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddTextItem ( "tразр=", 6, 6 ); MenuAddTimePlayerItem ( &FourtySeconds );
  
}

void LoadAsimmChargeMenuStage2Executing ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iразр=", 6, 6 ); // Выводим сообщение "Iз=00,00<__,__ А
  MenuAddFixedPointNumberPlayerItem ( &AsimmChargeStage2MaxCurrent, 4, 2, 100 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[DISCHARGE_CURRENT]), 4, 2, 100 );
  MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмин=", 5, 5 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberEditorItem ( &AsimmChargeStage2MinVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
}

void LoadAsimmChargeMenuStage2Statistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Uзар=", 5, 5 );  // Выводим сообщение "Uз=24,63...26,77 В"
  MenuAddFixedPointNumberPlayerItem ( &Stage2Umin, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage2Umax, 4, 2, 100 );
  MenuAddTextItem ( chVolts, 2, 3 );
  MenuAddLine ();   // Выводим сообщение "tэтапа=24,63...26,77 В"
  MenuAddTextItem ( chStageTime, 7, 7 ); MenuAddTimePlayerItem ( &Stage2Time );
  MenuAddLine ();   // Выводим сообщение "Qэтапа=532,64 Ач"
  MenuAddTextItem ( chStageVolume, 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &Stage2Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadAsimmChargeMenuStage2Over ( void ) {
   MenuAddLine ();
   MenuAddTextItem ( "Iразр=", 6, 6 );
   MenuAddFixedPointNumberPlayerItem ( &AsimmChargeStage2MaxCurrent, 4, 2, 100 ); // Выводим сообщение "Iзар1=24,64 А" без возможности редактирования значения
   MenuAddTextItem ( chAmpers, 2, 3 );
   MenuAddLine ();
   MenuAddTextItem ( chUmax2, 8, 8 );
   MenuAddFixedPointNumberPlayerItem ( &AsimmChargeStage2MinVoltage, 4, 2, 100 ); // Выводим сообщение "Umax1=24,64 В" без возможности редактирования значения
   MenuAddTextItem ( chVolts, 2, 2 );
}

void LoadAsimmChargeMenuStage2 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadAsimmChargeMenuStage2Tune ();
    return;
   }
   else if ( tmpChargeControllerStage == MENU_MODE_WORKS ) {
      if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadAsimmChargeMenuStage2Over ();
     LoadAsimmChargeMenuStage2Statistics ();
    }
      else if (Stage2Reached ) {
       if ( Stage8Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadAsimmChargeMenuStage2Executing ();
       LoadAsimmChargeMenuStage2Statistics ();
     }
     else {
       MenuAddLine ();
       MenuAddTextItem ("этап отложен", 12, 12);
       LoadAsimmChargeMenuStage2Tune ();
       LoadAsimmChargeMenuStage2Statistics ();   
     }
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadAsimmChargeMenuStage2Tune ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_PAUSED ) {
      if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 16, 16);
     LoadAsimmChargeMenuStage2Over ();
     LoadAsimmChargeMenuStage2Statistics ();
    }
      else if (Stage2Reached ) {
       if ( Stage8Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап приостановл", 16, 16);
       LoadAsimmChargeMenuStage2Tune ();
       LoadAsimmChargeMenuStage2Statistics ();
     }
     else {
       MenuAddLine ();
       MenuAddTextItem ("этап отложен", 12, 12);
       LoadAsimmChargeMenuStage2Tune ();
       LoadAsimmChargeMenuStage2Statistics ();   
     }
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadAsimmChargeMenuStage2Tune ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_STOPPED ) {
      if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadAsimmChargeMenuStage2Over ();
     LoadAsimmChargeMenuStage2Statistics ();
    }
      else if (Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadAsimmChargeMenuStage2Over ();
     LoadAsimmChargeMenuStage2Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadAsimmChargeMenuStage2Over ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
}

extern int32_t AsimmChargeStage3MaxVoltage;
extern int32_t AsimmChargeStage3MaxCurrent;
extern int32_t AsimmChargeStage3MinCurrent;

void LoadAsimmChargeMenuStage3Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &AsimmChargeStage3MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &AsimmChargeStage3MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( " А", 2, 2 );
    MenuAddLine ();
  MenuAddMarkerItem ( "Iмин=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &AsimmChargeStage3MinCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( " А", 2, 2 );
}

void LoadAsimmChargeMenuStage3Executing ( void ) { 
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &AsimmChargeStage3MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &AsimmChargeStage3MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 2, 100 );
  MenuAddTextItem ( " А", 2, 2 );
    MenuAddLine ();
  MenuAddMarkerItem ( "Iмин=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &AsimmChargeStage3MinCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( " А", 2, 2 );
}

void LoadAsimmChargeMenuStage3Statistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberPlayerItem ( &Stage3Imax, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage3Imin, 4, 2, 100 );
  MenuAddTextItem ( " В", 2, 2 );
  MenuAddLine ();
  MenuAddTextItem ( "Qэтапа=", 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &Stage3Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadAsimmChargeMenuStage3Over ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberPlayerItem ( &AsimmChargeStage3MaxVoltage, 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмакс=", 6, 6 );
  MenuAddFixedPointNumberPlayerItem ( &AsimmChargeStage3MaxCurrent, 4, 2, 100 );
  MenuAddTextItem ( " А", 2, 2 );
    MenuAddLine ();
  MenuAddMarkerItem ( "Iмин=", 3, 3 );
  MenuAddFixedPointNumberPlayerItem ( &AsimmChargeStage3MinCurrent, 4, 2, 100 );
  MenuAddTextItem ( " А", 2, 2 );
}

void LoadAsimmChargeMenuStage3 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadAsimmChargeMenuStage3Tune ();
    return;
   }
   else if ( tmpChargeControllerStage == MENU_MODE_WORKS ) {
      if ( Stage4Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadAsimmChargeMenuStage3Over ();
     LoadAsimmChargeMenuStage3Statistics ();
    }
      else if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadAsimmChargeMenuStage3Executing ();
     LoadAsimmChargeMenuStage3Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadAsimmChargeMenuStage3Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_PAUSED ) {
      if ( Stage4Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadAsimmChargeMenuStage3Over ();
     LoadAsimmChargeMenuStage3Statistics ();
    }
      else if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadAsimmChargeMenuStage3Tune ();
     LoadAsimmChargeMenuStage3Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadAsimmChargeMenuStage3Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_STOPPED ) {
      if ( Stage4Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadAsimmChargeMenuStage3Over ();
     LoadAsimmChargeMenuStage3Statistics ();
    }
      else if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadAsimmChargeMenuStage3Over ();
     LoadAsimmChargeMenuStage3Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadAsimmChargeMenuStage3Over ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
  }
}

extern int32_t AsimmChargeStage4MaxVoltage;
extern int32_t AsimmChargeStage4MaxCurrent;

void LoadAsimmChargeMenuStage4Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Uзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &AsimmChargeStage4MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "tзар=", 5, 5 );
  MenuAddTimeEditorItem ( &Stage4MaxTime );
}

void LoadAsimmChargeMenuStage4Executing ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Uзар=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &AsimmChargeStage4MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 );MenuAddFixedPointNumberPlayerItem ( &Values[BATTERY_VOLTAGE], 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddTextItem ( "tэтапа=", 7, 7 ); MenuAddTimePlayerItem ( &Stage4Time );
  MenuAddLine ();
  MenuAddMarkerItem ( "    из ", 7, 7 ); MenuAddTimeEditorItem ( &Stage4MaxTime );
  MenuAddLine ();
  MenuAddTextItem ( "Iзар = ", 7, 7 );
  MenuAddFixedPointNumberPlayerItem ( &AsimmChargeStage4MaxCurrent, 4, 2, 100 );
  MenuAddTextItem ( " А", 2, 3 );
}

void LoadAsimmChargeMenuStage4Statistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberPlayerItem ( &Stage3Imax, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage3Imin, 4, 2, 100 );
  MenuAddTextItem ( "А", 1, 1 );
  MenuAddLine ();
  MenuAddTextItem ( "Qэтапа=", 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &Stage4Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadAsimmChargeMenuStage4Over ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Iзар = ", 7, 7 );
  MenuAddFixedPointNumberPlayerItem ( &AsimmChargeStage4MaxCurrent, 4, 2, 100 );
  MenuAddTextItem ( " А", 2, 3 );
  MenuAddLine ();
  MenuAddTextItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberPlayerItem ( &AsimmChargeStage4MaxVoltage, 4, 2, 100 );
  MenuAddTextItem ( " В", 2, 2 );
}

void LoadAsimmChargeMenuStage4 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadAsimmChargeMenuStage4Tune ();
    return;
   }
   else if ( tmpChargeControllerStage == MENU_MODE_WORKS ) {
      if ( Stage5Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadAsimmChargeMenuStage4Over ();
     LoadAsimmChargeMenuStage4Statistics ();
    }
      else if ( Stage4Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadAsimmChargeMenuStage4Executing ();
     LoadAsimmChargeMenuStage4Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadAsimmChargeMenuStage4Tune ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_PAUSED ) {
      if ( Stage5Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadAsimmChargeMenuStage4Over ();
     LoadAsimmChargeMenuStage4Statistics ();
    }
      else if ( Stage4Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadAsimmChargeMenuStage4Tune ();
     LoadAsimmChargeMenuStage4Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadAsimmChargeMenuStage4Tune ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_STOPPED ) {
      if ( Stage5Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadAsimmChargeMenuStage4Over ();
     LoadAsimmChargeMenuStage4Statistics ();
    }
      else if ( Stage4Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadAsimmChargeMenuStage4Over ();
     LoadAsimmChargeMenuStage4Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadAsimmChargeMenuStage4Over ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
  }
}

void LoadAsimmChargeMenuStatistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "tобщ = ", 7, 7 ); MenuAddTimePlayerItem ( &TotalTime );
  MenuAddLine ();
  MenuAddTextItem ( "Qобщ = ", 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &TotalVolume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadAsimmChargeMenu ( void ) {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadDifficultModesMenu;
  MenuIndex = ASIMM_CHARGE_MENU;
  MenuAddLine ();
  MenuAddTextItem ("**Заряд асимм I*", 16, 16);
  tmpSubsystemState = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
  if ( tmpSubsystemState == 0 ) {
    MenuAddLine ();
    MenuAddTextItem ( "Настройка режима", 16, 16 );
  }
  else if ( tmpSubsystemState == MENU_MODE_WORKS ) {
      // Одна из ступеней режима заряда постоянным током
      if ( Stage4Reached ) { // Если достигнута последняя ступень, то выводим соответствующее сообщение
      MenuAddLine ();
      MenuAddTextItem ("Идёт завершение", 15, 16 );
      MenuAddLine ();
      MenuAddTextItem ("заряда…", 7, 16 );
    }
    else if ( Stage3Reached ) {
      MenuAddLine ();
      MenuAddTextItem ("Идёт заряд пост", 15, 15 );
      MenuAddLine ();
      MenuAddTextItem ("напряжением…", 12, 12 );
    }
    else {
      if (Stage8Reached ) { // Если последняя ступень не достигнута, но достигнута вторая ступень, то выводим соответствующее сообщение
        MenuAddLine ();
        MenuAddTextItem ("Идёт разряд…", 12, 12 );
      }
      else if ( Stage1Reached ) { // Если вторая ступень не достигнута, но достигнута первая ступень, то выводим соответствующее сообщение
        MenuAddLine ();
        MenuAddTextItem ( "Идёт заряд током", 16, 16 );
        MenuAddLine ();
        MenuAddTextItem ("первой ступени…", 15, 16 );
      }
    }
  }
  else {
    if ( tmpSubsystemState == MENU_MODE_PAUSED ) {
      MenuAddLine ();
      MenuAddTextItem ("Приостановлен по", 16, 16 );
    }
    else if ( tmpSubsystemState == MENU_MODE_STOPPED ) {
        MenuAddLine ();
      MenuAddTextItem ("Завершен по", 11, 16 );
    }
    MenuAddLine ();
    if (ModeStopOrPauseReason == OPERATOR ) MenuAddTextItem ("решен. оператора", 16, 16);
    else if ( ModeStopOrPauseReason == OVER ) MenuAddTextItem ("готовности", 10, 16 );
    else if ( ModeStopOrPauseReason == OVERHEATING ) MenuAddTextItem ("перегреву модуля", 16, 16);
    else if ( ModeStopOrPauseReason == FAULT ) MenuAddTextItem ("аварии", 6, 16 );
  }
  MenuAddLine ();
  MenuAddTextItem ( "1. Цикл зар-разр", 16, 16 );
  LoadAsimmChargeMenuStage1 ();
  LoadAsimmChargeMenuStage2 ();
  MenuAddLine ();
  MenuAddTextItem ( "2. Заряд пост U", 15, 16 );
  LoadAsimmChargeMenuStage3 ();
  MenuAddLine ();
  MenuAddTextItem ( "3. Дозар пост I", 15, 16 );
  LoadAsimmChargeMenuStage4 ();
  if ( tmpSubsystemState == 0 ) {
    MenuAddLine ();
    MenuAddTextItem ( "4. Управление", 13, 16 );
  }
  else {
    MenuAddLine ();
    MenuAddTextItem ("4. Статистика", 13, 16 );
    LoadAsimmChargeMenuStatistics ();
    MenuAddLine ();
    MenuAddTextItem ("5. Управление", 13, 16 );
  }
  LoadMenuCommands ();
  MenuAddLine ();
  MenuAddTextItem (chMenuEnd, 16, 16);
}

extern int32_t ConstIWithLimitUChargeStage1MaxCurrent;
extern int32_t ConstIWithLimitUChargeStage1MaxVoltage;

void LoadConstIWithLimitUChargeMenuStage1Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &ConstIWithLimitUChargeStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &ConstIWithLimitUChargeStage1MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
}

void LoadConstIWithLimitUChargeMenuStage1Executing ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 ); // Выводим сообщение "Iз=00,00<__,__ А
  MenuAddFixedPointNumberEditorItem ( &ConstIWithLimitUChargeStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 2, 100 ); MenuAddTextItem ( "А", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberEditorItem ( &ConstIWithLimitUChargeStage1MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 ); MenuAddTextItem ( "В", 1, 1 );
}

void LoadConstIWithLimitUChargeMenuStage1Statistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Uзар=", 5, 5 );  // Выводим сообщение "Uз=24,63...26,77 В"
  MenuAddFixedPointNumberPlayerItem ( &Stage1Umin, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Umax, 4, 2, 100 );
  MenuAddTextItem ( chVolts, 2, 3 );
  MenuAddLine ();   // Выводим сообщение "tэтапа=24,63...26,77 В"
  MenuAddTextItem ( chStageTime, 7, 7 ); MenuAddTimePlayerItem ( &Stage1Time );
  MenuAddLine ();   // Выводим сообщение "Qэтапа=532,64 Ач"
  MenuAddTextItem ( chStageVolume, 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadConstIWithLimitUChargeMenuStage1Over ( void ) {
   MenuAddLine ();
   MenuAddTextItem ( "Iзар=", 5, 5 );
   MenuAddFixedPointNumberPlayerItem ( &ConstIWithLimitUChargeStage1MaxCurrent, 4, 2, 100 ); // Выводим сообщение "Iзар1=24,64 А" без возможности редактирования значения
   MenuAddTextItem ( chAmpers, 2, 7 );
   MenuAddLine ();
   MenuAddTextItem ( "Uмакс=", 6, 6 );
   MenuAddFixedPointNumberPlayerItem ( &ConstIWithLimitUChargeStage1MaxVoltage, 4, 2, 100 ); // Выводим сообщение "Umax1=24,64 В" без возможности редактирования значения
   MenuAddTextItem ( chVolts, 2, 7 );
}

void LoadConstIWithLimitUChargeMenuStage1 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadConstIWithLimitUChargeMenuStage1Tune ();
      return;
   }
   else if ( tmpChargeControllerStage == MENU_MODE_WORKS ) {
      if ( Stage2Reached ) { // Если достигнут этап 2, то...
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16); // ...то этап 1 завершен,
       LoadConstIWithLimitUChargeMenuStage1Over ();         // Выводим настройки режима без возможности редактирования
       LoadConstIWithLimitUChargeMenuStage1Statistics ();   // Выводим статистику блока
    }
      else if ( Stage1Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadConstIWithLimitUChargeMenuStage1Executing ();
       LoadConstIWithLimitUChargeMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
       MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadConstIWithLimitUChargeMenuStage1Tune ();
       MenuAddLine ();
       MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_PAUSED ) {
      if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
       LoadConstIWithLimitUChargeMenuStage1Over ();
       LoadConstIWithLimitUChargeMenuStage1Statistics ();
    }
      else if ( Stage1Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап приостановл", 16, 16);
       LoadConstIWithLimitUChargeMenuStage1Tune ();
       LoadConstIWithLimitUChargeMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
       MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadConstIWithLimitUChargeMenuStage1Tune ();
       MenuAddLine ();
       MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_STOPPED ) {
      if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
       LoadConstIWithLimitUChargeMenuStage1Over ();
       LoadConstIWithLimitUChargeMenuStage1Statistics ();
    }
      else if ( Stage1Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadConstIWithLimitUChargeMenuStage1Over ();
       LoadConstIWithLimitUChargeMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
       MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadConstIWithLimitUChargeMenuStage1Over ();
       MenuAddLine ();
       MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
}

extern int32_t ConstIWithLimitUChargeStage2MaxVoltage;
extern int32_t ConstIWithLimitUChargeStage2MaxCurrent;
extern int32_t ConstIWithLimitUChargeStage2MinVoltage;

void LoadConstIWithLimitUChargeMenuStage2Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Uзар=", 5, 5 ); 
  MenuAddFixedPointNumberEditorItem ( &ConstIWithLimitUChargeStage2MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[BATTERY_VOLTAGE], 4, 2, 100 ); MenuAddTextItem ( chVolts, 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмакс=", 6, 6 ); MenuAddFixedPointNumberEditorItem ( &ConstIWithLimitUChargeStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( " А", 2, 5 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмин=", 5, 5 ); MenuAddFixedPointNumberEditorItem ( &ConstIWithLimitUChargeStage2MinVoltage, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( " А", 2, 6 );
}

void LoadConstIWithLimitUChargeMenuStage2Executing ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Uзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &ConstIWithLimitUChargeStage2MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[BATTERY_VOLTAGE], 4, 2, 100 );
  // MenuAddTextItem ( chVolts, 2, 3 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &ConstIWithLimitUChargeStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[CHARGE_CURRENT], 4, 2, 100 );
  // MenuAddTextItem ( " А", 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Iмин=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &ConstIWithLimitUChargeStage2MinVoltage, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( "<", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[CHARGE_CURRENT], 4, 2, 100 );
  // MenuAddTextItem ( " А", 2, 2 );
}

void LoadConstIWithLimitUChargeMenuStage2Statistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Iзар=", 5, 5 );  // Выводим сообщение "Uз=24,63...26,77 В"
  MenuAddFixedPointNumberPlayerItem ( &Stage2Imax, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage2Imin, 4, 2, 100 );
  MenuAddTextItem ( chAmpers, 2, 2 );
}

void LoadConstIWithLimitUChargeMenuStage2Over ( void ) {
   MenuAddLine ();
   MenuAddTextItem ( "Uзар=", 5, 5 );
   MenuAddFixedPointNumberPlayerItem ( &ConstIWithLimitUChargeStage2MaxVoltage, 4, 2, 100 ); // Выводим сообщение "Iзар1=24,64 А" без возможности редактирования значения
   MenuAddTextItem ( chVolts, 2, 6 );
   MenuAddLine ();
   MenuAddTextItem ( "Iмакс=", 6, 6 );
   MenuAddFixedPointNumberPlayerItem ( &ConstIWithLimitUChargeStage2MaxCurrent, 4, 2, 100 ); // Выводим сообщение "Umax1=24,64 В" без возможности редактирования значения
   MenuAddTextItem ( chAmpers, 2, 2 );
   MenuAddLine ();
   MenuAddTextItem ( "Iмин=", 5, 5 );
   MenuAddFixedPointNumberPlayerItem ( &ConstIWithLimitUChargeStage2MinVoltage, 4, 2, 100 ); // Выводим сообщение "Umax1=24,64 В" без возможности редактирования значения
   MenuAddTextItem ( chAmpers, 2, 6 );
}

void LoadConstIWithLimitUChargeMenuStage2 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadConstIWithLimitUChargeMenuStage2Tune ();
    return;
   }
   else if ( tmpChargeControllerStage == MENU_MODE_WORKS ) {
      if ( Stage3Reached ) { // Если достигнут этап 2, то...
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16); // ...то этап 1 завершен,
     LoadConstIWithLimitUChargeMenuStage2Over ();         // Выводим настройки режима без возможности редактирования
     LoadConstIWithLimitUChargeMenuStage2Statistics ();   // Выводим статистику блока
    }
      else if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadConstIWithLimitUChargeMenuStage2Executing ();
     LoadConstIWithLimitUChargeMenuStage2Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadConstIWithLimitUChargeMenuStage2Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_PAUSED ) {
      if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadConstIWithLimitUChargeMenuStage2Over ();
     LoadConstIWithLimitUChargeMenuStage2Statistics ();
    }
      else if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап приостановл", 16, 16);
       LoadConstIWithLimitUChargeMenuStage2Tune ();
     LoadConstIWithLimitUChargeMenuStage2Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadConstIWithLimitUChargeMenuStage2Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage == MENU_MODE_STOPPED ) {
      if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadConstIWithLimitUChargeMenuStage2Over ();
     LoadConstIWithLimitUChargeMenuStage2Statistics ();
    }
      else if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadConstIWithLimitUChargeMenuStage2Over ();
     LoadConstIWithLimitUChargeMenuStage2Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadConstIWithLimitUChargeMenuStage2Over ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
}

extern int32_t ConstIWithLimitUChargeStage3MaxVoltage;
extern int32_t ConstIWithLimitUChargeStage3MaxCurrent;

void LoadConstIWithLimitUChargeMenuStage3Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Uзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &ConstIWithLimitUChargeStage3MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "tзар=", 5, 5 );
  MenuAddTimeEditorItem ( &Stage3MaxTime );
}

void LoadConstIWithLimitUChargeMenuStage3Executing ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Uзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &ConstIWithLimitUChargeStage3MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 );MenuAddFixedPointNumberPlayerItem ( &Values[BATTERY_VOLTAGE], 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ( "tзар=", 6, 6 );   MenuAddTimeEditorItem ( &Stage3Time );
  MenuAddLine ();
  MenuAddMarkerItem ( "    из ", 7, 7 ); MenuAddTimeEditorItem ( &Stage3MaxTime );
  MenuAddLine ();
  MenuAddTextItem ( "Iзар = ", 7, 7 );
  MenuAddFixedPointNumberPlayerItem ( &Values[CHARGE_CURRENT], 4, 2, 100 );
  MenuAddTextItem ( " А", 2, 3 );
}

void LoadConstIWithLimitUChargeMenuStage3Statistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberPlayerItem ( &Stage2Umin, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage2Umax, 4, 2, 100 );
  MenuAddTextItem ( " В", 2, 2 );
  MenuAddLine ();
  MenuAddTextItem ( "Qэтапа=", 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &Stage3Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadConstIWithLimitUChargeMenuStage3Over ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Iзар = ", 7, 7 );
  MenuAddFixedPointNumberPlayerItem ( &ConstIWithLimitUChargeStage3MaxCurrent, 4, 2, 100 );
  MenuAddTextItem ( " А", 2, 3 );
  MenuAddLine ();
  MenuAddTextItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberPlayerItem ( &ConstIWithLimitUChargeStage3MaxVoltage, 4, 2, 100 );
  MenuAddTextItem ( " В", 2, 2 );
  MenuAddLine ();
  MenuAddTextItem ( "Uзар=", 5, 5 );
  MenuAddFixedPointNumberPlayerItem ( &Stage2Umin, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage2Umax, 4, 2, 100 );
  MenuAddTextItem ( " В", 2, 2 );
}

void LoadConstIWithLimitUChargeMenuStage3 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadConstIWithLimitUChargeMenuStage3Tune ();
    return;
   }
   else if ( tmpChargeControllerStage & MENU_MODE_WORKS ) {
      if ( Stage4Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadConstIWithLimitUChargeMenuStage3Over ();
     LoadConstIWithLimitUChargeMenuStage3Statistics ();
    }
      else if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadConstIWithLimitUChargeMenuStage3Executing ();
     LoadConstIWithLimitUChargeMenuStage3Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadConstIWithLimitUChargeMenuStage3Tune ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage & MENU_MODE_PAUSED ) {
      if ( Stage4Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadConstIWithLimitUChargeMenuStage3Over ();
     LoadConstIWithLimitUChargeMenuStage3Statistics ();
    }
      else if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadConstIWithLimitUChargeMenuStage3Tune ();
     LoadConstIWithLimitUChargeMenuStage3Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadConstIWithLimitUChargeMenuStage3Tune ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage & MENU_MODE_STOPPED ) {
      if ( Stage4Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadConstIWithLimitUChargeMenuStage3Over ();
     LoadConstIWithLimitUChargeMenuStage3Statistics ();
    }
      else if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadConstIWithLimitUChargeMenuStage3Over ();
     LoadConstIWithLimitUChargeMenuStage3Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadConstIWithLimitUChargeMenuStage3Over ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
}

void LoadConstIWithLimitUChargeMenuStatistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ("4. Общие данные", 15, 16 );
  MenuAddLine ();
  MenuAddTextItem ( "tобщ = ", 7, 7 ); MenuAddTimePlayerItem ( &TotalTime );
  MenuAddLine ();
  MenuAddTextItem ( "Qобщ = ", 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &TotalVolume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadConstIWithLimitUChargeMenu ( void ) {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadDifficultModesMenu;
  MenuIndex = CONST_I_WITH_LIMIT_U_CHARGE_MENU;
  MenuAddLine ();
  MenuAddTextItem ("**Зар I с огр U*", 16, 16);
  tmpSubsystemState = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
  if ( tmpSubsystemState == 0 ) {
    MenuAddLine ();
    MenuAddTextItem ( "Настройка режима", 16, 16 );
  }
  else if ( tmpSubsystemState == MENU_MODE_WORKS ) {
      // Одна из ступеней режима заряда постоянным током
      if ( Stage3Reached ) { // Если достигнута последняя ступень, то выводим соответствующее сообщение
      MenuAddLine ();
      MenuAddTextItem ("Идёт завершение", 15, 16 );
      MenuAddLine ();
      MenuAddTextItem ("заряда…", 7, 16 );
    }
    else {
      MenuAddLine ();
        MenuAddTextItem ( "Идёт заряд током", 16, 16 );
      if (Stage2Reached ) { // Если последняя ступень не достигнута, но достигнута вторая ступень, то выводим соответствующее сообщение
        MenuAddLine ();
        MenuAddTextItem ("второй ступени…", 15, 16 );
      }
      else if ( Stage1Reached ) { // Если вторая ступень не достигнута, но достигнута первая ступень, то выводим соответствующее сообщение
        MenuAddLine ();
        MenuAddTextItem ("первой ступени…", 15, 16 );
      }
    }
  }
  else {
    if ( tmpSubsystemState == MENU_MODE_PAUSED ) {
      MenuAddLine ();
      MenuAddTextItem ("Приостановлен по", 16, 16 );
    }
    else if ( tmpSubsystemState == MENU_MODE_STOPPED ) {
        MenuAddLine ();
      MenuAddTextItem ("Завершен по", 11, 16 );
    }
    MenuAddLine ();
    if (ModeStopOrPauseReason == OPERATOR ) MenuAddTextItem ("решен. оператора", 16, 16);
    else if ( ModeStopOrPauseReason == OVER ) MenuAddTextItem ("готовности", 10, 16 );
    else if ( ModeStopOrPauseReason == OVERHEATING ) MenuAddTextItem ("перегреву модуля", 16, 16);
    else if ( ModeStopOrPauseReason == FAULT ) MenuAddTextItem ("аварии", 6, 16 );
  }
  MenuAddLine ();
  MenuAddTextItem ( "1. Зар 10-час I", 15, 16 );
  LoadConstIWithLimitUChargeMenuStage1 ();
  MenuAddLine ();
  MenuAddTextItem ( "2. Заряд пост U", 15, 16 );
  LoadConstIWithLimitUChargeMenuStage2 ();
  MenuAddLine ();
  MenuAddTextItem ( "3. Дозар пост I", 15, 16 );
  LoadConstIWithLimitUChargeMenuStage3 ();
  if ( tmpSubsystemState == 0 ) {
    MenuAddLine ();
    MenuAddTextItem ( "4. Управление", 13, 16 );
  }
  else {
    MenuAddLine ();
    MenuAddTextItem ("4. Статистика", 13, 16 );
    LoadConstIWithLimitUChargeMenuStatistics ();
    MenuAddLine ();
    MenuAddTextItem ("5. Управление", 13, 16 );
  }
  LoadMenuCommands ();
  MenuAddLine ();
  MenuAddTextItem (chMenuEnd, 16, 16);
}







extern int32_t CTCStage1MaxCurrent;
extern int32_t CTCStage1MaxVoltage;

void LoadCTCMenuStage1Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &CTCStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 3 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &CTCStage1MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
}

void LoadCTCMenuStage1Executing ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 ); MenuAddFixedPointNumberEditorItem ( &CTCStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 2, 100 );
  MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberEditorItem ( &CTCStage1MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
}

void LoadCTCMenuStage1Statistics ( void ) {
  if ( Stage1Reached ) {
    MenuAddLine ();
    MenuAddTextItem ( "Uзар=", 5, 5 );  // Выводим сообщение "Uз=24,63...26,77 В"
    MenuAddFixedPointNumberPlayerItem ( &Stage1Umin, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Umax, 4, 2, 100 );
    MenuAddTextItem ( chVolts, 2, 3 );
    MenuAddLine ();   // Выводим сообщение "tэтапа=24,63...26,77 В"
    MenuAddTextItem ( chStageTime, 7, 7 ); MenuAddTimePlayerItem ( &Stage1Time );
    MenuAddLine ();   // Выводим сообщение "Qэтапа=532,64 Ач"
    MenuAddTextItem ( chStageVolume, 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
  }
  else {
    MenuAddLine ();
    MenuAddTextItem ( 0, 16, 16 );
  }
}

void LoadCTCMenuStage1Over ( void ) {
   MenuAddLine ();
   MenuAddTextItem ( "Iзар=", 5, 5 );
   MenuAddFixedPointNumberPlayerItem ( &CTCStage1MaxCurrent, 4, 2, 100 ); // Выводим сообщение "Iзар1=24,64 А" без возможности редактирования значения
   MenuAddTextItem ( chAmpers, 2, 3 );
   MenuAddLine ();
   MenuAddTextItem ( "Uмакс=", 6, 6 ); 
   MenuAddFixedPointNumberPlayerItem ( &CTCStage1MaxVoltage, 4, 2, 100 ); // Выводим сообщение "Umax1=24,64 В" без возможности редактирования значения
   MenuAddTextItem ( chVolts, 2, 2 );
}

void LoadCTCMenuStage1 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadCTCMenuStage1Tune ();
    return;
   }
   else if ( tmpChargeControllerStage & MENU_MODE_WORKS ) {
      if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadCTCMenuStage1Over ();
     LoadCTCMenuStage1Statistics ();
    }
      else if (Stage1Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadCTCMenuStage1Executing ();
     LoadCTCMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage1Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage & MENU_MODE_PAUSED ) {
      if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 16, 16);
     LoadCTCMenuStage1Over ();
     LoadCTCMenuStage1Statistics ();
    }
      else if (Stage1Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadCTCMenuStage1Tune ();
     LoadCTCMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage1Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage & MENU_MODE_STOPPED ) {
      if ( Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadCTCMenuStage1Over ();
     LoadCTCMenuStage1Statistics ();
    }
      else if (Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadCTCMenuStage1Over ();
     LoadCTCMenuStage1Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage1Over ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
}

extern int32_t CTCStage2MaxCurrent;
extern int32_t CTCStage2MaxVoltage;
extern int32_t CTCStage2MinVoltageIntervalPerHour;

void LoadCTCMenuStage2Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &CTCStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 3 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &CTCStage2MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
}

void LoadCTCMenuStage2Executing ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 ); MenuAddFixedPointNumberEditorItem ( &CTCStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 2, 100 );
  MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberEditorItem ( &CTCStage2MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
}

void LoadCTCMenuStage2Statistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Uзар=", 5, 5 );  // Выводим сообщение "Uз=24,63...26,77 В"
  MenuAddFixedPointNumberPlayerItem ( &Stage2Umin, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage2Umax, 4, 2, 100 );
  MenuAddTextItem ( chVolts, 2, 3 );
  MenuAddLine ();   // Выводим сообщение "tэтапа=24,63...26,77 В"
  MenuAddTextItem ( chStageTime, 7, 7 ); MenuAddTimePlayerItem ( &Stage2Time );
  MenuAddLine ();   // Выводим сообщение "Qэтапа=532,64 Ач"
  MenuAddTextItem ( chStageVolume, 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &Stage2Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadCTCMenuStage2Over ( void ) {
   MenuAddLine ();
   MenuAddTextItem ( "Iзар=", 5, 5 );
   MenuAddFixedPointNumberPlayerItem ( &CTCStage2MaxCurrent, 4, 2, 100 ); // Выводим сообщение "Iзар1=24,64 А" без возможности редактирования значения
   MenuAddTextItem ( chAmpers, 2, 3 );
   MenuAddLine ();
   MenuAddTextItem ( "Uмакс=", 6, 6 );
   MenuAddFixedPointNumberPlayerItem ( &CTCStage2MaxVoltage, 4, 2, 100 ); // Выводим сообщение "Umax1=24,64 В" без возможности редактирования значения
   MenuAddTextItem ( chVolts, 2, 2 );
}

void LoadCTCMenuStage2 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadCTCMenuStage2Tune ();
    return;
   }
   else if ( tmpChargeControllerStage & MENU_MODE_WORKS ) {
      if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadCTCMenuStage2Over ();
     LoadCTCMenuStage2Statistics ();
    }
      else if (Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadCTCMenuStage2Executing ();
     LoadCTCMenuStage2Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage2Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage & MENU_MODE_PAUSED ) {
      if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 16, 16);
     LoadCTCMenuStage2Over ();
     LoadCTCMenuStage2Statistics ();
    }
      else if (Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап приостановл", 16, 16);
       LoadCTCMenuStage2Tune ();
     LoadCTCMenuStage2Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage2Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage & MENU_MODE_STOPPED ) {
      if ( Stage3Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadCTCMenuStage2Over ();
     LoadCTCMenuStage2Statistics ();
    }
      else if (Stage2Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadCTCMenuStage2Over ();
     LoadCTCMenuStage2Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage2Over ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
}

void LoadCTCMenuStage4Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "tпауз=", 6, 6 );
  MenuAddTimeEditorItem ( &Stage4MaxTime );
}

void LoadCTCMenuStage4Executing ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "tпауз=", 6, 6 ); MenuAddTimePlayerItem ( &Stage4Time );
  MenuAddLine ();
  MenuAddMarkerItem ( "    из ", 7, 7 ); MenuAddTimeEditorItem ( &Stage4MaxTime );
}

void LoadCTCMenuStage4Statistics ( void ) {
}

void LoadCTCMenuStage4Over ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "tпауз=", 6, 6 ); MenuAddTimePlayerItem ( &Stage4Time );
}

void LoadCTCMenuStage4 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadCTCMenuStage4Tune ();
    return;
   }
   else if ( tmpChargeControllerStage & MENU_MODE_WORKS ) {
      if ( Stage5Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadCTCMenuStage4Over ();
     LoadCTCMenuStage4Statistics ();
    }
      else if ( Stage4Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadCTCMenuStage4Executing ();
     LoadCTCMenuStage4Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage4Tune ();
    }
   }
   else if ( tmpChargeControllerStage & MENU_MODE_PAUSED ) {
      if ( Stage5Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadCTCMenuStage4Over ();
     LoadCTCMenuStage4Statistics ();
    }
      else if ( Stage4Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadCTCMenuStage4Tune ();
     LoadCTCMenuStage4Statistics ();
    }
     else {
       MenuAddLine ();
       MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage4Tune ();
     }
   }
   else if ( tmpChargeControllerStage & MENU_MODE_STOPPED ) {
      if ( Stage5Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadCTCMenuStage4Over ();
     LoadCTCMenuStage4Statistics ();
    }
      else if ( Stage4Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadCTCMenuStage4Over ();
     LoadCTCMenuStage4Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage4Over ();
    }
  }
}

extern int32_t CTCStage5MaxCurrent;
extern int32_t CTCStage5MinVoltage;

void LoadCTCMenuStage5Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iразр=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &CTCStage5MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( " А    ", 6, 6 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмин=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &CTCStage5MinVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( "<", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( " В", 2, 2 );
}

void LoadCTCMenuStage5Executing ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iразр=", 6, 6 ); MenuAddFixedPointNumberEditorItem ( &CTCStage5MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem(">", 1, 1); MenuAddFixedPointNumberPlayerItem ( &Values[DISCHARGE_CURRENT], 4, 2, 100 );
  MenuAddTextItem (" А", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмин=", 5, 5 ); MenuAddFixedPointNumberEditorItem ( &CTCStage5MinVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem("<", 1, 1); MenuAddFixedPointNumberPlayerItem ( &Values[BATTERY_VOLTAGE], 4, 2, 100 );
  MenuAddTextItem (" В", 2, 2);
}

void LoadCTCMenuStage5Statistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Uакб=", 5, 5 );
  MenuAddFixedPointNumberPlayerItem ( &Stage5Umax, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage5Umin, 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddTextItem ( "tразр=", 6, 6 ); MenuAddTimePlayerItem ( &Stage5Time );
  MenuAddLine ();
  MenuAddTextItem ( "Qэтапа=", 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &Stage5Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadCTCMenuStage5Over ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Iраз = ", 7, 7 );
  MenuAddFixedPointNumberPlayerItem ( &CTCStage5MaxCurrent, 4, 2, 100 );
  MenuAddTextItem (" А", 2, 4);
  MenuAddLine ();
  MenuAddTextItem ( "Uмин=", 5, 5 );
  MenuAddFixedPointNumberPlayerItem ( &CTCStage5MinVoltage, 4, 2, 100 ); MenuAddTextItem ( "<", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( " В", 2, 2 );
}

void LoadCTCMenuStage5 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadCTCMenuStage5Tune ();
    return;
   }
   else if ( tmpChargeControllerStage & MENU_MODE_WORKS ) {
      if ( Stage6Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadCTCMenuStage5Over ();
     LoadCTCMenuStage5Statistics ();
    }
      else if ( Stage5Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadCTCMenuStage5Executing ();
     LoadCTCMenuStage5Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage5Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage & MENU_MODE_PAUSED ) {
      if ( Stage6Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadCTCMenuStage5Over ();
     LoadCTCMenuStage5Statistics ();
    }
      else if ( Stage5Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап приостановл", 16, 16);
       LoadCTCMenuStage5Tune ();
     LoadCTCMenuStage5Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage5Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage & MENU_MODE_STOPPED ) {
      if ( Stage6Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadCTCMenuStage5Over ();
     LoadCTCMenuStage5Statistics ();
    }
      else if ( Stage5Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadCTCMenuStage5Over ();
     LoadCTCMenuStage5Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage5Over ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
  }
}

void LoadCTCMenuStage6Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "tпауз=", 6, 6 );
  MenuAddTimeEditorItem ( &Stage6MaxTime );
}

void LoadCTCMenuStage6Executing ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "tпауз=", 6, 6 ); MenuAddTimePlayerItem ( &Stage6Time );
  MenuAddLine ();
  MenuAddMarkerItem ( "    из ", 7, 7 ); MenuAddTimeEditorItem ( &Stage6MaxTime );
}

void LoadCTCMenuStage6Statistics ( void ) {
}

void LoadCTCMenuStage6Over ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "tпауз=", 6, 6 ); MenuAddTimePlayerItem ( &Stage6Time );
}

void LoadCTCMenuStage6 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadCTCMenuStage6Tune ();
    return;
   }
   else if ( tmpChargeControllerStage & MENU_MODE_WORKS ) {
      if ( Stage7Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadCTCMenuStage6Over ();
     LoadCTCMenuStage6Statistics ();
    }
      else if ( Stage6Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadCTCMenuStage6Executing ();
     LoadCTCMenuStage6Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage6Tune ();
    }
   }
   else if ( tmpChargeControllerStage & MENU_MODE_PAUSED ) {
      if ( Stage7Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadCTCMenuStage6Over ();
     LoadCTCMenuStage6Statistics ();
    }
      else if ( Stage6Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadCTCMenuStage6Tune ();
     LoadCTCMenuStage6Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage6Tune ();
    }
   }
   else if ( tmpChargeControllerStage & MENU_MODE_STOPPED ) {
      if ( Stage7Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadCTCMenuStage6Over ();
     LoadCTCMenuStage6Statistics ();
    }
      else if ( Stage6Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadCTCMenuStage6Over ();
     LoadCTCMenuStage6Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage6Over ();
    }
  }
}

extern int32_t CTCStage7MaxCurrent;
extern int32_t CTCStage7MaxVoltage;

void LoadCTCMenuStage7Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &CTCStage7MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 3 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &CTCStage7MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
}

void LoadCTCMenuStage7Executing ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 ); MenuAddFixedPointNumberEditorItem ( &CTCStage7MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 2, 100 );
  MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberEditorItem ( &CTCStage7MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
}

void LoadCTCMenuStage7Statistics ( void ) {
  if ( Stage7Reached ) {
    MenuAddLine ();
    MenuAddTextItem ( "Uзар=", 5, 5 );  // Выводим сообщение "Uз=24,63...26,77 В"
    MenuAddFixedPointNumberPlayerItem ( &Stage7Umin, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage7Umax, 4, 2, 100 );
    MenuAddTextItem ( chVolts, 2, 3 );
    MenuAddLine ();   // Выводим сообщение "tэтапа=24,63...26,77 В"
    MenuAddTextItem ( chStageTime, 7, 7 ); MenuAddTimePlayerItem ( &Stage7Time );
    MenuAddLine ();   // Выводим сообщение "Qэтапа=532,64 Ач"
    MenuAddTextItem ( chStageVolume, 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &Stage7Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
  }
  else {
    MenuAddLine ();
    MenuAddTextItem ( 0, 16, 16 );
  }
}

void LoadCTCMenuStage7Over ( void ) {
   MenuAddLine ();
   MenuAddTextItem ( "Iзар=", 5, 5 );
   MenuAddFixedPointNumberPlayerItem ( &CTCStage7MaxCurrent, 4, 2, 100 ); // Выводим сообщение "Iзар1=24,64 А" без возможности редактирования значения
   MenuAddTextItem ( chAmpers, 2, 3 );
   MenuAddLine ();
   MenuAddTextItem ( "Uмакс=", 6, 6 );
   MenuAddFixedPointNumberPlayerItem ( &CTCStage7MaxVoltage, 4, 2, 100 ); // Выводим сообщение "Umax1=24,64 В" без возможности редактирования значения
   MenuAddTextItem ( chVolts, 2, 2 );
}

void LoadCTCMenuStage7 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadCTCMenuStage7Tune ();
    return;
   }
   else if ( tmpChargeControllerStage & MENU_MODE_WORKS ) {
      if ( Stage8Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadCTCMenuStage7Over ();
     LoadCTCMenuStage7Statistics ();
    }
      else if (Stage7Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadCTCMenuStage7Executing ();
     LoadCTCMenuStage7Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage7Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage & MENU_MODE_PAUSED ) {
      if ( Stage8Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 16, 16);
     LoadCTCMenuStage7Over ();
     LoadCTCMenuStage7Statistics ();
    }
      else if (Stage7Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadCTCMenuStage7Tune ();
     LoadCTCMenuStage7Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage7Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage & MENU_MODE_STOPPED ) {
      if ( Stage8Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadCTCMenuStage7Over ();
     LoadCTCMenuStage7Statistics ();
    }
      else if (Stage7Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadCTCMenuStage7Over ();
     LoadCTCMenuStage7Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage7Over ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
}

extern int32_t CTCStage8MaxCurrent;
extern int32_t CTCStage8MaxVoltage;

void LoadCTCMenuStage8Tune ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberEditorItem ( &CTCStage8MaxCurrent, 4, 2, 100, 0, 100, 9000 );
  MenuAddTextItem ( chAmpers, 2, 3 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberEditorItem ( &CTCStage8MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
}

void LoadCTCMenuStage8Executing ( void ) {
  MenuAddLine ();
  MenuAddMarkerItem ( "Iзар=", 5, 5 ); MenuAddFixedPointNumberEditorItem ( &CTCStage8MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 2, 100 );
  MenuAddTextItem ( chAmpers, 2, 2 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмакс=", 6, 6 ); // Выводим сообщение "Uм1=__,__>26,57 В"
  MenuAddFixedPointNumberEditorItem ( &CTCStage8MaxVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
  MenuAddTextItem ( "В", 1, 1 );
}

void LoadCTCMenuStage8Statistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "Uзар=", 5, 5 );  // Выводим сообщение "Uз=24,63...26,77 В"
  MenuAddFixedPointNumberPlayerItem ( &Stage8Umin, 4, 2, 100 ); MenuAddTextItem ( "…", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Stage8Umax, 4, 2, 100 );
  MenuAddTextItem ( chVolts, 2, 3 );
  MenuAddLine ();   // Выводим сообщение "tэтапа=24,63...26,77 В"
  MenuAddTextItem ( chStageTime, 7, 7 ); MenuAddTimePlayerItem ( &Stage8Time );
  MenuAddLine ();   // Выводим сообщение "Qэтапа=532,64 Ач"
  MenuAddTextItem ( chStageVolume, 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &Stage8Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadCTCMenuStage8Over ( void ) {
   MenuAddLine ();
   MenuAddTextItem ( "Iзар=", 5, 5 );
   MenuAddFixedPointNumberPlayerItem ( &CTCStage8MaxCurrent, 4, 2, 100 ); // Выводим сообщение "Iзар1=24,64 А" без возможности редактирования значения
   MenuAddTextItem ( chAmpers, 2, 3 );
   MenuAddLine ();
   MenuAddTextItem ( chUmax2, 9, 9 );
   MenuAddFixedPointNumberPlayerItem ( &CTCStage8MaxVoltage, 4, 2, 100 ); // Выводим сообщение "Umax1=24,64 В" без возможности редактирования значения
   MenuAddTextItem ( chVolts, 2, 2 );
}

void LoadCTCMenuStage8 ( void ) {
   int32_t tmpChargeControllerStage;
   tmpChargeControllerStage = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
   if ( tmpChargeControllerStage == 0 ) {
      LoadCTCMenuStage8Tune ();
    return;
   }
   else if ( tmpChargeControllerStage & MENU_MODE_WORKS ) {
      if ( Stage9Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadCTCMenuStage8Over ();
     LoadCTCMenuStage8Statistics ();
    }
      else if (Stage8Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadCTCMenuStage8Executing ();
     LoadCTCMenuStage8Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage8Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage & MENU_MODE_PAUSED ) {
      if ( Stage9Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 16, 16);
     LoadCTCMenuStage8Over ();
     LoadCTCMenuStage8Statistics ();
    }
      else if (Stage8Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап выполняется", 16, 16);
       LoadCTCMenuStage8Tune ();
     LoadCTCMenuStage8Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage8Tune ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
   else if ( tmpChargeControllerStage & MENU_MODE_STOPPED ) {
      if ( Stage9Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап завершен", 13, 16);
     LoadCTCMenuStage8Over ();
     LoadCTCMenuStage8Statistics ();
    }
      else if (Stage8Reached ) {
       MenuAddLine ();
       MenuAddTextItem ("этап не завершен", 16, 16);
       LoadCTCMenuStage8Over ();
     LoadCTCMenuStage8Statistics ();
    }
    else {
       MenuAddLine ();
     MenuAddTextItem ("этап не достигн.", 16, 16);
       LoadCTCMenuStage8Over ();
       MenuAddLine ();
     MenuAddTextItem ("..нет статистики", 16, 16);
    }
   }
}

void LoadCTCMenuStatistics ( void ) {
  MenuAddLine ();
  MenuAddTextItem ( "tобщ = ", 7, 7 ); MenuAddTimePlayerItem ( &TotalTime );
  MenuAddLine ();
  MenuAddTextItem ( "Qобщ = ", 7, 7 ); MenuAddFixedPointNumberPlayerItem ( &TotalVolume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadCTCMenu ( void ) {
  ClearMenu ();
  MenuIndex = CTC_MENU;
  MenuAddLine ();
  MenuAddTextItem ("******КТЦ*******", 16, 16);
  tmpSubsystemState = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
  if ( tmpSubsystemState == 0 ) {
    MenuOnLeftButtonPress = LoadDifficultModesMenu;
    MenuAddLine ();
    MenuAddTextItem ( "Настройка режима", 16, 16 );
  }
  else if ( tmpSubsystemState == MENU_MODE_WORKS ) {
    MenuOnLeftButtonPress = LoadMainMenu;
      // Одна из ступеней режима заряда постоянным током
    if ( Stage9Reached ) { // Если достигнута последняя ступень, то выводим соответствующее сообщение
      MenuAddLine ();
      MenuAddTextItem ("Идёт завершение", 15, 16 );
      MenuAddLine ();
      MenuAddTextItem ("заряда…", 7, 16 );
    }
    else if (Stage8Reached ) { // Если последняя ступень не достигнута, но достигнута вторая ступень, то выводим соответствующее сообщение
      MenuAddLine ();
      MenuAddTextItem ( chChargeIsGoing, 16, 16 );
      MenuAddLine ();
      MenuAddTextItem ("второй ступени…", 15, 16 );
    }
    else if ( Stage7Reached ) { // Если вторая ступень не достигнута, но достигнута первая ступень, то выводим соответствующее сообщение
      MenuAddLine ();
      MenuAddTextItem ( chChargeIsGoing, 16, 16 );
      MenuAddLine ();
      MenuAddTextItem ("первой ступени…", 15, 16 );
    }
    else if ( Stage6Reached ) {
      MenuAddLine ();
      MenuAddTextItem ( "Пауза перед", 11, 16 );
      MenuAddLine ();
      MenuAddTextItem ("зарядом…", 8, 8 );
    }
    else if ( Stage5Reached ) {
      MenuAddLine ();
      MenuAddTextItem ( "Идёт разряд…", 12, 12 );
    }
    else if ( Stage4Reached ) {
      MenuAddLine ();
      MenuAddTextItem ( "Пауза перед", 11, 16 );
      MenuAddLine ();
      MenuAddTextItem ("разрядом…", 9, 9 );
    }
    else if ( Stage3Reached ) { // Если достигнута последняя ступень, то выводим соответствующее сообщение
      MenuAddLine ();
      MenuAddTextItem ("Идёт завершение", 15, 16 );
      MenuAddLine ();
      MenuAddTextItem ("заряда…", 7, 16 );
    }
    else if (Stage2Reached ) { // Если последняя ступень не достигнута, но достигнута вторая ступень, то выводим соответствующее сообщение
      MenuAddLine ();
      MenuAddTextItem ( chChargeIsGoing, 16, 16 );
      MenuAddLine ();
      MenuAddTextItem ("второй ступени…", 15, 16 );
    }
    else if ( Stage1Reached ) { // Если вторая ступень не достигнута, но достигнута первая ступень, то выводим соответствующее сообщение
      MenuAddLine ();
      MenuAddTextItem ( chChargeIsGoing, 16, 16 );
      MenuAddLine ();
      MenuAddTextItem ("первой ступени…", 15, 16 );
    }
  }
  else if ( tmpSubsystemState == MENU_MODE_PAUSED )    {
    MenuOnLeftButtonPress = LoadMainMenu;
    MenuAddLine ();
    MenuAddTextItem ("Приостановлен по", 16, 16 );
    MenuAddLine ();
    if (ModeStopOrPauseReason == OPERATOR ) MenuAddTextItem ("решен. оператора", 16, 16);
    else if ( ModeStopOrPauseReason == OVERHEATING ) MenuAddTextItem ("перегреву модуля", 16, 16);
    else if ( ModeStopOrPauseReason == FAULT ) MenuAddTextItem ("аварии", 6, 16 );
  }
  else if ( tmpSubsystemState == MENU_MODE_STOPPED ) {
    MenuOnLeftButtonPress = LoadMainMenu;
    MenuAddLine ();
    MenuAddTextItem ("Завершен по", 11, 16 );
    MenuAddLine ();
    if (ModeStopOrPauseReason == OPERATOR ) MenuAddTextItem ("решен. оператора", 16, 16);
    else if ( ModeStopOrPauseReason == TIME ) MenuAddTextItem ("уставке времени", 15, 16);
    else if ( ModeStopOrPauseReason == FAULT ) MenuAddTextItem ("аварии", 6, 16 );
    else if ( ModeStopOrPauseReason == OVER ) MenuAddTextItem ("готовности", 10, 16 );
    
    MenuAddLine ();
    MenuAddTextItem ("5. Управление", 13, 16 );
  }
  MenuAddLine ();
  MenuAddTextItem ( "1. Зар 10-час I", 15, 16 );
  LoadCTCMenuStage1 ();
  MenuAddLine ();
  MenuAddTextItem ( "2. Зар 20-час I", 15, 16 );
  LoadCTCMenuStage2 ();
  MenuAddLine ();
  MenuAddTextItem ( "3. Пауза", 8, 8 );
  LoadCTCMenuStage4 ();
  MenuAddLine ();
  MenuAddTextItem ( "4. Разр 10-час I", 16, 16 );
  LoadCTCMenuStage5 ();
  MenuAddLine ();
  MenuAddTextItem ( "5. Пауза", 8, 8 );
  LoadCTCMenuStage6 ();
  MenuAddLine ();
  MenuAddTextItem ( "6. Зар 10-час I", 16, 16 );
  LoadCTCMenuStage7 ();
  MenuAddLine ();
  MenuAddTextItem ( "7. Зар 20-час I", 16, 16 );
  LoadCTCMenuStage8 ();

  if ( tmpSubsystemState == 0 ) {
    MenuAddLine ();
    MenuAddTextItem ("8. Управление", 14, 16 );
  }
  else {
    MenuAddLine ();
    MenuAddTextItem ("8. Статистика", 14, 16 );
    LoadCTCMenuStatistics ();
    MenuAddLine ();
    MenuAddTextItem ("9. Управление", 14, 16 );
  }
  
  LoadMenuCommands ();
  MenuAddLine ();
  MenuAddTextItem (chMenuEnd, 16, 16);
}

extern int32_t SimpleCTCStage1MaxCurrent;
extern int32_t SimpleCTCStage1MaxVoltage;
extern int32_t SimpleCTCStage2MaxCurrent;
extern int32_t SimpleCTCStage2MaxVoltage;
extern int32_t SimpleCTCStage2MinVoltageIntervalPerHour;
#define SimpleCTCStage3MaxTime (*((uint64_t*)(&(References[SIMPLE_CTC_PAUSE_TIME_HW]))))
// extern int64_t SimpleCTCStage3MaxTime;
extern int32_t SimpleCTCStage4MaxCurrent;
extern int32_t SimpleCTCStage4MinVoltage;
#define SimpleCTCStage5MaxTime (*((uint64_t*)(&(References[SIMPLE_CTC_PAUSE_TIME_HW]))))
// extern int64_t SimpleCTCStage5MaxTime;

int32_t SimpleCTCBatteryVoltageIndex = 0;
int32_t SimpleCTCBatteryMinVoltageIndex = 0;
int32_t SimpleCTCBatteryVoltageComboBox;
int32_t SimpleCTCBatteryMinVoltageComboBox;

void OnSimpleCTCBatteryVoltageSelect ( int32_t NewSimpleCTCBatteryVoltageIndex ) {
  if ( NewSimpleCTCBatteryVoltageIndex == 0 ) {
    SimpleCTCBatteryVoltageIndex = 0;
    ClearComboBoxItemElements ( SimpleCTCBatteryMinVoltageComboBox );
    MenuAddComboBoxItemElement ( SimpleCTCBatteryMinVoltageComboBox, "21 В", 4 );
    MenuAddComboBoxItemElement ( SimpleCTCBatteryMinVoltageComboBox, "20,4 В", 4 );
    ComboBoxSetElementIndex ( SimpleCTCBatteryMinVoltageComboBox, SimpleCTCBatteryMinVoltageIndex );
    SimpleCTCStage1MaxVoltage = References[SIMPLE_CONST_I_CHARGE_24V_1ST_STAGE_MAX_VOLTAGE];
    SimpleCTCStage2MaxVoltage = References[SIMPLE_CONST_I_CHARGE_24V_2ND_STAGE_MAX_VOLTAGE];
    SimpleCTCStage2MinVoltageIntervalPerHour = References[SIMPLE_CONST_I_CHARGE_24V_2ND_STAGE_MAX_DELTA_VOLTAGE];
    pMaxFaultBatteryVoltage = &(References[MAX_24V_BATTERY_VOLTAGE]);
    pMinFaultBatteryVoltage = &(References[MIN_24V_BATTERY_VOLTAGE]);
  }
  else if ( NewSimpleCTCBatteryVoltageIndex == 1 ) {
    SimpleCTCBatteryVoltageIndex = 1;
    ClearComboBoxItemElements ( SimpleCTCBatteryMinVoltageComboBox );
    MenuAddComboBoxItemElement ( SimpleCTCBatteryMinVoltageComboBox, "10,5 В", 4 );
    MenuAddComboBoxItemElement ( SimpleCTCBatteryMinVoltageComboBox, "10,2 В", 4 );
    ComboBoxSetElementIndex ( SimpleCTCBatteryMinVoltageComboBox, SimpleCTCBatteryMinVoltageIndex );
    SimpleCTCStage1MaxVoltage = References[SIMPLE_CONST_I_CHARGE_12V_1ST_STAGE_MAX_VOLTAGE];
    SimpleCTCStage2MaxVoltage = References[SIMPLE_CONST_I_CHARGE_12V_2ND_STAGE_MAX_VOLTAGE];
    SimpleCTCStage2MinVoltageIntervalPerHour = References[SIMPLE_CONST_I_CHARGE_12V_2ND_STAGE_MAX_DELTA_VOLTAGE];
    pMaxFaultBatteryVoltage = &(References[MAX_12V_BATTERY_VOLTAGE]);
    pMinFaultBatteryVoltage = &(References[MIN_12V_BATTERY_VOLTAGE]);
   }
}

void OnSimpleCTCBatteryMinVoltageSelect ( int32_t NewSimpleCTCBatteryMinVoltageIndex ) {
   if ( NewSimpleCTCBatteryMinVoltageIndex == 0 ) {
     SimpleCTCBatteryMinVoltageIndex = 0;
     if ( SimpleCTCBatteryVoltageIndex == 0 ) {
       SimpleCTCStage4MinVoltage = References[SIMPLE_CONTROL_DISCHARGE_21V_MIN_VOLTAGE];
     }
     else if ( SimpleCTCBatteryVoltageIndex == 1 ) {
       SimpleCTCStage4MinVoltage = References[SIMPLE_CONTROL_DISCHARGE_10_5V_MIN_VOLTAGE];
     }
   }
   else if ( NewSimpleCTCBatteryMinVoltageIndex == 1 ) {
     SimpleCTCBatteryMinVoltageIndex = 1;
     if ( SimpleCTCBatteryVoltageIndex == 0 ) {
       SimpleCTCStage4MinVoltage = References[SIMPLE_CONTROL_DISCHARGE_20_4V_MIN_VOLTAGE];
     }
     else if ( SimpleCTCBatteryVoltageIndex == 1 ) {
       SimpleCTCStage4MinVoltage = References[SIMPLE_CONTROL_DISCHARGE_10_2V_MIN_VOLTAGE];
     }
   }
}

void LoadSimpleCTCTuneMenu () {
  MenuAddLine ();
  MenuAddMarkerItem ( "Uакб=", 5, 5 );
  SimpleCTCBatteryVoltageComboBox = MenuAddComboBoxItem ( 2, OnSimpleCTCBatteryVoltageSelect );
  MenuAddComboBoxItemElement ( SimpleCTCBatteryVoltageComboBox, "24 В", 4 );
  MenuAddComboBoxItemElement ( SimpleCTCBatteryVoltageComboBox, "12 В", 4 );
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар1=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem (" А", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар2=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem (" А", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмин=", 5, 5 );
  SimpleCTCBatteryMinVoltageComboBox = MenuAddComboBoxItem ( 2, OnSimpleCTCBatteryMinVoltageSelect );
  ComboBoxSetElementIndex ( SimpleCTCBatteryVoltageComboBox, SimpleCTCBatteryVoltageIndex );
  ComboBoxSetElementIndex ( SimpleCTCBatteryMinVoltageComboBox, SimpleCTCBatteryMinVoltageIndex );
  MenuAddLine ();
  MenuAddMarkerItem ("Iразр=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage4MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem (" А", 2, 2);
}

void LoadSimpleCTCStage1ExecutingMenu () {
  MenuAddLine ();
  MenuAddTextItem ("Заряд I 1 ступ..", 16, 16 );
  MenuAddLine ();
  MenuAddTextItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberPlayerItem ( &SimpleCTCStage1MaxVoltage, 4, 100, 3 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 100, 3 ); MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар1=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("~", 1, 1); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 100, 3 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар2=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмин=", 5, 5 );
  SimpleCTCBatteryMinVoltageComboBox = MenuAddComboBoxItem ( 2, OnSimpleCTCBatteryMinVoltageSelect );
  OnSimpleCTCBatteryVoltageSelect ( SimpleCTCBatteryVoltageIndex );
  ComboBoxSetElementIndex ( SimpleCTCBatteryMinVoltageComboBox, SimpleCTCBatteryMinVoltageIndex );
  MenuAddLine ();
  MenuAddMarkerItem ("Iразр=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage4MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddTextItem ( "tзар=", 5, 5 ); MenuAddTimePlayerItem ( &Stage1Time );
  MenuAddLine ();
  MenuAddTextItem ( "Qзар=", 5, 5 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleCTCStage2ExecutingMenu () {
  MenuAddLine ();
  MenuAddTextItem ("Заряд I 2 ступ..", 16, 16 );
  MenuAddLine ();
  MenuAddTextItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberPlayerItem ( &SimpleCTCStage2MaxVoltage, 4, 100, 3 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 100, 3 ); MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар2=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("~", 1, 1); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 100, 3 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмин=", 5, 5 );
  SimpleCTCBatteryMinVoltageComboBox = MenuAddComboBoxItem ( 2, OnSimpleCTCBatteryMinVoltageSelect );
  OnSimpleCTCBatteryVoltageSelect ( SimpleCTCBatteryVoltageIndex );
  ComboBoxSetElementIndex ( SimpleCTCBatteryMinVoltageComboBox, SimpleCTCBatteryMinVoltageIndex );
  MenuAddLine ();
  MenuAddMarkerItem ("Iразр=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage4MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар1=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddTextItem ( "tзар=", 5, 5 ); MenuAddTimePlayerItem ( &Stage1Time );
  MenuAddLine ();
  MenuAddTextItem ( "Qзар=", 5, 5 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleCTCStage3ExecutingMenu () {
  MenuAddLine (); MenuAddTextItem ( "Пауза...", 8, 16 );
  MenuAddLine (); MenuAddTextItem ( "tпауз=", 6, 6 ); MenuAddTimePlayerItem ( &Stage3Time );
  MenuAddLine (); MenuAddTextItem ( "   из ", 6, 6 ); MenuAddTimePlayerItem ( &SimpleCTCStage3MaxTime );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмин=", 5, 5 );
  SimpleCTCBatteryMinVoltageComboBox = MenuAddComboBoxItem ( 2, OnSimpleCTCBatteryMinVoltageSelect );
  OnSimpleCTCBatteryVoltageSelect ( SimpleCTCBatteryVoltageIndex );
  ComboBoxSetElementIndex ( SimpleCTCBatteryMinVoltageComboBox, SimpleCTCBatteryMinVoltageIndex );
  MenuAddLine ();
  MenuAddMarkerItem ("Iразр=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage4MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem (" А", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар1=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem (" А", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар2=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem (" А", 2, 2);
}

void LoadSimpleCTCStage4ExecutingMenu () {
  MenuAddLine ();
  MenuAddTextItem ("Идёт разряд...", 14, 16 );
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмин=", 5, 5 );
  SimpleCTCBatteryMinVoltageComboBox = MenuAddComboBoxItem ( 2, OnSimpleCTCBatteryMinVoltageSelect );
  OnSimpleCTCBatteryVoltageSelect ( SimpleCTCBatteryVoltageIndex );
  ComboBoxSetElementIndex ( SimpleCTCBatteryMinVoltageComboBox, SimpleCTCBatteryMinVoltageIndex );
  MenuAddTextItem ( "<", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 100, 3 ); MenuAddTextItem (" В", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ("Iразр=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage4MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("~", 1, 1); MenuAddFixedPointNumberPlayerItem ( &(Values[DISCHARGE_CURRENT]), 4, 100, 3 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар1=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар2=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddTextItem ( "tразр=", 6, 6 ); MenuAddTimePlayerItem ( &Stage4Time );
  MenuAddLine ();
  MenuAddTextItem ( "Qразр=", 6, 6 ); MenuAddFixedPointNumberPlayerItem ( &Stage4Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleCTCStage5ExecutingMenu () {
  MenuAddLine (); MenuAddTextItem ( "Пауза...", 8, 16 );
  MenuAddLine (); MenuAddTextItem ( "tпауз=", 6, 6 ); MenuAddTimePlayerItem ( &Stage5Time );
  MenuAddLine (); MenuAddTextItem ( "   из ", 6, 6 ); MenuAddTimePlayerItem ( &SimpleCTCStage5MaxTime );
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар1=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem (" А", 2, 2);
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар2=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem (" А", 2, 2);
  MenuAddLine ();
  MenuAddTextItem ( "tразр=", 6, 6 ); MenuAddTimePlayerItem ( &Stage4Time );
  MenuAddLine ();
  MenuAddTextItem ( "Qразр=", 6, 6 ); MenuAddFixedPointNumberPlayerItem ( &Stage4Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleCTCStage6ExecutingMenu () {
  MenuAddLine ();
  MenuAddTextItem ("Заряд I 1 ступ..", 16, 16 );
  MenuAddLine ();
  MenuAddTextItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberPlayerItem ( &SimpleCTCStage1MaxVoltage, 4, 100, 3 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 100, 3 ); MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар1=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("~", 1, 1); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 100, 3 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар2=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddTextItem ( "tзар=", 5, 5 ); MenuAddTimePlayerItem ( &Stage6Time );
  MenuAddLine ();
  MenuAddTextItem ( "Qзар=", 5, 5 ); MenuAddFixedPointNumberPlayerItem ( &Stage6Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
  MenuAddLine ();
  MenuAddTextItem ( "tразр=", 6, 6 ); MenuAddTimePlayerItem ( &Stage4Time );
  MenuAddLine ();
  MenuAddTextItem ( "Qразр=", 6, 6 ); MenuAddFixedPointNumberPlayerItem ( &Stage4Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleCTCStage7ExecutingMenu () {
  MenuAddLine ();
  MenuAddTextItem ("Заряд I 2 ступ..", 16, 16 );
  MenuAddLine ();
  MenuAddTextItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberPlayerItem ( &SimpleCTCStage2MaxVoltage, 4, 100, 3 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 100, 3 ); MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар2=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("~", 1, 1); MenuAddFixedPointNumberPlayerItem ( &(Values[CHARGE_CURRENT]), 4, 100, 3 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddTextItem ( "tзар=", 5, 5 ); MenuAddTimePlayerItem ( &Stage6Time );
  MenuAddLine ();
  MenuAddTextItem ( "Qзар=", 5, 5 ); MenuAddFixedPointNumberPlayerItem ( &Stage6Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
  MenuAddLine ();
  MenuAddTextItem ( "tразр=", 6, 6 ); MenuAddTimePlayerItem ( &Stage4Time );
  MenuAddLine ();
  MenuAddTextItem ( "Qразр=", 6, 6 ); MenuAddFixedPointNumberPlayerItem ( &Stage4Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleCTCStage1PausedMenu () {
  MenuAddLine ();
  MenuAddTextItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberPlayerItem ( &SimpleCTCStage1MaxVoltage, 4, 100, 3 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 100, 3 ); MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар1=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар2=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмин=", 5, 5 );
  SimpleCTCBatteryMinVoltageComboBox = MenuAddComboBoxItem ( 2, OnSimpleCTCBatteryMinVoltageSelect );
  OnSimpleCTCBatteryVoltageSelect ( SimpleCTCBatteryVoltageIndex );
  ComboBoxSetElementIndex ( SimpleCTCBatteryMinVoltageComboBox, SimpleCTCBatteryMinVoltageIndex );
  MenuAddLine ();
  MenuAddMarkerItem ("Iразр=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage4MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddTextItem ( "tзар=", 5, 5 ); MenuAddTimePlayerItem ( &Stage1Time );
  MenuAddLine ();
  MenuAddTextItem ( "Qзар=", 5, 5 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleCTCStage2PausedMenu () {
  MenuAddLine ();
  MenuAddTextItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberPlayerItem ( &SimpleCTCStage1MaxVoltage, 4, 100, 3 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 100, 3 ); MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар2=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар1=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмин=", 5, 5 );
  SimpleCTCBatteryMinVoltageComboBox = MenuAddComboBoxItem ( 2, OnSimpleCTCBatteryMinVoltageSelect );
  OnSimpleCTCBatteryVoltageSelect ( SimpleCTCBatteryVoltageIndex );
  ComboBoxSetElementIndex ( SimpleCTCBatteryMinVoltageComboBox, SimpleCTCBatteryMinVoltageIndex );
  MenuAddLine ();
  MenuAddMarkerItem ("Iразр=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage4MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddTextItem ( "tзар=", 5, 5 ); MenuAddTimePlayerItem ( &Stage1Time );
  MenuAddLine ();
  MenuAddTextItem ( "Qзар=", 5, 5 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleCTCStage3PausedMenu () {
}

void LoadSimpleCTCStage4PausedMenu () {
  MenuAddLine ();
  MenuAddMarkerItem ( "Uмин=", 5, 5 );
  SimpleCTCBatteryMinVoltageComboBox = MenuAddComboBoxItem ( 2, OnSimpleCTCBatteryMinVoltageSelect );
  OnSimpleCTCBatteryVoltageSelect ( SimpleCTCBatteryVoltageIndex );
  ComboBoxSetElementIndex ( SimpleCTCBatteryMinVoltageComboBox, SimpleCTCBatteryMinVoltageIndex );
  MenuAddLine ();
  MenuAddMarkerItem ("Iразр=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage4MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар1=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар2=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddTextItem ( "tразр=", 6, 6 ); MenuAddTimePlayerItem ( &Stage4Time );
  MenuAddLine ();
  MenuAddTextItem ( "Qразр=", 6, 6 ); MenuAddFixedPointNumberPlayerItem ( &Stage4Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleCTCStage5PausedMenu () {
}

void LoadSimpleCTCStage6PausedMenu () {
  MenuAddLine ();
  MenuAddTextItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberPlayerItem ( &SimpleCTCStage1MaxVoltage, 4, 100, 3 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 100, 3 ); MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар1=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар2=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddTextItem ( "tзар=", 5, 5 ); MenuAddTimePlayerItem ( &Stage6Time );
  MenuAddLine ();
  MenuAddTextItem ( "Qзар=", 5, 5 ); MenuAddFixedPointNumberPlayerItem ( &Stage6Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
  MenuAddLine ();
  MenuAddTextItem ( "tразр=", 6, 6 ); MenuAddTimePlayerItem ( &Stage4Time );
  MenuAddLine ();
  MenuAddTextItem ( "Qразр=", 6, 6 ); MenuAddFixedPointNumberPlayerItem ( &Stage4Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleCTCStage7PausedMenu () {
  MenuAddLine ();
  MenuAddTextItem ( "Uмакс=", 6, 6 );
  MenuAddFixedPointNumberPlayerItem ( &SimpleCTCStage2MaxVoltage, 4, 100, 3 ); MenuAddTextItem ( ">", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 100, 3 ); MenuAddTextItem ( "В", 1, 1 );
  MenuAddLine ();
  MenuAddMarkerItem ("Iзар2=", 6, 6);
  MenuAddFixedPointNumberEditorItem ( &SimpleCTCStage2MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ("А", 1, 1);
  MenuAddLine ();
  MenuAddTextItem ( "tзар=", 5, 5 ); MenuAddTimePlayerItem ( &Stage6Time );
  MenuAddLine ();
  MenuAddTextItem ( "Qзар=", 5, 5 ); MenuAddFixedPointNumberPlayerItem ( &Stage6Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
  MenuAddLine ();
  MenuAddTextItem ( "tразр=", 6, 6 ); MenuAddTimePlayerItem ( &Stage4Time );
  MenuAddLine ();
  MenuAddTextItem ( "Qразр=", 6, 6 ); MenuAddFixedPointNumberPlayerItem ( &Stage4Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

void LoadSimpleCTCOverMenu () {
  MenuAddLine ();
  MenuAddTextItem ("Завершен по", 11, 16 );
  MenuAddLine ();
  if (ModeStopOrPauseReason == OPERATOR ) MenuAddTextItem ("решен. оператора", 16, 16);
  else if ( ModeStopOrPauseReason == OVER ) MenuAddTextItem ("готовности", 10, 16 );
  else if ( ModeStopOrPauseReason == OVERHEATING ) MenuAddTextItem ("перегреву модуля", 16, 16);
  else if ( ModeStopOrPauseReason == FAULT ) MenuAddTextItem ("аварии", 6, 16 );
  MenuAddLine ();
  MenuAddTextItem ( "Uакб=", 5, 5 );
  if ( SimpleCTCBatteryVoltageIndex == 0 ) {
    MenuAddTextItem ( "24 В", 4, 4 );
  MenuAddLine ();
  MenuAddTextItem ("Uмин=", 5, 5);
  if ( SimpleCTCBatteryMinVoltageIndex == 0 ) {
    MenuAddTextItem ( "21 В", 4, 4);
  }
  else if ( SimpleCTCBatteryMinVoltageIndex == 1 ) {
    MenuAddTextItem ( "20,4 В", 6, 6);
  }
  }
  else if ( SimpleCTCBatteryVoltageIndex == 1 ) {
    MenuAddTextItem ( "12 В", 4, 4 );
  MenuAddLine ();
  MenuAddTextItem ("Uмин=", 5, 5);
  if ( SimpleCTCBatteryMinVoltageIndex == 0 ) {
    MenuAddTextItem ( "10,5 В", 6, 6);
  }
  else if ( SimpleCTCBatteryMinVoltageIndex == 1 ) {
    MenuAddTextItem ( "10,2 В", 6, 6);
  }
  }
  MenuAddLine ();
  MenuAddTextItem ("Iзар1=", 6, 6);
  MenuAddFixedPointNumberPlayerItem ( &SimpleCTCStage1MaxCurrent, 4, 100, 2 ); MenuAddTextItem (" А", 2, 2);
  MenuAddLine ();
  MenuAddTextItem ("Iзар2=", 6, 6);
  MenuAddFixedPointNumberPlayerItem ( &SimpleCTCStage2MaxCurrent, 4, 100, 2 ); MenuAddTextItem (" А", 2, 2);
  MenuAddLine ();
  MenuAddTextItem ("Iразр=", 6, 6);
  MenuAddFixedPointNumberPlayerItem ( &SimpleCTCStage4MaxCurrent, 4, 100, 2 ); MenuAddTextItem (" А", 2, 2);
  MenuAddLine ();
  MenuAddTextItem ( "tразр=", 6, 6 ); MenuAddTimePlayerItem ( &Stage4Time );
  MenuAddLine ();
  MenuAddTextItem ( "Qразр=", 6, 6 ); MenuAddFixedPointNumberPlayerItem ( &Stage4Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
  MenuAddLine ();
  MenuAddTextItem ( "tзар=", 5, 5 ); MenuAddTimePlayerItem ( &Stage6Time );
  MenuAddLine ();
  MenuAddTextItem ( "Qзар=", 5, 5 ); MenuAddFixedPointNumberPlayerItem ( &Stage6Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
}

int32_t tmpModeStage;

void LoadSimpleCTCMenu () {
  ClearMenu ();
  MenuIndex = SIMPLE_CTC_MENU;
  MenuAddLine ();
  MenuAddTextItem ("******КТЦ*******", 16, 16);
  MenuOnLeftButtonPress = LoadMainMenu;
  tmpSubsystemState = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
  tmpModeStage = GetSubsystemState ( UZRU10A ) & STAGE_MASK;
  if ( tmpSubsystemState == 0 ) {
    LoadSimpleCTCTuneMenu ();
  }
  else if ( tmpSubsystemState == MENU_MODE_WORKS ) {
    switch ( tmpModeStage ) {
      case MODE_STAGE1: LoadSimpleCTCStage1ExecutingMenu (); break;
      case MODE_STAGE2: LoadSimpleCTCStage2ExecutingMenu (); break;
      case MODE_STAGE3: LoadSimpleCTCStage3ExecutingMenu (); break;
      case MODE_STAGE4: LoadSimpleCTCStage4ExecutingMenu (); break;
      case MODE_STAGE5: LoadSimpleCTCStage5ExecutingMenu (); break;
      case MODE_STAGE6: LoadSimpleCTCStage6ExecutingMenu (); break;
      case MODE_STAGE7: LoadSimpleCTCStage7ExecutingMenu (); break;
    }
  }
  else if ( tmpSubsystemState == MENU_MODE_PAUSED ) {
  
    MenuAddLine ();
    MenuAddTextItem ("Приостановлен по", 16, 16 );
    MenuAddLine ();
    if (ModeStopOrPauseReason == OPERATOR ) MenuAddTextItem ("решен. оператора", 16, 16);
    else if ( ModeStopOrPauseReason == OVER ) MenuAddTextItem ("готовности", 10, 16 );
    else if ( ModeStopOrPauseReason == OVERHEATING ) MenuAddTextItem ("перегреву модуля", 16, 16);
    else if ( ModeStopOrPauseReason == FAULT ) MenuAddTextItem ("аварии", 6, 16 );
    switch ( tmpModeStage ) {
      case MODE_STAGE1: LoadSimpleCTCStage1PausedMenu (); break;
      case MODE_STAGE2: LoadSimpleCTCStage2PausedMenu (); break;
      case MODE_STAGE3: LoadSimpleCTCStage3PausedMenu (); break;
      case MODE_STAGE4: LoadSimpleCTCStage4PausedMenu (); break;
      case MODE_STAGE5: LoadSimpleCTCStage5PausedMenu (); break;
      case MODE_STAGE6: LoadSimpleCTCStage6PausedMenu (); break;
      case MODE_STAGE7: LoadSimpleCTCStage7PausedMenu (); break;
    }
  }
  else if ( tmpSubsystemState == MENU_MODE_STOPPED ) {
    LoadSimpleCTCOverMenu ();
  }
  LoadMenuCommands ();
  MenuAddLine ();
  MenuAddTextItem ( "---конец-меню---", 16, 16 );
}

extern int32_t SimpleControlDischargeStage1MaxCurrent;
extern int32_t SimpleControlDischargeStage1MinVoltage;

int32_t SimpleControlDischargeBatteryVoltageComboBox;
int32_t SimpleControlDischargeBatteryVoltageIndex;
int32_t SimpleControlDischargeBatteryMinVoltageComboBox;
int32_t SimpleControlDischargeBatteryMinVoltageIndex;

void SimpleControlDischargeMenuOnBatteryVoltageSelect ( int32_t BatteryVoltageIndex ) {
  ClearComboBoxItemElements ( SimpleControlDischargeBatteryMinVoltageComboBox );
  if ( BatteryVoltageIndex == 0 ) {
    SimpleControlDischargeBatteryVoltageIndex = 0;
    MenuAddComboBoxItemElement ( SimpleControlDischargeBatteryMinVoltageComboBox, "21 В", 4 );
    MenuAddComboBoxItemElement ( SimpleControlDischargeBatteryMinVoltageComboBox, "20,4 В", 6 );
    ComboBoxSetElementIndex ( SimpleControlDischargeBatteryMinVoltageComboBox, SimpleControlDischargeBatteryMinVoltageIndex );
		pMaxFaultBatteryVoltage = &(References[MAX_24V_BATTERY_VOLTAGE]);
    pMinFaultBatteryVoltage = &(References[MIN_24V_BATTERY_VOLTAGE]);
  }
  else if ( BatteryVoltageIndex == 1 ) {
    SimpleControlDischargeBatteryVoltageIndex = 1;
    MenuAddComboBoxItemElement ( SimpleControlDischargeBatteryMinVoltageComboBox, "10,5 В", 6 );
    MenuAddComboBoxItemElement ( SimpleControlDischargeBatteryMinVoltageComboBox, "10,2 В", 6 );
    ComboBoxSetElementIndex ( SimpleControlDischargeBatteryMinVoltageComboBox, SimpleControlDischargeBatteryMinVoltageIndex );
    pMaxFaultBatteryVoltage = &(References[MAX_12V_BATTERY_VOLTAGE]);
    pMinFaultBatteryVoltage = &(References[MIN_12V_BATTERY_VOLTAGE]);
  }
}

void SimpleControlDischargeMenuOnBatteryMinVoltageSelect ( int32_t BatteryMinVoltageIndex ) {
  if ( BatteryMinVoltageIndex == 0 ) {
    SimpleControlDischargeBatteryMinVoltageIndex = 0;
    if ( SimpleControlDischargeBatteryVoltageIndex == 0 ) { SimpleControlDischargeStage1MinVoltage = References[SIMPLE_CONTROL_DISCHARGE_21V_MIN_VOLTAGE]; }
    else if ( SimpleControlDischargeBatteryVoltageIndex == 1 ) { SimpleControlDischargeStage1MinVoltage = References[SIMPLE_CONTROL_DISCHARGE_10_5V_MIN_VOLTAGE]; }
  }
  else if ( BatteryMinVoltageIndex == 1 ) {
    SimpleControlDischargeBatteryMinVoltageIndex = 1;
    if ( SimpleControlDischargeBatteryVoltageIndex == 0 ) { SimpleControlDischargeStage1MinVoltage = References[SIMPLE_CONTROL_DISCHARGE_20_4V_MIN_VOLTAGE]; }
    else if ( SimpleControlDischargeBatteryVoltageIndex == 1 ) { SimpleControlDischargeStage1MinVoltage = References[SIMPLE_CONTROL_DISCHARGE_10_2V_MIN_VOLTAGE]; }
  }
}

void LoadSimpleControlDischargeMenu ( void ) {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadMainMenu;
  MenuIndex = SIMPLE_CONTROL_DISCHARGE_MENU;
  MenuAddLine ();
  MenuAddTextItem ("**Контр. разряд*", 16, 16);
  tmpSubsystemState = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
  if ( tmpSubsystemState == 0 ) {
    MenuAddLine ();
    MenuAddTextItem ("Настройка режима", 16, 16 );
    MenuAddLine ();
    MenuAddMarkerItem ( "Uакб=", 5, 5 );
    SimpleControlDischargeBatteryVoltageComboBox = MenuAddComboBoxItem ( 2, SimpleControlDischargeMenuOnBatteryVoltageSelect );
    MenuAddComboBoxItemElement ( SimpleControlDischargeBatteryVoltageComboBox, "24 В", 4 );
    MenuAddComboBoxItemElement ( SimpleControlDischargeBatteryVoltageComboBox, "12 В", 4 );
    MenuAddLine ();
    MenuAddMarkerItem ( "Uмин=", 5, 5 );
    SimpleControlDischargeBatteryMinVoltageComboBox = MenuAddComboBoxItem ( 2, SimpleControlDischargeMenuOnBatteryMinVoltageSelect );
    ComboBoxSetElementIndex ( SimpleControlDischargeBatteryVoltageComboBox, SimpleControlDischargeBatteryVoltageIndex );
    ComboBoxSetElementIndex ( SimpleControlDischargeBatteryMinVoltageComboBox, SimpleControlDischargeBatteryMinVoltageIndex );
    MenuAddLine ();
    MenuAddMarkerItem ( "Iразр=", 6, 6 );
    MenuAddFixedPointNumberEditorItem ( &SimpleControlDischargeStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 );
    MenuAddTextItem ( chAmpers, 2, 2 );
    MenuAddLine ();
    MenuAddMarkerItem ( "Начать разряд.", 14, 16 );
    MenuAddVoidCommandItem ( StartMode, BTN_START_ONLY );
  }
  else if ( tmpSubsystemState == MENU_MODE_WORKS ) {
    MenuAddLine ();
    MenuAddTextItem ("Идёт разряд...", 14, 16 );
    MenuAddLine ();
    MenuAddMarkerItem ( "Uмин=", 5, 5 );
    SimpleControlDischargeBatteryMinVoltageComboBox = MenuAddComboBoxItem ( 2, SimpleControlDischargeMenuOnBatteryMinVoltageSelect );
  if ( SimpleControlDischargeBatteryVoltageIndex == 0 ) {
    MenuAddComboBoxItemElement ( SimpleControlDischargeBatteryMinVoltageComboBox, "21", 2 );
    MenuAddComboBoxItemElement ( SimpleControlDischargeBatteryMinVoltageComboBox, "20,4", 4 );
  }
  else {
    MenuAddComboBoxItemElement ( SimpleControlDischargeBatteryMinVoltageComboBox, "10,5", 4 );
    MenuAddComboBoxItemElement ( SimpleControlDischargeBatteryMinVoltageComboBox, "10,2", 4 );
  }
    ComboBoxSetElementIndex ( SimpleControlDischargeBatteryMinVoltageComboBox, SimpleControlDischargeBatteryMinVoltageIndex );
    MenuAddTextItem ( "<", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[BATTERY_VOLTAGE], 4, 2, 100 ); MenuAddTextItem ( " В", 2, 2 );
    MenuAddLine ();
    MenuAddMarkerItem ( "Iразр=", 6, 6 );
    MenuAddFixedPointNumberEditorItem ( &SimpleControlDischargeStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem ( "~", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[DISCHARGE_CURRENT], 4, 2, 100 );
    MenuAddTextItem ( "А", 1, 1 );
    MenuAddLine ();
    MenuAddTextItem ( "tразр=", 6, 6 ); MenuAddTimePlayerItem ( &Stage1Time );
    MenuAddLine ();
    MenuAddTextItem ( "Qразр=", 6, 6 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
    MenuAddLine ();
    MenuAddMarkerItem ( "Приостановить", 13, 16 );
    MenuAddVoidCommandItem ( ManualPauseMode, BTN_RIGHT_ONLY );
    MenuAddLine ();
    MenuAddMarkerItem ( "Завершить разряд", 16, 16 );
    MenuAddVoidCommandItem ( ManualStopMode, BTN_RIGHT_ONLY );
  }
  else if ( tmpSubsystemState == MENU_MODE_PAUSED ) {
    MenuAddLine ();
    MenuAddTextItem ("Приостановлен по", 16, 16 );
    MenuAddLine ();
    if (ModeStopOrPauseReason == OPERATOR ) MenuAddTextItem ("решен. оператора", 16, 16);
    else if ( ModeStopOrPauseReason == OVERHEATING ) MenuAddTextItem ("перегреву модуля", 16, 16);
    else if ( ModeStopOrPauseReason == FAULT ) MenuAddTextItem ("аварии", 6, 16 );
    MenuAddLine ();
    MenuAddMarkerItem ( "Uмин=", 5, 5 );
    SimpleControlDischargeBatteryMinVoltageComboBox = MenuAddComboBoxItem ( 2, SimpleControlDischargeMenuOnBatteryMinVoltageSelect );
  if ( SimpleControlDischargeBatteryVoltageIndex == 0 ) {
    MenuAddComboBoxItemElement ( SimpleControlDischargeBatteryMinVoltageComboBox, "21", 2 );
    MenuAddComboBoxItemElement ( SimpleControlDischargeBatteryMinVoltageComboBox, "20,4", 4 );
  }
  else {
    MenuAddComboBoxItemElement ( SimpleControlDischargeBatteryMinVoltageComboBox, "10,5", 4 );
    MenuAddComboBoxItemElement ( SimpleControlDischargeBatteryMinVoltageComboBox, "10,2", 4 );
  }
    ComboBoxSetElementIndex ( SimpleControlDischargeBatteryMinVoltageComboBox, SimpleControlDischargeBatteryMinVoltageIndex );
    MenuAddTextItem ( "<", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &Values[BATTERY_VOLTAGE], 4, 2, 100 ); MenuAddTextItem ( " В", 2, 2 );
    MenuAddLine ();
    MenuAddMarkerItem ( "Iразр=", 6, 6 );
    MenuAddFixedPointNumberEditorItem ( &SimpleControlDischargeStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 );
    MenuAddTextItem ( chAmpers, 2, 2 );
    MenuAddLine ();
    MenuAddTextItem ( "tразр=", 6, 6 ); MenuAddTimePlayerItem ( &Stage1Time );
    MenuAddLine ();
    MenuAddTextItem ( "Qразр=", 6, 6 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
    MenuAddLine ();
    MenuAddMarkerItem ( "Продолж. разряд", 15, 16 );
    MenuAddVoidCommandItem ( ResumeMode, BTN_RIGHT_ONLY );
    if ( ModeStopOrPauseReason != OPERATOR ) {
      MenuAddLine ();
      MenuAddMarkerItem ( "Приостановить", 13, 16 );
      MenuAddVoidCommandItem ( ManualPauseMode, BTN_RIGHT_ONLY );
    }
    MenuAddLine ();
    MenuAddMarkerItem ( "Завершить разряд", 16, 16 );
    MenuAddVoidCommandItem ( ManualStopMode, BTN_RIGHT_ONLY );
  }
  else if ( tmpSubsystemState == MENU_MODE_STOPPED ) {
    MenuAddLine ();
    MenuAddTextItem ("Завершен по", 11, 16 );
    MenuAddLine ();
    if (ModeStopOrPauseReason == OPERATOR ) MenuAddTextItem ("решен. оператора", 16, 16);
    else if ( ModeStopOrPauseReason == FAULT ) MenuAddTextItem ("аварии", 6, 16 );
    else if ( ModeStopOrPauseReason == OVER ) MenuAddTextItem ("готовности", 10, 10 );
    MenuAddLine ();
    MenuAddTextItem ( "Uакб=", 5, 5 );
  if ( SimpleControlDischargeBatteryVoltageIndex == 0 ) {
    MenuAddTextItem ( "24 В", 4, 4 );
  }
  else {
    MenuAddTextItem ( "12 В", 4, 4 );
  }
  MenuAddLine ();
  MenuAddTextItem ( "Uмин=", 5, 5 );
  if ( SimpleControlDischargeBatteryVoltageIndex == 0 ) {
    if ( SimpleControlDischargeBatteryMinVoltageIndex == 0 ) {
      MenuAddTextItem ( "21 В", 4, 4 );
    }
    else {
      MenuAddTextItem ( "20,4 В", 6, 6 );
    }
  }
  if ( SimpleControlDischargeBatteryVoltageIndex == 1 ) {
    if ( SimpleControlDischargeBatteryMinVoltageIndex == 0 ) {
      MenuAddTextItem ( "10,5 В", 6, 6 );
    }
    else {
      MenuAddTextItem ( "10,2 В", 6, 6 );
    }
  }
  MenuAddLine ();
  MenuAddTextItem ( "Iразр=", 6, 6 ); 
    MenuAddFixedPointNumberPlayerItem ( &SimpleControlDischargeStage1MaxCurrent, 4, 2, 100 );
    MenuAddTextItem (" А", 2, 2);
    MenuAddLine ();
    MenuAddTextItem ( "tразр=", 6, 6 ); MenuAddTimePlayerItem ( &Stage1Time );
    MenuAddLine ();
    MenuAddTextItem ( "Qразр=", 6, 6 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
    MenuAddLine ();
    MenuAddMarkerItem ( "Сбросить данные", 15, 16 );
    MenuAddVoidCommandItem ( ClearMode, BTN_RIGHT_ONLY );
  }
  MenuAddLine ();
  MenuAddTextItem (chMenuEnd, 16, 16);
}


extern int32_t IDischarge;

void IncIDischarge ( void ) {
  IDischarge++;
}

void DecIDischarge ( void ) {
  IDischarge--;
  if ( IDischarge < 0 ) { IDischarge = 0; }
}

extern int32_t ControlDischargeStage1MaxCurrent;
extern int32_t ControlDischargeStage1MinVoltage;

void LoadControlDischargeMenu ( void ) {
  ChargeMode = CONTROL_DISCHARGE;
  ClearMenu ();
  MenuOnLeftButtonPress = LoadDifficultModesMenu;
  MenuIndex = CONTROL_DISCHARGE_MENU;
  MenuAddLine ();
  MenuAddTextItem ("**Контр. разряд*", 16, 16);
  tmpSubsystemState = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
  if ( tmpSubsystemState == 0 ) {
    MenuAddLine ();
    MenuAddTextItem ("Настройка режима", 16, 16 );
    MenuAddLine ();
    MenuAddTextItem ("1. Параметры", 12, 16 );
    MenuAddLine ();
    MenuAddMarkerItem ( "Iраз=", 5, 5 );
    MenuAddFixedPointNumberEditorItem ( &ControlDischargeStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 );
    MenuAddTextItem ( " А", 2, 2 );
    MenuAddLine ();
    MenuAddMarkerItem ( "Uмин=", 5, 5 );
    MenuAddFixedPointNumberEditorItem ( &ControlDischargeStage1MinVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( "<", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
    MenuAddTextItem ( " В", 2, 2 );
    MenuAddLine ();
    MenuAddTextItem ("2. Управление", 13, 16 );
    MenuAddLine ();
    MenuAddMarkerItem ( "Начать разряд.", 14, 16 );
    MenuAddVoidCommandItem ( StartMode, BTN_START_ONLY );
  }
  else if ( tmpSubsystemState == MENU_MODE_WORKS ) {
    MenuAddLine ();
    MenuAddTextItem ("Идёт разряд...", 14, 16 );
    MenuAddLine ();
    MenuAddTextItem ("1. Параметры", 12, 16 );
    MenuAddLine ();
    MenuAddMarkerItem ( "Iраз=", 5, 5 ); MenuAddFixedPointNumberEditorItem ( &ControlDischargeStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 ); MenuAddTextItem(">", 1, 1); MenuAddFixedPointNumberPlayerItem ( &Values[DISCHARGE_CURRENT], 4, 2, 100 );
    MenuAddTextItem (" А", 2, 2);
    MenuAddLine ();
    MenuAddMarkerItem ( "Uмин=", 5, 5 ); MenuAddFixedPointNumberEditorItem ( &ControlDischargeStage1MinVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem("<", 1, 1); MenuAddFixedPointNumberPlayerItem ( &Values[BATTERY_VOLTAGE], 4, 2, 100 );
    MenuAddTextItem (" В", 2, 2);
    MenuAddLine ();
    MenuAddTextItem ( "tраз=", 5, 5 ); MenuAddTimePlayerItem ( &Stage1Time );
    MenuAddLine ();
    MenuAddTextItem ( "Qраз=", 5, 5 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
    MenuAddLine ();
    MenuAddTextItem ("2. Управление", 13, 16 );
    MenuAddLine ();
    MenuAddMarkerItem ( "Приостановить", 13, 16 );
    MenuAddVoidCommandItem ( ManualPauseMode, BTN_RIGHT_ONLY );
    MenuAddLine ();
    MenuAddMarkerItem ( "Завершить разряд", 16, 16 );
    MenuAddVoidCommandItem ( ManualStopMode, BTN_RIGHT_ONLY );
  }
  else if ( tmpSubsystemState == MENU_MODE_PAUSED ) {
    MenuAddLine ();
    MenuAddTextItem ("Приостановлен по", 16, 16 );
    MenuAddLine ();
    if (ModeStopOrPauseReason == OPERATOR ) MenuAddTextItem ("решен. оператора", 16, 16);
    else if ( ModeStopOrPauseReason == OVERHEATING ) MenuAddTextItem ("перегреву модуля", 16, 16);
    else if ( ModeStopOrPauseReason == FAULT ) MenuAddTextItem ("аварии", 6, 16 );
    MenuAddLine ();
    MenuAddTextItem ("1. Параметры", 12, 16 );
    MenuAddLine ();
    MenuAddMarkerItem ( "Iраз=", 5, 5 );
    MenuAddFixedPointNumberEditorItem ( &ControlDischargeStage1MaxCurrent, 4, 2, 100, 0, 100, 9000 );
    MenuAddTextItem (" А", 2, 4);
    MenuAddLine ();
    MenuAddMarkerItem ( "Uмин=", 5, 5 );
    MenuAddFixedPointNumberEditorItem ( &ControlDischargeStage1MinVoltage, 4, 2, 100, 0, 500, 3600 ); MenuAddTextItem ( "<", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
    MenuAddTextItem ( " В", 2, 2 );
    MenuAddLine ();
    MenuAddTextItem ( "tраз=", 5, 5 ); MenuAddTimePlayerItem ( &Stage1Time );
    MenuAddLine ();
    MenuAddTextItem ( "Qраз=", 4, 4 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
    MenuAddLine ();
    MenuAddTextItem ("2. Управление", 13, 16 );
    MenuAddLine ();
    MenuAddMarkerItem ( "Продолж. разряд", 15, 16 );
    MenuAddVoidCommandItem ( ResumeMode, BTN_RIGHT_ONLY );
    if ( ModeStopOrPauseReason != OPERATOR ) {
      MenuAddLine ();
      MenuAddMarkerItem ( "Приостановить", 13, 16 );
      MenuAddVoidCommandItem ( ManualPauseMode, BTN_RIGHT_ONLY );
    }
    MenuAddLine ();
    MenuAddMarkerItem ( "Завершить разряд", 16, 16 );
    MenuAddVoidCommandItem ( ManualStopMode, BTN_RIGHT_ONLY );
  }
  else if ( tmpSubsystemState == MENU_MODE_STOPPED ) {
    MenuAddLine ();
    MenuAddTextItem ("Завершен по", 11, 16 );
    MenuAddLine ();
    if (ModeStopOrPauseReason == OPERATOR ) MenuAddTextItem ("решен. оператора", 16, 16);
    else if ( ModeStopOrPauseReason == FAULT ) MenuAddTextItem ("аварии", 6, 16 );
    else if ( ModeStopOrPauseReason == OVER ) MenuAddTextItem ("готовности", 10, 10 );
    MenuAddLine ();
    MenuAddTextItem ("1. Параметры", 12, 16 );
    MenuAddLine ();
    MenuAddTextItem ( "Iраз=", 5, 5 );
    MenuAddFixedPointNumberPlayerItem ( &ControlDischargeStage1MaxCurrent, 4, 2, 100 );
    MenuAddTextItem (" А", 2, 2);
    MenuAddLine ();
    MenuAddTextItem ( "Uмин=", 5, 5 );
    MenuAddFixedPointNumberPlayerItem ( &ControlDischargeStage1MinVoltage, 4, 2, 100 ); MenuAddTextItem ( "<", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(Values[BATTERY_VOLTAGE]), 4, 2, 100 );
    MenuAddTextItem ( " В", 2, 2 );
    MenuAddLine ();
    MenuAddTextItem ( "tраз=", 5, 5 ); MenuAddTimePlayerItem ( &Stage1Time );
    MenuAddLine ();
    MenuAddTextItem ( "Qраз=", 5, 5 ); MenuAddFixedPointNumberPlayerItem ( &Stage1Volume, 6, 4, 100 ); MenuAddTextItem ( " Ач", 3, 3 );
    MenuAddLine ();
    MenuAddTextItem ("2. Управление", 13, 16 );
    MenuAddLine ();
    MenuAddMarkerItem ( "Сбросить данные", 15, 16 );
    MenuAddVoidCommandItem ( ClearMode, BTN_RIGHT_ONLY );
  }
  MenuAddLine ();
  MenuAddTextItem (chMenuEnd, 16, 16);
}

void LoadMeasuresMenu ( void ) {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadMainMenu;
  MenuIndex = MEASURES_MENU;
  MenuAddLine ();
  MenuAddTextItem ("***Измерения****", 16, 16);
  MenuAddLine ();
  MenuAddTextItem ( "Uвыпр=", 6, 6 );
  MenuAddFixedPointNumberPlayerItem ( &( Values[NETWORK_VOLTAGE] ), 4, 2, 100 );
  MenuAddTextItem (" В", 2, 2 );
  MenuAddLine ();
  MenuAddTextItem ( "Uфильт=", 7, 7 );
  MenuAddFixedPointNumberPlayerItem ( &( Values[SOURCE_DC_CONVERTER_VOLTAGE] ), 4, 2, 100 );
  MenuAddTextItem (" В", 2, 2 );
  MenuAddLine ();
  MenuAddTextItem ( "Uакб=", 5, 5 );
  MenuAddFixedPointNumberPlayerItem ( &( Values[BATTERY_VOLTAGE] ), 4, 2, 100 );
  MenuAddTextItem (" В  ", 4, 4 );
  MenuAddLine ();
  MenuAddTextItem ( "Iзар=", 5, 5 );
  MenuAddFixedPointNumberPlayerItem ( &( Values[CHARGE_CURRENT] ), 4, 2, 100 );
  MenuAddTextItem (" А  ", 4, 4 );
  MenuAddLine ();
  MenuAddTextItem ( "Iразр=", 6, 6 );
  MenuAddFixedPointNumberPlayerItem ( &( Values[DISCHARGE_CURRENT] ), 4, 2, 100 );
  MenuAddTextItem (" А ", 3, 3 );
  MenuAddLine ();
  MenuAddTextItem ( "tрад=", 5, 5 );
  MenuAddFixedPointNumberPlayerItem ( &( Values[RADIATOR_TEMPERATURE] ), 4, 2, 100 );
  MenuAddTextItem (" °C ", 4, 4 );
  MenuAddLine ();
  MenuAddTextItem (chMenuEnd, 16, 16);
}

void LoadNoBatteryHelp () {
  ClearFault ( FAULT_NO_BATTERY );
}

extern uint32_t FaultsCount;

void LoadFaultsMenu ( void ) {
  ClearMenu ();
  MenuOnLeftButtonPress = LoadMainMenu;
  MenuIndex = FAULTS_MENU;
  MenuAddLine ();
  MenuAddTextItem ("***Аварии***", 12, 16 );
  if ( GetFaultState(FAULT_NO_BATTERY) ) { MenuAddLine (); MenuAddMarkerItem ("Батарея не подкл.", 16, 16 ); MenuAddIntCommandItem ( ClearFault, FAULT_NO_BATTERY ); }
  if ( GetFaultState(FAULT_BATTERY_SHORT_CIRQUIT) ) { MenuAddLine (); MenuAddMarkerItem ("КЗ АКБ при заряде", 16, 16 ); MenuAddIntCommandItem ( ClearFault, FAULT_BATTERY_SHORT_CIRQUIT ); }
  if ( GetFaultState(PAYLOAD_LOCKING_FAULT) ) { MenuAddLine (); MenuAddMarkerItem ("Авария нагрузки", 16, 16 ); MenuAddIntCommandItem ( ClearFault, PAYLOAD_LOCKING_FAULT ); }
  if ( GetFaultState(FAULT_PAYLOAD_IS_LOCKED) ) { MenuAddLine (); MenuAddMarkerItem ("Нагрузка занята", 15, 16 ); MenuAddIntCommandItem ( ClearFault, FAULT_PAYLOAD_IS_LOCKED ); }
  if ( GetFaultState(PAYLOAD_FREE_FAULT) ) { MenuAddLine(); MenuAddMarkerItem("Неисп освоб нагр", 16, 16); MenuAddIntCommandItem ( ClearFault, PAYLOAD_FREE_FAULT ); }
  if ( GetFaultState(FAULT_PAYLOAD_SHORT_CIRQUIT) ) { MenuAddLine(); MenuAddMarkerItem("КЗ в цепи нагруз", 16, 16); MenuAddIntCommandItem ( ClearFault, FAULT_PAYLOAD_SHORT_CIRQUIT ); }
  if ( GetFaultState(FAULT_LOW_NETWORK_VOLTAGE) ) { MenuAddLine(); MenuAddMarkerItem("Uсети ниже нормы", 16, 16); MenuAddIntCommandItem ( ClearFault, FAULT_LOW_NETWORK_VOLTAGE ); }
  if ( GetFaultState(FAULT_HIGH_NETWORK_VOLTAGE) ) { MenuAddLine(); MenuAddMarkerItem("Uсети выше нормы", 16, 16); MenuAddIntCommandItem ( ClearFault, FAULT_HIGH_NETWORK_VOLTAGE ); }
  if ( GetFaultState(FAULT_LOW_CONVERTER_VOLTAGE) ) { MenuAddLine(); MenuAddMarkerItem("Uпреоб ниже норм", 16, 16); MenuAddIntCommandItem ( ClearFault, FAULT_LOW_CONVERTER_VOLTAGE ); }
  if ( GetFaultState(FAULT_HIGH_CONVERTER_VOLTAGE) ) { MenuAddLine(); MenuAddMarkerItem("Uпреоб выше норм", 16, 16); MenuAddIntCommandItem ( ClearFault, FAULT_HIGH_CONVERTER_VOLTAGE ); }  
  if ( GetFaultState(FAULT_LOW_BATTERY_VOLTAGE) ) { MenuAddLine(); MenuAddMarkerItem("Uакб ниже нормы", 15, 16); MenuAddIntCommandItem ( ClearFault, FAULT_LOW_BATTERY_VOLTAGE ); }
  if ( GetFaultState(FAULT_NO_CHARGE_CURRENT) ) { MenuAddLine(); MenuAddMarkerItem("Нет тока заряда", 16, 16); MenuAddIntCommandItem ( ClearFault, FAULT_NO_CHARGE_CURRENT ); }
  if ( GetFaultState(FAULT_NO_DISCHARGE_CURRENT) ) { MenuAddLine(); MenuAddMarkerItem("Нет тока разряда", 16, 16); MenuAddIntCommandItem ( ClearFault, FAULT_NO_DISCHARGE_CURRENT ); }
  if ( GetFaultState(FAULT_PRECHARGE_FAULT) ) { MenuAddLine(); MenuAddMarkerItem("Авария предзаряда", 16, 16); MenuAddIntCommandItem ( ClearFault, FAULT_PRECHARGE_FAULT ); }
  if ( GetFaultState(FAULT_MODE_CANCEL) ) { MenuAddLine(); MenuAddMarkerItem("Авар зав режима", 15, 16); MenuAddIntCommandItem ( ClearFault, FAULT_MODE_CANCEL ); }
  if ( GetFaultState(CHARGE_REGULATION_FAULT) ) { MenuAddLine(); MenuAddMarkerItem("Отказ регул зар", 15, 16); MenuAddIntCommandItem ( ClearFault, CHARGE_REGULATION_FAULT ); }
  if ( GetFaultState(DISCHARGE_REGULATION_FAULT) ) { MenuAddLine(); MenuAddMarkerItem("Отказ регул разр", 16, 16); MenuAddIntCommandItem ( ClearFault, DISCHARGE_REGULATION_FAULT ); }
  if ( GetFaultState(FAULT_HIGH_BATTERY_VOLTAGE) ) { MenuAddLine(); MenuAddMarkerItem("Uакб выше нормы", 15, 16); MenuAddIntCommandItem ( ClearFault, FAULT_HIGH_BATTERY_VOLTAGE ); }
  if ( GetFaultState(FAULT_RADIATOR_OVERHEAT) ) { MenuAddLine(); MenuAddMarkerItem("Перегрев модуля", 15, 16); MenuAddIntCommandItem ( ClearFault, FAULT_RADIATOR_OVERHEAT ); }
  if ( GetFaultState(FAULT_TEMP_SENSOR_DISCONNECT) ) { MenuAddLine(); MenuAddMarkerItem("Обрыв датч темп", 15, 16); MenuAddIntCommandItem ( ClearFault, FAULT_TEMP_SENSOR_DISCONNECT ); }
  if ( FaultsCount ) {
    MenuAddLine ();
    MenuAddMarkerItem ("Сбросить все", 12, 16 );
    MenuAddVoidCommandItem ( ClearFaults, BTN_RIGHT_ONLY );
  }
  MenuAddLine ();
  MenuAddTextItem (chMenuEnd, 16, 16);
}

static int32_t tmpValue;

void CalibrateChargeVoltage ( int32_t NewVoltageValue ) {
  BatteryVoltageMeasureCalibrations[CalibrationIndex] = tmpValue;
  CalibrationIndex++;
  if ( i < NUM_OF_BATTERY_VOLTAGE_CALIBRATIONS - 1 ) {
    SetTargetChargeVoltageSignal ( 256 * CalibrationIndex );
  }
  else if ( i == NUM_OF_BATTERY_VOLTAGE_CALIBRATIONS - 1 ) {
    SetTargetChargeVoltageSignal ( 4095 );
  }
  else {
    SetSubsystemState ( UZRU10A, UZRU10A_STOP_BATTERY_VOLTAGE_MEASURE_CALIBRATION );
    UpdateInEEPROM ( Calibrations );
    LoadMyMenu ();
  }
}

extern int32_t TargetChargeVoltageSignal;

void LoadCalibrateChargeVoltageMenu ( void ) {
  ClearMenu ();
  MenuIndex = CALIBRATE_BATTERY_VOLTAGE_MENU;
  MenuAddLine ();
  MenuAddIntPlayerItem ( &(NormedPreValues[BATTERY_VOLTAGE]), 4 ); MenuAddTextItem ( "~", 1, 1 ); MenuAddIntPlayerItem ( &TargetChargeVoltageSignal, 4 );
  MenuAddLine ();
  MenuAddMarkerItem ( "U=", 5, 5 ); MenuAddFixedPointNumberEditorItem ( &tmpValue, 5, 3, 100, CalibrateChargeVoltage, 0, 100000 );
  MenuAddTextItem ( "~", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(BatteryVoltageMeasureCalibrations[CalibrationIndex]), 5, 5, 100 );
}

// int32_t* ChargeCurrents = &(Calibrations[CHARGE_CURRENT_ON_0_PREVALUE_INDEX]);

extern int32_t TargetChargeCurrentSignal;

void LoadCalibrateChargeCurrentMenu ( void ) {
  ClearMenu ();
  MenuIndex = CALIBRATE_CHARGE_CURRENT_MENU;
  MenuAddLine ();
  MenuAddIntPlayerItem ( &(NormedPreValues[CHARGE_CURRENT]), 4 ); MenuAddTextItem ( "~", 1, 1 ); MenuAddIntPlayerItem ( &TargetChargeCurrentSignal, 4 );
  MenuAddLine ();
  MenuAddMarkerItem ( "I=", 5, 5 ); MenuAddFixedPointNumberEditorItem ( &tmpValue, 5, 3, 100, CalibrateChargeCurrent, 0, 100000 );
  MenuAddTextItem ( "~", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(ChargeCurrentMeasureCalibrations[i]), 5, 5, 100 );
}

// int32_t* DischargeCurrents = &(Calibrations[DISCHARGE_CURRENT_ON_0_PREVALUE_INDEX]);

extern int32_t TargetDischargeCurrentSignal;

void LoadCalibrateDischargeCurrentMenu ( void ) {
  ClearMenu ();
  MenuIndex = CALIBRATE_DISCHARGE_CURRENT_MENU;
  MenuAddLine ();
  MenuAddIntPlayerItem ( &(NormedPreValues[DISCHARGE_CURRENT]), 4); MenuAddTextItem ( "~", 1, 1 ); MenuAddIntPlayerItem ( &TargetDischargeCurrentSignal, 4 );
  MenuAddLine ();
  MenuAddMarkerItem ( "I=", 5, 5 ); MenuAddFixedPointNumberEditorItem ( &tmpValue, 5, 3, 100, CalibrateChargeVoltage, 0, 100000 );
  MenuAddTextItem ( "~", 1, 1 ); MenuAddFixedPointNumberPlayerItem ( &(DischargeCurrentMeasureCalibrations[i]), 5, 5, 100 );
}

// ?????????? ??????? ?????? "????" ??? ?????? ???? ?? ?????????
void DefaultOnDownButtonPress ( void ) {
  if ( MenuMarkerLine < NumOfMenuLines ) {
    MenuMarkerLine++;
  }
  else {
    MenuMarkerLine = NumOfMenuLines;
  }
  if ( MenuMarkerLine> MenuTopBevelLine ) {
    MenuTopBevelLine = MenuMarkerLine - 1;
  }
}

// ?????????? ??????? ?????? "?????" ??? ?????? ???? ?? ?????????
void DefaultOnUpButtonPress ( void ) {
  if ( MenuMarkerLine > 0 ) {
    MenuMarkerLine--;
  }
  else {
    MenuMarkerLine = 0;
  }
  if ( MenuMarkerLine < MenuTopBevelLine ) {
    MenuTopBevelLine = MenuMarkerLine;
  }
}

// ?????????? ??????? ?????? "????" ??? ?????? ???? ?? ?????????
void DefaultOnLeftButtonPress ( void ) {
  MenuMarkerLine = 0;
  MenuTopBevelLine = 0;
}

// ?????????? ??????? ?????? "?????" ??? ?????? ???? ?? ?????????
void DefaultOnRightButtonPress ( void ) {
  MenuMarkerLine = NumOfMenuLines - 1;
  MenuTopBevelLine = MenuMarkerLine - 1;
}

// ?????????? ??????? ?????? "?????" ??? ?????? ???? ?? ?????????
void DefaultOnStartButtonPress ( void ) {
  SetSubsystemState ( MANUAL_OPERATOR, 0 );
}

// ?????????? ??????? ?????? "????" ??? ?????? ???? ?? ?????????
void DefaultOnStopButtonPress ( void ) {
  int32_t tmpChargeControllerState = GetSubsystemState ( UZRU10A ) & MENU_MODE_MASK;
  if ( tmpChargeControllerState & MENU_MODE_WORKS ) {
    ManualPauseMode ();
  }
  else if ( tmpChargeControllerState & MENU_MODE_PAUSED ) {
    ManualStopMode ();
  }
}

uint32_t  FloatEditorOnDownButtonPress ( uint32_t FloatEditorIndex );
uint32_t  IntEditorOnDownButtonPress ( uint32_t IntEditorIndex );
uint32_t  FixedPointNumberEditorOnDownButtonPress ( uint32_t FixedPointNumberEditorIndex );
uint32_t  TimeEditorOnDownButtonPress ( uint32_t TimeEditorIndex );
uint32_t  FloatEditorOnUpButtonPress ( uint32_t FloatEditorIndex );
uint32_t  IntEditorOnUpButtonPress ( uint32_t IntEditorIndex );
uint32_t  FixedPointNumberEditorOnUpButtonPress ( uint32_t FixedPointNumberEditorIndex );
uint32_t  TimeEditorOnUpButtonPress ( uint32_t TimeEditorIndex );
uint32_t  FloatEditorOnLeftButtonPress ( uint32_t FloatEditorIndex );
uint32_t  IntEditorOnLeftButtonPress ( uint32_t IntEditorIndex );
uint32_t  FixedPointNumberEditorOnLeftButtonPress ( uint32_t FixedPointNumberEditorIndex );
uint32_t  TimeEditorOnLeftButtonPress ( uint32_t TimeEditorIndex );
uint32_t  FloatEditorOnRightButtonPress ( uint32_t FloatEditorIndex );
uint32_t  IntEditorOnRightButtonPress ( uint32_t IntEditorIndex );
uint32_t  FixedPointNumberEditorOnRightButtonPress ( uint32_t FixedPointNumberEditorIndex );
uint32_t  TimeEditorOnRightButtonPress ( uint32_t TimeEditorIndex );
uint32_t  ComboBoxOnUpButtonPress ( uint32_t ComboBoxIndex );
uint32_t  ComboBoxOnDownButtonPress ( uint32_t ComboBoxIndex );
uint32_t  ComboBoxOnLeftButtonPress ( uint32_t ComboBoxIndex );
uint32_t  ComboBoxOnRightButtonPress ( uint32_t ComboBoxIndex );

uint32_t TryExecuteDownButtonPress ( MarkerLine ) {
  for ( i = MenuLines[MarkerLine].FirstItemIndex; i < MenuLines[MarkerLine+1].FirstItemIndex; i++ ) {
    switch ( MenuItems[i].ItemKind ) {
      case FLOAT_EDITOR_ITEM:
        return ( FloatEditorOnDownButtonPress ( MenuItems[i].ItemIndex ) );
        break;
      case INT_EDITOR_ITEM:
        return ( IntEditorOnDownButtonPress ( MenuItems[i].ItemIndex ) );
        break;
      case FIXED_POINT_NUMBER_EDITOR_ITEM:
        return ( FixedPointNumberEditorOnDownButtonPress ( MenuItems[i].ItemIndex ) );
        break;
      case TIME_EDITOR_ITEM:
        return ( TimeEditorOnDownButtonPress ( MenuItems[i].ItemIndex ) );
        break;
      case COMBO_BOX_ITEM:
        return ( ComboBoxOnDownButtonPress ( MenuItems[i].ItemIndex ) );
        break;
    }
  }
  return 0;
}

uint32_t TryExecuteUpButtonPress ( MarkerLine ) {
  for ( i = MenuLines[MarkerLine].FirstItemIndex; i < MenuLines[MarkerLine+1].FirstItemIndex; i++ ) {
    switch ( MenuItems[i].ItemKind ) {
   case FLOAT_EDITOR_ITEM:
    return ( FloatEditorOnUpButtonPress ( MenuItems[i].ItemIndex ) );
    break;
   case INT_EDITOR_ITEM:
    return ( IntEditorOnUpButtonPress ( MenuItems[i].ItemIndex ) );
    break;
   case FIXED_POINT_NUMBER_EDITOR_ITEM:
    return ( FixedPointNumberEditorOnUpButtonPress ( MenuItems[i].ItemIndex ) );
    break;
   case TIME_EDITOR_ITEM:
    return ( TimeEditorOnUpButtonPress ( MenuItems[i].ItemIndex ) );
    break;
      case COMBO_BOX_ITEM:
        return ( ComboBoxOnUpButtonPress ( MenuItems[i].ItemIndex ) );
        break;
   }
  }
  return 0;
}

uint32_t TryExecuteLeftButtonPress ( MarkerLine ) {
  for ( i = MenuLines[MarkerLine].FirstItemIndex; i < MenuLines[MarkerLine+1].FirstItemIndex; i++ ) {
    switch ( MenuItems[i].ItemKind ) {
   case FLOAT_EDITOR_ITEM:
    return ( FloatEditorOnLeftButtonPress ( MenuItems[i].ItemIndex ) );
    break;
   case INT_EDITOR_ITEM:
    return ( IntEditorOnLeftButtonPress ( MenuItems[i].ItemIndex ) );
    break;
   case FIXED_POINT_NUMBER_EDITOR_ITEM:
    return ( FixedPointNumberEditorOnLeftButtonPress ( MenuItems[i].ItemIndex ) );
    break;
   case TIME_EDITOR_ITEM:
    return ( TimeEditorOnLeftButtonPress ( MenuItems[i].ItemIndex ) );
    break;
      case COMBO_BOX_ITEM:
        return ( ComboBoxOnLeftButtonPress ( MenuItems[i].ItemIndex ) );
        break;
   }
  }
  return 0;
}

uint32_t TryExecuteRightButtonPress ( MarkerLine ) {
  for ( i = MenuLines[MarkerLine].FirstItemIndex; i < MenuLines[MarkerLine+1].FirstItemIndex; i++ ) {
  switch ( MenuItems[i].ItemKind ) {
   case FLOAT_EDITOR_ITEM:
    return ( FloatEditorOnRightButtonPress ( MenuItems[i].ItemIndex ) ); // ???? ??????? ?????????? 1, ?? ???? ?? ????????? ???????? ????????
    break;
   case INT_EDITOR_ITEM:
    return ( IntEditorOnRightButtonPress ( MenuItems[i].ItemIndex ) ); // ???? ??????? ?????????? 1, ?? ???? ?? ????????? ???????? ????????
    break;
   case FIXED_POINT_NUMBER_EDITOR_ITEM:
    return ( FixedPointNumberEditorOnRightButtonPress ( MenuItems[i].ItemIndex ) );
    break;
   case TIME_EDITOR_ITEM:
    return ( TimeEditorOnRightButtonPress ( MenuItems[i].ItemIndex ) ); // ???? ??????? ?????????? 1, ?? ???? ?? ????????? ???????? ????????
    break;
      case COMBO_BOX_ITEM:
        return ( ComboBoxOnRightButtonPress ( MenuItems[i].ItemIndex ) );
        break;
   case VOID_COMMAND_ITEM:
    if ( ( VoidCommandItems[MenuItems[i].ItemIndex].Action ) && ( VoidCommandItems[MenuItems[i].ItemIndex].ButtonKinds == BTN_RIGHT_ONLY ) ) { VoidCommandItems[MenuItems[i].ItemIndex].Action(); }
    return 2; // ?.?. ??????? ?????? ???????????, ?? ??? ?????? ??????????? ??????? ????, ?????????????, ?????????? 1
    break;
   case INT_COMMAND_ITEM:
    IntCommandItems[MenuItems[i].ItemIndex].Action ( IntCommandItems[MenuItems[i].ItemIndex].Argument );
    return 2; // ?.?. ??????? ?????? ???????????, ?? ??? ?????? ??????????? ??????? ????, ?????????????, ?????????? 1
    break;
   }
  }
  return 0;
}

void OnDownButtonPress ( void ) {
  if ( !TryExecuteDownButtonPress ( MenuMarkerLine ) ) {
    if ( MenuOnDownButtonPress ) {
    MenuOnDownButtonPress ();
   }
  }
}

void OnUpButtonPress ( void ) {
  if ( !TryExecuteUpButtonPress ( MenuMarkerLine ) ) {
    if ( MenuOnUpButtonPress ) {
      MenuOnUpButtonPress ();
    }
  }
}

void OnLeftButtonPress ( void ) {
  uint32_t tmpResult = TryExecuteLeftButtonPress ( MenuMarkerLine );
  if ( tmpResult == 2 ) {
   MarkerActive = 1;
  }
  else if ( tmpResult == 1 ) {
   MarkerActive = 0;
  }
  else if ( tmpResult == 0 ) {
    if ( MenuOnLeftButtonPress ) {
      MenuOnLeftButtonPress ();
    }
  }
}

void OnRightButtonPress ( void ) {
  uint32_t tmpResult = TryExecuteRightButtonPress ( MenuMarkerLine );
  if ( tmpResult == 2 ) {
   MarkerActive = 1;
  }
  else if ( tmpResult == 1 ) {
   MarkerActive = 0;
  }
  else if ( tmpResult == 0 ) {
    if ( MenuOnRightButtonPress ) {
      MenuOnRightButtonPress ();
    }
  }
}

uint32_t TryExecuteStartButtonPress ( int32_t MarkerLine );
uint32_t TryExecuteStopButtonPress ( void );

void OnStartButtonPress ( void ) {
  if ( !TryExecuteStartButtonPress ( MenuMarkerLine ) ) {
    if ( MenuOnStartButtonPress ) {
      MenuOnStartButtonPress ();
    }
  }
}

void OnStopButtonPress ( void ) {
  if ( !TryExecuteStopButtonPress ( ) ) {
    if ( MenuOnStopButtonPress ) {
      MenuOnStopButtonPress ();
    }
  }
}

void AnalyzeMenu ( void ) {
  uint32_t KeyboardEvent = GetKeyboardEvent ();
  uint32_t KeyIndex = KeyboardEvent >> 16;
  uint32_t KeyAction = KeyboardEvent & 0xFFFF;
  switch ( KeyAction ) {
  case BTN_EVENT_PRESS:
   switch ( KeyIndex ) {
  case BTN_UP:
    OnUpButtonPress ();
    break;
  case BTN_DOWN:
    OnDownButtonPress ();
    break;
  case BTN_LEFT:
    OnLeftButtonPress ();
   break;
  case BTN_RIGHT:
    OnRightButtonPress ();
    break;
  case BTN_START:
    OnStartButtonPress ();
   break;
  case BTN_STOP:
    OnStopButtonPress ();
   break;
  case BTN_ALL_ARROW_BUTTNS:
    LoadPasswordMenu ();
    break;
  }
  break;
  case BTN_EVENT_SHIFT:
   switch ( KeyIndex ) {
  case BTN_UP:
    OnUpButtonPress ();
    break;
  case BTN_DOWN:
    OnDownButtonPress ();
    break;
  case BTN_LEFT:
    OnLeftButtonPress ();
   break;
  case BTN_RIGHT:
    OnRightButtonPress ();
    break;
  case BTN_START:
   break;
  case BTN_STOP:
    OnStopButtonPress ();
   break;
  }
  break;
  }
}

void DisplayMenuLine ( uint32_t LineIndex );

void DisplayMenu ( void ) {
  if ( MenuTopBevelLine > ( NumOfMenuLines - 1 ) ) {
    MenuTopBevelLine = NumOfMenuLines - 1;
  }
  if ( MenuMarkerLine > NumOfMenuLines ) {
    MenuMarkerLine = NumOfMenuLines;
  }
  if ( MenuTopBevelLine < 0 ) { MenuTopBevelLine = 0; }
  DisplayStart1stLine ();
  DisplayMenuLine ( MenuTopBevelLine );
  DisplayStart2ndLine ();
  DisplayMenuLine ( MenuTopBevelLine+1 );
}

void DisplayMenuItem ( uint32_t ItemIndex, uint32_t LineIndex );

void DisplayMenuLine ( uint32_t LineIndex ) {
  for ( i = MenuLines[LineIndex].FirstItemIndex; i < MenuLines[LineIndex+1].FirstItemIndex; i++ ) {
    DisplayMenuItem ( i, LineIndex );
  }
  DisplayNewLine ();
}

void DisplayText ( uint32_t TextItemIndex );
void DisplayMarker ( uint32_t MarkerItemIndex, uint32_t Pulsate );
void DisplayStaticMarker ( uint32_t StaticMarkerItemIndex );
void DisplayFloatPlayer ( uint32_t FloatPlayerIndex );
void DisplayIntPlayer ( uint32_t IntPlayerIndex );\
void DisplayFixedPointNumberPlayer ( uint32_t FixedPointNumblerPlayerIndex );
void DisplayTimePlayer ( uint32_t TimePlayerIndex );
void DisplayFloatEditor ( uint32_t FloatEditorIndex );
void DisplayIntEditor ( uint32_t IntEditorIndex );
void DisplayFixedPointNumberEditor ( int32_t FixedPointNumblerEditorIndex );
void DisplayTimeEditor ( uint32_t TimeEditorIndex );
void DisplayComboBoxItem ( int32_t ComboBoxIndex );

void DisplayMenuItem ( uint32_t ItemIndex, uint32_t LineIndex ) {
  switch ( MenuItems[ItemIndex].ItemKind ) {
   case TEXT_ITEM:
    DisplayText ( MenuItems[ItemIndex].ItemIndex );
    break;
   case MARKER_ITEM:
    DisplayMarker ( MenuItems[ItemIndex].ItemIndex, ( MenuMarkerLine == LineIndex ) && ( MarkerActive ) );
    break;
   case STATIC_MARKER_ITEM:
    DisplayStaticMarker ( MenuItems[ItemIndex].ItemIndex );
    break;
   case COMBO_BOX_ITEM:
    DisplayComboBoxItem ( MenuItems[ItemIndex].ItemIndex );
    break;
   /*
   case FLOAT_PLAYER_ITEM:
    DisplayFloatPlayer ( MenuItems[ItemIndex].ItemIndex );
    break;
   */
   case INT_PLAYER_ITEM:
    DisplayIntPlayer ( MenuItems[ItemIndex].ItemIndex );
    break;
   case FIXED_POINT_NUMBER_PLAYER_ITEM:
    DisplayFixedPointNumberPlayer ( MenuItems[ItemIndex].ItemIndex );
    break;
   case TIME_PLAYER_ITEM:
    DisplayTimePlayer ( MenuItems[ItemIndex].ItemIndex );
    break;
   case FLOAT_EDITOR_ITEM:
    DisplayFloatEditor ( MenuItems[ItemIndex].ItemIndex );
    break;
   case INT_EDITOR_ITEM:
    DisplayIntEditor ( MenuItems[ItemIndex].ItemIndex );
    break;
   case FIXED_POINT_NUMBER_EDITOR_ITEM:
    DisplayFixedPointNumberEditor ( MenuItems[ItemIndex].ItemIndex );
    break;
   case TIME_EDITOR_ITEM:
    DisplayTimeEditor ( MenuItems[ItemIndex].ItemIndex );
    break;
  }
}

void DisplayText ( uint32_t TextItemIndex ) {
  WriteText ( TextItems[TextItemIndex].pText, TextItems[TextItemIndex].Length, TextItems[TextItemIndex].Length, 0, 1 );
}

void DisplayMarker ( uint32_t MarkerItemIndex, uint32_t Pulsate ) {
  if ( Pulsate ) {
    if ( MenuTimeStamp & 0x02000000 ) {
      WriteText ( MarkerItems[MarkerItemIndex].pMarkerText, MarkerItems[MarkerItemIndex].Length, MarkerItems[MarkerItemIndex].Length, 0, 1 );
    }
    else {
      WriteText ( pEmptyText, MarkerItems[MarkerItemIndex].Length, MarkerItems[MarkerItemIndex].Length, 0, 1 );
    }
  }
  else {
   WriteText ( MarkerItems[MarkerItemIndex].pMarkerText, MarkerItems[MarkerItemIndex].Length, MarkerItems[MarkerItemIndex].Length, 0, 1 );
  }
}

void DisplayStaticMarker ( uint32_t StaticMarkerItemIndex ) {
  WriteText ( MarkerItems[StaticMarkerItemIndex].pMarkerText, MarkerItems[StaticMarkerItemIndex].Length, MarkerItems[StaticMarkerItemIndex].Length );
}

/*
void DisplayFloatPlayer ( uint32_t FloatPlayerIndex ) {
  WriteFloat ( FloatPlayerItems[FloatPlayerIndex].pFloat, FloatPlayerItems[FloatPlayerIndex].Width );
}*/

void DisplayIntPlayer ( uint32_t IntPlayerIndex ) {
  WriteInt ( IntPlayerItems[IntPlayerIndex].pInt, IntPlayerItems[IntPlayerIndex].Width );
}

void DisplayFixedPointNumberPlayer ( uint32_t FixedPointNumblerPlayerIndex ) {
  if ( IsIntervalOverFrom ( (uint64_t)49000000, (uint64_t)FixedPointNumberPlayerItems[FixedPointNumblerPlayerIndex].LastValueUpdateTimeStamp ) ) {
    FixedPointNumberPlayerItems[FixedPointNumblerPlayerIndex].Value = *(FixedPointNumberPlayerItems[FixedPointNumblerPlayerIndex].pInt);
    GetTimeStamp ( &( FixedPointNumberPlayerItems[FixedPointNumblerPlayerIndex].LastValueUpdateTimeStamp ) );
  }
  WriteFixedPointNumber ( FixedPointNumberPlayerItems[FixedPointNumblerPlayerIndex].Value, FixedPointNumberPlayerItems[FixedPointNumblerPlayerIndex].Divider, FixedPointNumberPlayerItems[FixedPointNumblerPlayerIndex].Width );
}

void DisplayTimePlayer ( uint32_t TimePlayerIndex ) {
  WriteTime ( TimePlayerItems[TimePlayerIndex].pTime );
}

void DisplayFloatEditor ( uint32_t FloatEditorIndex ) {
}

void DisplayFixedPointNumberEditor ( int32_t FixedPointNumberEditorIndex ) {
/*
  if ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition == FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Width ) {
   if ( MenuTimeStamp & 0x02000000 ) {
    for ( j = 0; j < FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Width; j++ ) {
      WriteChar ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Digits[j] );
    }
   }
   else {
    WriteText ( pEmptyText, FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Width, FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Width );
   }
  }
  else */ if ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition < FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Width ) {
   for ( j = 0; j < FixedPointNumberEditorItems[FixedPointNumberEditorIndex].PointPosition; j++ ) {
    if ( j == FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition ) {
     if ( MenuTimeStamp & 0x02000000 ) {
      WriteChar ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Digits[j] );
     }
     else {
      WriteChar ( ' ' );
     }
    }
    else {
     WriteChar ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Digits[j] );
    }
   }
   WriteChar (',');
   for ( j = FixedPointNumberEditorItems[FixedPointNumberEditorIndex].PointPosition+1; j < FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Width; j++ ) {
    if ( j == FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition ) {
     if ( MenuTimeStamp & 0x02000000 ) {
      WriteChar ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Digits[j] );
     }
     else {
      WriteChar ( ' ' );
     }
    }
    else {
     WriteChar ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Digits[j] );
    }
   }
  }
  else {
    for ( j = 0; j < FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Width; j++ ) {
      WriteChar ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Digits[j] );
    }
  }
  //*/
}

void DisplayIntEditor ( uint32_t IntEditorIndex ) {
}

void DisplayComboBoxItem ( int32_t ComboBoxIndex ) {
  int32_t tmpComboBoxElementIndex = ComboBoxItems[ComboBoxIndex].FirstElementIndex + ComboBoxItems[ComboBoxIndex].ElementIndex;
  if ( ComboBoxItems[ComboBoxIndex].CursorPosition == 1 ) {
    if ( MenuTimeStamp & 0x02000000 ) {
      WriteText ( pEmptyText, ComboBoxElements[tmpComboBoxElementIndex].TextLength, ComboBoxElements[tmpComboBoxElementIndex].TextLength, 0, 1 );
    }
    else {
      WriteText ( ComboBoxElements[tmpComboBoxElementIndex].pItemText, ComboBoxElements[tmpComboBoxElementIndex].TextLength, ComboBoxElements[tmpComboBoxElementIndex].TextLength, 0, 1 );
    }
  }
  else {
    WriteText ( ComboBoxElements[tmpComboBoxElementIndex].pItemText, ComboBoxElements[tmpComboBoxElementIndex].TextLength, ComboBoxElements[tmpComboBoxElementIndex].TextLength, 0, 1 );
  }
}

void DisplayTimeEditor ( uint32_t TimeEditorIndex ) {
  if ( TimeEditorItems[TimeEditorIndex].CursorPosition == 6 ) {
   if ( MenuTimeStamp & 0x02000000 ) {
    WriteChar ( TimeEditorItems[TimeEditorIndex].Digits[0] );
    WriteChar ( TimeEditorItems[TimeEditorIndex].Digits[1] );
    WriteChar ( 'ч' );
    WriteChar ( TimeEditorItems[TimeEditorIndex].Digits[2] );
    WriteChar ( TimeEditorItems[TimeEditorIndex].Digits[3] );
    WriteChar ( 'м' );
    WriteChar ( TimeEditorItems[TimeEditorIndex].Digits[4] );
    WriteChar ( TimeEditorItems[TimeEditorIndex].Digits[5] );
    WriteChar ( 'с' );
   }
   else { WriteText ( pEmptyText, 9, 9 ); }
  }
  else if ( TimeEditorItems[TimeEditorIndex].CursorPosition == 7 ) {
   WriteChar ( TimeEditorItems[TimeEditorIndex].Digits[0] );
   WriteChar ( TimeEditorItems[TimeEditorIndex].Digits[1] );
   WriteChar ( 'ч' );
   WriteChar ( TimeEditorItems[TimeEditorIndex].Digits[2] );
   WriteChar ( TimeEditorItems[TimeEditorIndex].Digits[3] );
   WriteChar ( 'м' );
   WriteChar ( TimeEditorItems[TimeEditorIndex].Digits[4] );
   WriteChar ( TimeEditorItems[TimeEditorIndex].Digits[5] );
   WriteChar ( 'с' );   
  }
  else {
   if ( ( TimeEditorItems[TimeEditorIndex].CursorPosition == 0 ) && !( MenuTimeStamp & 0x02000000 ) ) { WriteChar ( ' ' ); } else { WriteChar ( TimeEditorItems[TimeEditorIndex].Digits[0] ); }
   if ( ( TimeEditorItems[TimeEditorIndex].CursorPosition == 1 ) && !( MenuTimeStamp & 0x02000000 ) ) { WriteChar ( ' ' ); } else { WriteChar ( TimeEditorItems[TimeEditorIndex].Digits[1] ); }
   WriteChar ( 'ч' );  
   if ( ( TimeEditorItems[TimeEditorIndex].CursorPosition == 2 ) && !( MenuTimeStamp & 0x02000000 ) ) { WriteChar ( ' ' ); } else { WriteChar ( TimeEditorItems[TimeEditorIndex].Digits[2] ); }
   if ( ( TimeEditorItems[TimeEditorIndex].CursorPosition == 3 ) && !( MenuTimeStamp & 0x02000000 ) ) { WriteChar ( ' ' ); } else { WriteChar ( TimeEditorItems[TimeEditorIndex].Digits[3] ); }
   WriteChar ( 'м' );
   if ( ( TimeEditorItems[TimeEditorIndex].CursorPosition == 4 ) && !( MenuTimeStamp & 0x02000000 ) ) { WriteChar ( ' ' ); } else { WriteChar ( TimeEditorItems[TimeEditorIndex].Digits[4] ); }
   if ( ( TimeEditorItems[TimeEditorIndex].CursorPosition == 5 ) && !( MenuTimeStamp & 0x02000000 ) ) { WriteChar ( ' ' ); } else { WriteChar ( TimeEditorItems[TimeEditorIndex].Digits[5] ); }    
   WriteChar ( 'с' );  
  }
}

uint32_t FloatEditorOnDownButtonPress ( uint32_t FloatEditorIndex ) {
  return 0;
}

uint32_t IntEditorOnDownButtonPress ( uint32_t IntEditorIndex ) {
  return 0;
}

uint32_t TimeEditorOnDownButtonPress ( uint32_t TimeEditorIndex ) {
  if ( TimeEditorItems[TimeEditorIndex].CursorPosition == 7 ) {
   return 0;
  }
  else if ( TimeEditorItems[TimeEditorIndex].CursorPosition < 7 ) { 
   if ( TimeEditorItems[TimeEditorIndex].Digits[TimeEditorItems[TimeEditorIndex].CursorPosition] > '0' ) {
    TimeEditorItems[TimeEditorIndex].Digits[TimeEditorItems[TimeEditorIndex].CursorPosition]--;
   }
   return 1;
  }
}

uint32_t FloatEditorOnUpButtonPress ( uint32_t FloatEditorIndex ) {
  return 0;
}

char tmpBuffer;

uint32_t FixedPointNumberEditorOnUpButtonPress ( uint32_t FixedPointNumberEditorIndex ) {
  if ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition >= FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Width ) {
    return 0;
  }
  else if ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition < FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Width ) {
    if ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Digits[FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition] < '9' ) {
      FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Digits[FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition]++;
    }
  }
  return 1;
}

uint32_t FixedPointNumberEditorOnDownButtonPress ( uint32_t FixedPointNumberEditorIndex ) {
  if ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition >= FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Width ) {
    return 0;
  }
  else if ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition < FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Width ) {
    if ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Digits[FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition] == ',' ) {
    }
    else if ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Digits[FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition] > '0' ) {
      FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Digits[FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition]--;
    }
    else {
    }
  }
  return 1;
}

uint32_t IntEditorOnUpButtonPress ( uint32_t IntEditorIndex ) {
  return 0;
}

uint32_t TimeEditorOnUpButtonPress ( uint32_t TimeEditorIndex ) {
  if ( TimeEditorItems[TimeEditorIndex].CursorPosition == 7 ) {
   return 0;
  }
  else if ( TimeEditorItems[TimeEditorIndex].CursorPosition < 7 ) { 
   if ( ( TimeEditorItems[TimeEditorIndex].CursorPosition != 2 ) && ( TimeEditorItems[TimeEditorIndex].CursorPosition != 4 ) ) {
     if ( TimeEditorItems[TimeEditorIndex].Digits[TimeEditorItems[TimeEditorIndex].CursorPosition] < '9' ) {
      TimeEditorItems[TimeEditorIndex].Digits[TimeEditorItems[TimeEditorIndex].CursorPosition]++;
     }
   }
   else {
     if ( TimeEditorItems[TimeEditorIndex].Digits[TimeEditorItems[TimeEditorIndex].CursorPosition] < '5' ) {
      TimeEditorItems[TimeEditorIndex].Digits[TimeEditorItems[TimeEditorIndex].CursorPosition]++;
     }
   }
   return 1;
  }
}

uint32_t FloatEditorOnLeftButtonPress ( uint32_t FloatEditorIndex ) {
  return 0;
}

uint32_t IntEditorOnLeftButtonPress ( uint32_t IntEditorIndex ) {
  return 0;
}

uint32_t TimeEditorOnLeftButtonPress ( uint32_t TimeEditorIndex ) {
  if ( TimeEditorItems[TimeEditorIndex].CursorPosition >= 7 ) {
   return 0;
  }
  else if ( TimeEditorItems[TimeEditorIndex].CursorPosition == 0 ) {
   TimeEditorItems[TimeEditorIndex].CursorPosition = 7;
   DownloadTimeEditorItemValue ( TimeEditorIndex );
   return 2;
  }
  else {
   TimeEditorItems[TimeEditorIndex].CursorPosition--;
   return 1;
  }
}

uint32_t FloatEditorOnRightButtonPress ( uint32_t FloatEditorIndex ) {
  return 0;
}

uint32_t IntEditorOnRightButtonPress ( uint32_t IntEditorIndex ) {
  return 0;
}

uint32_t FixedPointNumberEditorOnLeftButtonPress ( uint32_t FixedPointNumberEditorIndex ) {
  if ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition >= FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Width ) {
   return 0;
  }
  else if ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition > 0 ) {
   FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition--;
   if ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Digits[FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition] < '0' ) {
     FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Digits[FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition] = ',';
     if ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition > 0 ) {
       FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition--;
     }
     else {
       FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition = FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Width;
       DownloadFixedPointNumberEditorItemValue ( FixedPointNumberEditorIndex );
       return 2;
     }
   }
   return 1;
  }
  else {
   FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition = FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Width;
   DownloadFixedPointNumberEditorItemValue ( FixedPointNumberEditorIndex );
   return 2;
  }
}

void UploadFixedPointNumberEditorItemValue ( int32_t FixedPointNumberEditorIndex ) {
  int32_t i;
  int32_t tmpValue = FixedPointNumberEditorItems[FixedPointNumberEditorIndex].PointPosition;
  int32_t tmpDivider = FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Divider;
  while ( tmpValue > 1 ) {
    tmpDivider *= 10;
    tmpValue--;
  }
  tmpValue = 0;
  i = 0;
  while ( i < FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Width ) {
    if ( i != FixedPointNumberEditorItems[FixedPointNumberEditorIndex].PointPosition ) {
      tmpValue += ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Digits[i] - '0')*tmpDivider;
      tmpDivider /= 10;
    }
    i++;
  }
  if ( tmpValue > FixedPointNumberEditorItems[FixedPointNumberEditorIndex].MaxValue ) {
    tmpValue = FixedPointNumberEditorItems[FixedPointNumberEditorIndex].MaxValue;
  }
  else if ( tmpValue < FixedPointNumberEditorItems[FixedPointNumberEditorIndex].MinValue ) {
    tmpValue = FixedPointNumberEditorItems[FixedPointNumberEditorIndex].MinValue;
  }
  *(FixedPointNumberEditorItems[FixedPointNumberEditorIndex].pValue) = tmpValue;
  DownloadFixedPointNumberEditorItemValue ( FixedPointNumberEditorIndex );
}

uint32_t FixedPointNumberEditorOnRightButtonPress ( uint32_t FixedPointNumberEditorIndex ) {
  if ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition == FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Width - 1 ) {
   FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition++;
   UploadFixedPointNumberEditorItemValue ( FixedPointNumberEditorIndex );
   if ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].OnConfirm ) { FixedPointNumberEditorItems[FixedPointNumberEditorIndex].OnConfirm ( *(FixedPointNumberEditorItems[FixedPointNumberEditorIndex].pValue) ); }
   return 2;
  }
  else if ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition >= FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Width ) {
   FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition = 0;
   return 1;
  }
  else {
   FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition++;
   if ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Digits[FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition] == ',' ) {
     FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition++;
     if ( FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition == FixedPointNumberEditorItems[FixedPointNumberEditorIndex].Width + 1 ) {
       FixedPointNumberEditorItems[FixedPointNumberEditorIndex].CursorPosition--;
     }
   }
   return 1;
  }
}

int32_t buffComboBoxElementIndex;

uint32_t ComboBoxOnRightButtonPress ( uint32_t ComboBoxItemIndex ) {
  if ( ComboBoxItems[ComboBoxItemIndex].CursorPosition == 0 ) {
    buffComboBoxElementIndex = ComboBoxItems[ComboBoxItemIndex].ElementIndex;
    ComboBoxItems[ComboBoxItemIndex].CursorPosition = 1;
    return 1;
  }
  else {
    ComboBoxItems[ComboBoxItemIndex].CursorPosition = 0;
    if ( ComboBoxItems[ComboBoxItemIndex].Action ) { ComboBoxItems[ComboBoxItemIndex].Action( ComboBoxItems[ComboBoxItemIndex].ElementIndex ); }
    return 2;
  }
}

uint32_t ComboBoxOnLeftButtonPress ( uint32_t ComboBoxItemIndex ) {
  if ( ComboBoxItems[ComboBoxItemIndex].CursorPosition == 0 ) {
    return 0;
  }
  else {
    ComboBoxItems[ComboBoxItemIndex].CursorPosition = 0;
    ComboBoxItems[ComboBoxItemIndex].ElementIndex = buffComboBoxElementIndex;
    return 2;
  }
}

uint32_t ComboBoxOnUpButtonPress ( uint32_t ComboBoxItemIndex ) {
  if ( ComboBoxItems[ComboBoxItemIndex].CursorPosition == 0 ) {
    ComboBoxItems[ComboBoxItemIndex].NumOfElements - 1;
  }
  else {
    if ( ComboBoxItems[ComboBoxItemIndex].ElementIndex > 0 ) {
      ComboBoxItems[ComboBoxItemIndex].ElementIndex--;
    }
    return 1;
  }
}

uint32_t ComboBoxOnDownButtonPress ( uint32_t ComboBoxItemIndex ) {
  if ( ComboBoxItems[ComboBoxItemIndex].CursorPosition == 0 ) {
    return 0;
  }
  else {
    if ( ComboBoxItems[ComboBoxItemIndex].ElementIndex < ( ComboBoxItems[ComboBoxItemIndex].NumOfElements - 1 ) ) {
      ComboBoxItems[ComboBoxItemIndex].ElementIndex = ( ComboBoxItems[ComboBoxItemIndex].ElementIndex + 1 ) % ComboBoxItems[ComboBoxItemIndex].NumOfElements;
    }
    return 1;
  }  
}

uint64_t tmpTimeEditorResult;

void UploadTimeEditorItemValue ( uint32_t TimeEditorIndex ) {
  uint64_t* pTime = TimeEditorItems[TimeEditorIndex].pTime;
  uint64_t tmpTime = (TimeEditorItems[TimeEditorIndex].Digits[0]-'0');
  tmpTime = (uint64_t)( (uint64_t)tmpTime * (uint64_t)10);
  tmpTime = (uint64_t)( (uint64_t)tmpTime + ( TimeEditorItems[TimeEditorIndex].Digits[1]-'0' ) );
  tmpTime = (uint64_t)( (uint64_t)tmpTime * (uint64_t)6);
  tmpTime = (uint64_t)( (uint64_t)tmpTime + ( TimeEditorItems[TimeEditorIndex].Digits[2]-'0' ) );
  tmpTime = (uint64_t)( (uint64_t)tmpTime * (uint64_t)10);
  tmpTime = (uint64_t)( (uint64_t)tmpTime + ( TimeEditorItems[TimeEditorIndex].Digits[3]-'0' ) );
  tmpTime = (uint64_t)( (uint64_t)tmpTime * (uint64_t)6);
  tmpTime = (uint64_t)( (uint64_t)tmpTime + ( TimeEditorItems[TimeEditorIndex].Digits[4]-'0' ) );
  tmpTime = (uint64_t)( (uint64_t)tmpTime * (uint64_t)10);
  tmpTime = (uint64_t)( (uint64_t)tmpTime + ( TimeEditorItems[TimeEditorIndex].Digits[5]-'0' ) );
  tmpTime = (uint64_t)( (uint64_t)tmpTime * (uint64_t)96000000);
  *pTime = (uint64_t)tmpTime;
}

uint32_t TimeEditorOnRightButtonPress ( uint32_t TimeEditorIndex ) {
  if ( TimeEditorItems[TimeEditorIndex].CursorPosition >= 7 ) {
   TimeEditorItems[TimeEditorIndex].CursorPosition = 0;
   return 1;
  }
  else if ( TimeEditorItems[TimeEditorIndex].CursorPosition == 6 ) {
   TimeEditorItems[TimeEditorIndex].CursorPosition = 7;
   UploadTimeEditorItemValue ( TimeEditorIndex );
   // if ( TimeEditorItems[TimeEditorIndex].OnConfirm ) { TimeEditorItems[TimeEditorIndex].OnConfirm(tmpTimeEditorResult); }
   return 2;
  }
  else {
   TimeEditorItems[TimeEditorIndex].CursorPosition++;
   return 1;
  }
}

uint32_t TryExecuteStartButtonPress ( MarkerLine ) {
  for ( i = MenuLines[MarkerLine].FirstItemIndex; i < MenuLines[MarkerLine+1].FirstItemIndex; i++ ) {
  switch ( MenuItems[i].ItemKind ) {
    case VOID_COMMAND_ITEM:
      if ( ( VoidCommandItems[MenuItems[i].ItemIndex].Action ) && ( VoidCommandItems[MenuItems[i].ItemIndex].ButtonKinds == BTN_START_ONLY ) ) { VoidCommandItems[MenuItems[i].ItemIndex].Action(); }
      return 2; // ?.?. ??????? ?????? ???????????, ?? ??? ?????? ??????????? ??????? ????, ?????????????, ?????????? 1
      break;
    }
  }
  return 0;
}

uint32_t TryExecuteStopButtonPress ( void ) {
  return 0;
}
