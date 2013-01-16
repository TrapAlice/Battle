#include "combat.h"
#include "moonmem.h"

void Combat_attack(const Combat* attacker, const char* attacker_name, Combat* defender, const char* defender_name){
	int damage = attacker->power - defender->defense;
	printf("%s attacks %s for %d\n", attacker_name, defender_name, damage);
	Combat_takeDamage(defender, defender_name, damage);
}

void Combat_takeDamage(Combat* defender, const char* defender_name, int damage){
	damage = (damage < 0) ? 0 : damage;
	defender->hp -= damage;
	printf("%s takes %d damage\n",defender_name, damage);
}

Combat* Combat_create(int maxhp, int power, int defense){
	Combat* combat = malloc(sizeof(Combat));
	combat->hp = maxhp;
	combat->maxhp = maxhp;
	combat->power = power;
	combat->defense = defense;
	return combat;
}