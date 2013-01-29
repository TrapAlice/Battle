#ifndef _MONSTERINIT_H
#define _MONSTERINIT_H

#include "monster.h"

extern monster_t** MonsterList;

enum MON_ID{
	mob_null,
	mob_slime,
	mob_pig,
	mob_fairy,
	num_monsters,
};

void initMonsters();
void uninitMonsters();

#endif