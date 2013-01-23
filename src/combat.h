#ifndef _COMBAT_H
#define _COMBAT_H

typedef struct{
	int hp, maxhp;
	int defense;
	int power;
	int hits;
}Combat;

Combat* createCombat(int, int, int);
void deleteCombat(Combat*);

#endif