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

Monster* createPlayer(int,int);
Monster* createMonster(char*, int, int, int, int);
Monster* cloneMonster(Monster*);
int checkDead(Monster*);
void deleteMonster(Monster*);
void attackMonster(MessageList*, Monster*, Monster*);
void takeDamage(MessageList*, Monster*, int);

#endif