#include "mobattack.h"
#include "monster.h"
#include "msg.h"
#include "item.h"
#include "equipped.h"
#include "rng.h"
#include "combat.h"
#include "skills.h"

void standardAttack(monster_t* const attacker, monster_t* const defender){
	int damage;
	int basepower = attacker->combat->power;
	int weaponpower = (attacker->equipment ? (getEquipment(attacker->equipment, E_RHAND) ? getEquipment(attacker->equipment, E_RHAND)->power : 0 ) : 0);
	int power = basepower + weaponpower;
	int basedefense = defender->combat->defense;
	int armordefense = getEquipmentDefense(defender->equipment);
	int defense = basedefense + armordefense;
	int shield;
	addMessage(globalMessage, "%s attacks %s", attacker->name, defender->name);

	if( weaponpower ){		
		shield = (defender->equipment ? (getEquipment(defender->equipment, E_LHAND) ? getEquipment(defender->equipment, E_LHAND)->power : 0) : 0);
		if( shield ){
			if( roll(basepower, 6) < roll(shield + defender->combat->power, 6) ){
				addMessage(globalMessage, "You blocked the attack with your shield!");
				increaseSkillifActive(defender->skills, getEquipment(defender->equipment, E_LHAND)->relatedSkill, 2);
				return;
			}
		}
	}

	damage = roll(power, 6);
	damage -= roll(defense, 6);
	
	damage = damage < 0 ? 0 : damage;

	takeDamage(globalMessage, defender, damage);
}

void fairyAttack(monster_t* const attacker, monster_t* const defender){
	int damage;
	addMessage(globalMessage, "The fairy fires a fireball");
	damage = roll(attacker->combat->power, 6);
	takeDamage(globalMessage, defender, damage);
}