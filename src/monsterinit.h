#ifndef _MONSTERINIT_H
#define _MONSTERINIT_H

#include "moonmem.h"
#include "monster.h"


enum MON_ID{
	null,
	mob_slime,
	mob_pig,
	num_monsters,
};

extern Monster** MonsterList;

void MonstersInit();
void MonstersGet(enum MON_ID);
void MonstersUninit();

#endif//_MONSTERINIT_H