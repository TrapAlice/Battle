#ifndef _COMBAT_H
#define _COMBAT_H

typedef struct combat_t{
	int hp, maxhp;
	int defense;
	int power;
}combat_t;

combat_t*  createCombat ( int, int, int );
combat_t*  cloneCombat  ( const combat_t* );
void       deleteCombat ( combat_t* );

#endif