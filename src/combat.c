#include "combat.h"
#include "moonmem.h"

void Combat_attack(Combat* attacker, Combat* defender){
	int damage = attacker->power - defender->defense;
	printf("Attacker: power: %d defense: %d hp: %d maxhp: %d\n",attacker->power, attacker->defense, attacker->hp, attacker->maxhp);
	printf("Defender: power: %d defense: %d hp: %d maxhp: %d\n",defender->power, defender->defense, defender->hp, defender->maxhp);
	Combat_takeDamage(defender, damage);
}

void Combat_takeDamage(Combat* defender, int damage){
	damage = (damage < 0) ? 0 : damage;
	printf("%d/%d damage:%d\n",defender->maxhp, defender->hp, damage);
	defender->hp -= damage;
	printf("%d/%d damage:%d\n",defender->maxhp, defender->hp, damage);
}

Combat* Combat_create(int maxhp, int power, int defense){
	Combat* combat = malloc(sizeof(Combat));
	combat->hp = maxhp;
	combat->maxhp = maxhp;
	combat->power = power;
	combat->defense = defense;
	return combat;
}