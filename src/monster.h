#ifndef _MONSTER_H
#define _MONSTER_H

#include "combat.h"
#include "object.h"

typedef struct{
	char* name;
	Combat* combat;
	Object* object;
}Monster;

Monster* Monster_playerCreate(int,int);
Monster* Monster_create(char*);
int Monster_checkDead(Monster*);

#endif//_MONSTER_H