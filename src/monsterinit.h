#ifndef _MONSTERINIT_H
#define _MONSTERINIT_H

#include "moonmem.h"
#include "monster.h"


enum MON_ID{
	mob_null,
	mob_slime,
	mob_pig,
	num_monsters,
};

extern Monster** MonsterList;

void initMonsters();
void uninitMonsters();

#endif