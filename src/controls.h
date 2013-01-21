#ifndef _CONTROLS_H
#define _CONTROLS_H

#include "libtcod.h"

TCOD_key_t key;

void waitForPress();
void getInput(TCOD_key_t*);

#endif