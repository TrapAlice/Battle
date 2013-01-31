#include "mobattack.h"
#include "monster.h"
#include "msg.h"
#include "item.h"
#include "equipped.h"
#include "rng.h"
#include "combat.h"

void standardAttack(monster_t* const attacker, monster_t* const defender){
	int damage;
	int basepower = attacker->combat->power;
	int weaponpower = (attacker->equipment ? (getEquipment(attacker->equipment, E_HAND) ? getEquipment(attacker->equipment, E_HAND)->power : 0 ) : 0);
	int power = basepower + weaponpower;
	int basedefense = defender->combat->defense;
	int armordefense = (defender->equipment ? (getEquipment(defender->equipment, E_CHEST) ? getEquipment(defender->equipment, E_CHEST)->power : 0) : 0);
	int defense = basedefense + armordefense;
	addMessage(globalMessage, "%s attacks %s",attacker->name, defender->name);

	damage = roll(power, 6);
	damage -= roll(defense, 6);
	
	damage = damage < 0 ? 0 : damage;

	takeDamage(globalMessage, defender, damage);
}

void fairyAttack(monster_t* const attacker, monster_t* const defender){
	int damage;
	addMessage(globalMessage,"The fairy fires a fireball");
	damage = roll(attacker->combat->power,6);
	takeDamage(globalMessage, defender, damage);
}