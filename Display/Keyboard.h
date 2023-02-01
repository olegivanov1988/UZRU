#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "stdint.h"
#include "main.h"

typedef enum {
  BTN_NO = 0,
  BTN_LEFT = BTN_NO,
  BTN_RIGHT,
  BTN_DOWN,
  BTN_UP,
  BTN_STOP,
  BTN_START,
  BTN_ALL_ARROW_BUTTNS,
  NUM_OF_KEYS
} ButtonID;

typedef enum {
  BTN_EVENT_NO_EVENTS = 0,
  BTN_EVENT_PRESS,
  BTN_EVENT_SHIFT,
  BTN_EVENT_SHIFT1,
  BTN_EVENT_SHIFT2,
  BTN_EVENT_SHIFT3,
  BTN_EVENT_DOWN,
  BTN_EVENT_UP,
  BTN_EVENT_IGNORE
} ButtonEvent;

typedef enum {
  BTN_STATE_DOWN = 0,
  BTN_STATE_UP,
  BTN_STATE_IGNORE
} ButtonState;

typedef enum {
  KEYBOARD_USED,
  KEYBOARD_FREE
} eKeyboardStates;

typedef enum {
  KEYBOARD_WANTS_NOTHING     //< Константа, обозначающая, что 
} eKeyboardWants;

void InitKeyboard ( void );
void DispatchKeyboard ( void );
void AnalyzeKeyboard ( void );

uint32_t GetKeyboardEvent ( void );

#endif // __KEYBOARD_H__
