#ifndef _MONSTER_H
#define _MONSTER_H

#include "monsterid.h"

struct object_t;
struct combat_t;
struct inventory_t;
struct equipment_t;
struct messagelist_t;
struct skills_t;

typedef struct monster_t monster_t;

struct monster_t{
	const char         *name;
	struct combat_t    *combat;
	int                 xp;
	struct inventory_t *inventory;
	struct object_t    *object;
	struct equipment_t *equipment;
	struct skills_t    *skills;
	void               (*birthFunction)(monster_t*);
	void               (*attackFunction)(monster_t*,monster_t*);
	void               (*deathFunction)(const monster_t*);
};

extern monster_t* player;
extern monster_t** MonsterList;

monster_t*  createPlayer  ( int, int );
monster_t*  createMonster ( const char*, int, int, int, int, void(*)(monster_t*), void(*)(monster_t*,monster_t*), void(*)(const monster_t*) );
monster_t*  cloneMonster  ( const monster_t* );
int         checkDead     ( const monster_t* );
void        deleteMonster ( monster_t* );
void        attackMonster ( struct messagelist_t*, monster_t*, monster_t* );
void        takeDamage    ( struct messagelist_t*, monster_t*, int );

#endif