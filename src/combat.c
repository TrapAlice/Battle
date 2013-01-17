#include "combat.h"
#include "moonmem.h"
#include "rng.h"
#include "msg.h"

void Combat_attack(const Combat* attacker, const char* attacker_name, Combat* defender, const char* defender_name){
	int damage = RNG_roll(attacker->hits,attacker->power) - defender->defense;
	Combat_takeDamage(defender, defender_name, damage);
}

void Combat_takeDamage(Combat* defender, const char* defender_name, int damage){
	defender->hp -= damage;
	if(defender->hp > defender->maxhp){
		defender->hp = defender->maxhp;
	}
}

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