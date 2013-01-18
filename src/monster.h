#ifndef _MONSTER_H
#define _MONSTER_H

#include "combat.h"
#include "object.h"

typedef struct{
	char* name;
	Combat* combat;
	int xp;
	Object* object;
}Monster;

//extern Monster* player;

Monster* Monster_playerCreate(int,int);
Monster* Monster_create(char*, int);
Monster* Monster_clone(Monster*);
int Monster_checkDead(Monster*);
void Monster_delete(Monster*);

#endif//_MONSTER_H