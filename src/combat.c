#include "combat.h"
#include "moonmem.h"
#include "rng.h"
#include "msg.h"

Combat* Combat_create(int maxhp, int power, int defense){
	Combat* combat = malloc(sizeof(Combat));
	combat->hp = maxhp;
	combat->maxhp = maxhp;
	combat->power = power;
	combat->defense = defense;
	combat->hits = 1;
	return combat;
}

void Combat_delete(Combat* combat){
	free(combat);
}