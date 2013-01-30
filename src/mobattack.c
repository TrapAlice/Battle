#include "mobattack.h"
#include "monster.h"
#include "msg.h"
#include "item.h"
#include "equipped.h"
#include "rng.h"
#include "combat.h"

void standardAttack(monster_t* attacker, monster_t* defender){
	int damage;
	int basepower = attacker->combat->power;
	int weaponpower = (attacker->equipment==NULL ? 0 : (getEquipment(attacker->equipment, E_HAND) == NULL ? 0 : getEquipment(attacker->equipment, E_HAND)->power));
	int basedefense = defender->combat->defense;
	int armordefense = (defender->equipment==NULL ? 0 : (getEquipment(defender->equipment, E_CHEST) == NULL ? 0 : getEquipment(defender->equipment, E_CHEST)->power));
	addMessage(globalMessage, "The %s attacks %s",attacker->name, defender->name);

	damage = roll(attacker->combat->hits, basepower);
	damage += (weaponpower < 0 ? 0 : roll(1, weaponpower));
	damage -= basedefense;
	damage -= roll(1,armordefense);
	
	damage = damage < 0 ? 0 : damage;

	takeDamage(globalMessage, defender, damage);
}

void fairyAttack(monster_t* attacker, monster_t* defender){
	int damage;
	addMessage(globalMessage,"The fairy fires a fireball");
	damage = roll(2,5);
	takeDamage(globalMessage, defender, damage);
}