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
	combat->hits = 1;
	return combat;
}

void deleteCombat(combat_t* combat){
	free(combat);
}