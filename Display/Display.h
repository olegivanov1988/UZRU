                                                                                                                                                                                                                                                                             #ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "stdint.h"
#include "main.h"

typedef enum {
  DISPLAY_WANTS_NOTHING     //< Константа, обозначающая, что 
} eDisplayWants;

typedef enum {
  START_SHOW_PARAMETER,
  START_EDIT_PARAMETER,
  START_EDIT_PASSWORD,
  START_SHOW_MENU,
  START_SHOW_PROGRAMMS,
  START_SHOW_FAULTS,
  START_SHOW_MESSAGES,
  CONFIRM_PASSWORD,
  CONFIRM_PARAMETER,
  EXECUTE_PROGRAMM,
  CLEAR_FAULT,
  CLEAR_ALL_FAULTS,
  NUM_OF_COMMANDS
} DisplayCommands;

typedef enum {
  SHOW_PARAMETER,
  EDIT_PARAMETER,
  SHOW_MENU,
  SHOW_PROGRAMMS,
  SHOW_FAULTS,
  SHOW_MESSAGES,
  EDIT_PASSWORD,
  NUM_OF_MODES
} DisplayModes;

typedef struct {
  uint32_t Command;
  int32_t Parameter1;
  int32_t Parameter2;
  int32_t Parameter3;
} DisplayCommand;

void InitDisplay ( void );
void DispatchDisplay ( void );
void AnalyzeDisplay ( void );

#endif // __DISPLAY_H__
