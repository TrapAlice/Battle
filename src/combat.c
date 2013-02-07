#include "combat.h"
#include "moonmem.h"
#include "rng.h"
#include "msg.h"

combat_t* createCombat(int maxhp, int power, int defense){
	combat_t* combat = malloc(sizeof(combat_t));
	combat->hp = maxhp;
	combat->maxhp = maxhp;
	combat->power = power;
	combat->defense = defense;
	return( combat );
}

combat_t* cloneCombat(const combat_t* const combat){
	combat_t* clone = malloc(sizeof(combat_t));
	clone->hp = combat->maxhp;
	clone->maxhp = combat->maxhp;
	clone->power = combat->power;
	clone->defense = combat->defense;
	return( clone );
}

void deleteCombat(combat_t* const combat){
	free(combat);
}