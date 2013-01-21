#ifndef _UI_H
#define _UI_H

#include "libtcod.h"

void printUI();

extern TCOD_console_t msgConsole;
extern TCOD_console_t combatConsole;
extern TCOD_console_t statusPanel;
extern TCOD_console_t inventoryPanel;

#endif