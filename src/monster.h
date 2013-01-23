#ifndef _MONSTER_H
#define _MONSTER_H

#include "combat.h"
#include "object.h"
#include "inventory.h"
#include "equipped.h"
#include "msg.h"

typedef struct{
	char* name;
	Combat* combat;
	int xp;
	Inventory* inventory;
	Object* object;
	Equipment* equipment;
}Monster;

extern Monster* player;

Monster* Monster_playerCreate(int,int);
Monster* Monster_create(char*, int, int, int, int);
Monster* Monster_clone(Monster*);
int Monster_checkDead(Monster*);
void Monster_delete(Monster*);
void Monster_attack(MessageList*, Monster*, Monster*);
void Monster_takeDamage(MessageList*, Monster*, int);

#endif