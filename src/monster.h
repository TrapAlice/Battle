#ifndef _MONSTER_H
#define _MONSTER_H

#include "combat.h"
#include "object.h"
#include "inventory.h"
#include "equipped.h"
#include "msg.h"
#include "skills.h"

typedef struct monster_t monster_t;

struct monster_t{
	char* name;
	Combat* combat;
	int xp;
	Inventory* inventory;
	Object* object;
	Equipment* equipment;
	Skills* skills;
	void (*deathFunction)(monster_t*);
};

extern monster_t* player;

monster_t* createPlayer(int,int);
monster_t* createMonster(char*, int, int, int, int,void(*mobdeath)(monster_t*));
monster_t* cloneMonster(monster_t*);
int checkDead(monster_t*);
void deleteMonster(monster_t*);
void attackMonster(MessageList*, monster_t*, monster_t*);
void takeDamage(MessageList*, monster_t*, int);

#endif