#ifndef _MONSTER_H
#define _MONSTER_H

#include "combat.h"

typedef struct{
	char* name;
	Combat* combat;
}Monster;

Monster* Monster_create(char*);
int Monster_checkDead(Monster*);

#endif//_MONSTER_H