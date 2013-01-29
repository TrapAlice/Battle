#ifndef _MONSTER_H
#define _MONSTER_H

struct object_t;
struct combat_t;
struct inventory_t;
struct equipment_t;
struct messagelist_t;
struct skills_t;

typedef struct monster_t monster_t;

struct monster_t{
	char* name;
	struct combat_t *combat;
	int xp;
	struct inventory_t *inventory;
	struct object_t *object;
	struct equipment_t *equipment;
	struct skills_t *skills;
	void (*deathFunction)(monster_t*);
};

extern monster_t* player;

monster_t* createPlayer(int,int);
monster_t* createMonster(char*, int, int, int, int,void(*mobdeath)(monster_t*));
monster_t* cloneMonster(monster_t*);
int checkDead(monster_t*);
void deleteMonster(monster_t*);
void attackMonster(struct messagelist_t*, monster_t*, monster_t*);
void takeDamage(struct messagelist_t*, monster_t*, int);

#endif