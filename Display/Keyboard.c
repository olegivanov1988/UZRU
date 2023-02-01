#include "main.h"
#include "Subsystems.h"
#include "Keyboard.h"
#include "Measurements.h"
#include "TimeControl.h"
#include "DriverMDR1986VE1T.h"
#include "Board.h"
#include "Relays.h"
#include "ChargeController.h"

int32_t KeysStates = 0;

typedef struct {
  uint32_t State;
  uint64_t LastEvent;
  uint64_t LastEventTimeStamp;
} ButtonStateStruct;

char* ButtonNames [7] = { "BTN_UP   ", "BTN_DOWN ", "BTN_LEFT ", "BTN_RIGHT", "BTN_START", "BTN_STOP ", "BTN_ALL" };

ButtonStateStruct ButtonsStates[NUM_OF_KEYS] = { { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 } };

uint32_t KeyIndex = 0;
uint32_t KeyState;

uint32_t KeyboardState = KEYBOARD_USED;

uint64_t LastButtonPressTimeStamp;

void InitKeyboard ( void ) {                              // Функция, выполняющая инициализацию объекта "Двигатель"
  KeyboardState = KEYBOARD_USED;
  GetTimeStamp ( &LastButtonPressTimeStamp );
  for ( KeyIndex = 0; KeyIndex < NUM_OF_KEYS; KeyIndex++ ) {
    ButtonsStates[KeyIndex].State = BTN_STATE_UP;
    ButtonsStates[KeyIndex].LastEvent = BTN_EVENT_NO_EVENTS;
  }
}

void DispatchKeyboard ( void ) {
}

void AnalyzeKeyboard ( void ) {                      // Функция анализа состояния объекта "Двигатель"
  if ( KeyboardState == KEYBOARD_USED ) {
    if ( IsIntervalOverFrom ( (uint64_t)1920000000, LastButtonPressTimeStamp ) ) {
      KeyboardState = KEYBOARD_FREE;
      SetSubsystemState ( KEYBOARD, KEYBOARD_FREE );
    }
    else {
      SetSubsystemState ( KEYBOARD, KEYBOARD_USED );
    }
  }
  else {
    SetSubsystemState ( KEYBOARD, KEYBOARD_FREE );
  }
}

uint32_t GetKeyboardEvent ( void ) {
  KeyIndex = ( KeyIndex + 1 ) % NUM_OF_KEYS;
  if ( KeyIndex != BTN_ALL_ARROW_BUTTNS ) {  // Если речь не идёт о виртуальной кнопке "все стрелки"...
    KeyState = ( Board_GetButtonsState () >> KeyIndex ) & 0x00000001;            // Запрашиваем у драйвера платы состояние текущей клавиши.
    if ( KeyState == BTN_STATE_DOWN ) { // Если кнопка нажата, то...
      KeyboardState = KEYBOARD_USED;
      SetRelayState ( HIGHLIGHT, ON );
      GetTimeStamp ( &LastButtonPressTimeStamp );
      if ( ( ( Board_GetButtonsState () | ( 1 << KeyIndex ) ) & 0x0000000F ) != 0x0000000F ) { // Если нажаты ещё кнопки
        KeyState = BTN_STATE_IGNORE;
      }
    }
    else {
    }
  }
  else {
    if ( ( Board_GetButtonsState () & 0x0000000F ) == 0x00000000 ) {
      KeyState = BTN_STATE_DOWN;
    }
    else {
      KeyState = BTN_STATE_UP;
    }
  }
  
  // В этом месте состояние кнопки может быть следующих видов: BTN_STATE_UP, BTN_STATE_DOWN и BTN_STATE_IGNORE
  
  // После того, как положение кнопки определено, можно распознавать событие кнопки
  if ( KeyState == BTN_STATE_UP ) {                        // Если клавиша не нажата, то...
    if ( ButtonsStates[KeyIndex].State == BTN_STATE_DOWN ) {   // А в предыдущий момент времени она была нажата, то...
      if ( ButtonsStates[KeyIndex].LastEvent == BTN_EVENT_DOWN ) { // Если предыдущее событие - надавливание кнопки, то...
        if ( IsIntervalOverFrom ( 4800000, ButtonsStates[KeyIndex].LastEventTimeStamp ) ) {    // Если с момента надавливания уже прошло 10 мс, то...
          ButtonsStates[KeyIndex].State = BTN_STATE_UP;            // Записываем новое состояние клавиши - не нажата,
          ButtonsStates[KeyIndex].LastEvent = BTN_EVENT_PRESS;     // записываем последнее событие клавиши - "щелчок",
          GetTimeStamp ( &(ButtonsStates[KeyIndex].LastEventTimeStamp) ); // записываем момент времени нажатия на клавишу,
          return ( KeyIndex << 16 ) | BTN_EVENT_PRESS; // возвращаем распознанное событие - номер клавиши и идентификатор нажатия.
        }
      }
      else {  // Если предыдущее событие - не надавливание, то это либо BTN_EVENT_SHIFT, либо BTN_EVENT_IGNORE, и в любом случае игнорируем событие.
        ButtonsStates[KeyIndex].State = BTN_STATE_UP; // Запоминаем, что кнопка не была нажата
        ButtonsStates[KeyIndex].LastEvent = BTN_EVENT_UP; // Последнее событие - отпускание кнопки
        GetTimeStamp  ( &(ButtonsStates[KeyIndex].LastEventTimeStamp) ); // Метка времени последнего события
        return 0;
      }
    }
    else { // Если предыдущее состояние - отпущена или игнор, то
      ButtonsStates[KeyIndex].State = BTN_STATE_UP;
      ButtonsStates[KeyIndex].LastEvent = BTN_EVENT_UP;
      return 0;
    }
  }
  else if ( KeyState == BTN_STATE_DOWN ) {                          // Иначе, т.е. если клавиша в текущий момент времени нажата
    if ( ButtonsStates[KeyIndex].State == BTN_STATE_UP ) { //         // Если в предыдущий момент времени клавиша не была нажата, то:
      ButtonsStates[KeyIndex].State = BTN_STATE_DOWN;                   // запоминаем новое состояние клавиши,
      ButtonsStates[KeyIndex].LastEvent = BTN_EVENT_DOWN;               // запоминаем новое событие клавиши - надавливание,
      GetTimeStamp ( &(ButtonsStates[KeyIndex].LastEventTimeStamp) );   // запоминаем метку времени события.
      return 0;
    }
    else if ( ButtonsStates[KeyIndex].State == BTN_STATE_DOWN ) {
      if ( ( ButtonsStates[KeyIndex].LastEvent == BTN_EVENT_DOWN ) && ( IsIntervalOverFrom ( 96000000, ButtonsStates[KeyIndex].LastEventTimeStamp ) ) ) {
        ButtonsStates[KeyIndex].LastEvent = BTN_EVENT_SHIFT1;           // запоминаем новое событие клавиши - удерживание, итерация 1,
        GetTimeStamp ( &( ButtonsStates[KeyIndex].LastEventTimeStamp ) );// запоминаем метку времени события,
        return ( KeyIndex << 16 ) | BTN_EVENT_SHIFT;                     // возвращаем событие - номер клавиши и идентификатор удержания.
      }
      else if ( ( ButtonsStates[KeyIndex].LastEvent == BTN_EVENT_SHIFT1 ) && ( IsIntervalOverFrom ( 48000000, ButtonsStates[KeyIndex].LastEventTimeStamp ) ) ) {
        ButtonsStates[KeyIndex].LastEvent = BTN_EVENT_SHIFT2;            // запоминаем новое событие клавиши - удержание, итерация 2,
        GetTimeStamp ( &( ButtonsStates[KeyIndex].LastEventTimeStamp ) );// запоминаем метку времени события,
        return ( KeyIndex << 16 ) | BTN_EVENT_SHIFT;                     // возвращаем событие - номер клавиши и идентификатор удержания.
      }
      else if ( ( ButtonsStates[KeyIndex].LastEvent == BTN_EVENT_SHIFT2 ) && ( IsIntervalOverFrom ( 9600000, ButtonsStates[KeyIndex].LastEventTimeStamp ) ) ) {
        // ButtonsStates[KeyIndex].LastEvent = BTN_EVENT_SHIFT2;            // запоминаем новое событие клавиши - удержание, итерация 2,
        GetTimeStamp ( &( ButtonsStates[KeyIndex].LastEventTimeStamp ) );// запоминаем метку времени события,
        return ( KeyIndex << 16 ) | BTN_EVENT_SHIFT;                     // возвращаем событие - номер клавиши и идентификатор удержания.
      }
      else {
        return 0;
      }
    }
  }
  else { // Иначе, т.е. если клавиша в текущий момент времени находится в состоянии IGNORE
    ButtonsStates[KeyIndex].State = BTN_STATE_IGNORE;
    ButtonsStates[KeyIndex].LastEvent = BTN_EVENT_IGNORE;
  }
}
