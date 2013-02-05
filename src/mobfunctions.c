#include "mobfunctions.h"
#include "monster.h"
#include "msg.h"
#include "item.h"
#include "equipped.h"
#include "rng.h"
#include "combat.h"
#include "skills.h"
#include "inventory.h"
#include "dbg.h"
#include "buff.h"

/*Birth functions*/
void equipMonster(monster_t* const monster){
	item_t* weapon;
	item_t* shield;
	monster->equipment = createEquipmentSlots();
	switch(monster->id){
		case mob_kobold:
			weapon = ItemList[item_knife];
			randomItemEnchant(weapon, 2);
			Equip(monster->equipment, E_RHAND, weapon);
			break;
		case mob_knight:
			weapon = ItemList[item_sword];
			shield = ItemList[item_shield];
			randomItemEnchant(shield, 2);
			Equip(monster->equipment, E_RHAND, weapon);
			Equip(monster->equipment, E_LHAND, shield);
			break;
	}
}

/*Attack functions*/
void standardAttack(monster_t* const attacker, monster_t* const defender){
	int damage;
	int basepower = attacker->combat->power;
	item_t* weapon = getEquipment(attacker->equipment, E_RHAND);
	int weaponpower = (weapon ? weapon->power : 0);
	
	int damagemod = 0;
	int power;
	int basedefense = defender->combat->defense;
	int armordefense = getEquipmentDefense(defender->equipment);
	int defense = basedefense + armordefense;
	int shieldblock;
	item_t* shield;

	triggerBuffs(attacker);

	if( weaponpower ){
		weaponpower += getSkillLevelifActive(attacker->skills, weapon->relatedSkill)/3;
		damagemod += getSkillLevelifActive(attacker->skills, weapon->relatedSkill) + weapon->powerBonus;
		shield = getEquipment(defender->equipment, E_LHAND);
		shieldblock = (shield ? shield->power : 0);
		if( shieldblock ){
			if( roll(basepower, 6) + weaponpower < roll(shieldblock + defender->combat->power, 6) + getSkillLevelifActive(defender->skills, SKILL_SHIELD)*2 ){
				if( defender == player ){
					addMessage(globalMessage, "You blocked the attack with your shield!");
				} else {
					addMessage(globalMessage, "The %s blocked your attack with their shield!", defender->name);
				}
				
				increaseSkillifActive(defender->skills, getEquipment(defender->equipment, E_LHAND)->relatedSkill, 2);
				return;
			}
		}
		if( itemDamage(weapon) ){
			addMessage(globalMessage, "Your weapon breaks");
			attacker->equipment->equipped[E_RHAND]=0;
		}
	}

	power = basepower + weaponpower;
	if( attacker == player ){
		addMessage(globalMessage, "You attack the %s", defender->name);
	} else {
		addMessage(globalMessage, "The %s attacks you", attacker->name);
	}
	
	damage = roll(power, 6)+damagemod;
	damage -= roll(defense, 6);
	
	damage = damage < 0 ? 0 : damage;

	

	takeDamage(defender, damage);
	
}

void fairyAttack(monster_t* const attacker, monster_t* const defender){
	int damage;
	addMessage(globalMessage, "The fairy fires a fireball");
	damage = roll(attacker->combat->power, 6);
	takeDamage(defender, damage);
}

void knightAttack(monster_t* const attacker, monster_t* const defender){
	standardAttack(attacker, defender);
	if( attacker->combat->hp<10 && !attacker->buff ){
		addMessage(globalMessage, "The Knight lifts his shield and starts chanting");
		attacker->buff = createBuff(5, &healingTick);
	}
}

/*Death functions*/
void standardDeath(const monster_t* const monster){
	addMessage(globalMessage, "The %s dies", monster->name);
}

void carveOnDeath(const monster_t* const monster){
	standardDeath(monster);
	switch( monster->id ){
		case mob_pig:
			if( skillCheck(player->skills, SKILL_CARVING, 5) ){
				addMessage(globalMessage, "Successfully carved some meat");
				addItemInventory(player->inventory, cloneItem(ItemList[item_meat]));
				increaseSkill(player->skills, SKILL_CARVING, 3);
			}
			increaseSkill(player->skills, SKILL_CARVING, 2);
			break;
		case mob_crab:
			if ( skillCheck(player->skills, SKILL_CARVING, 12) ){
				addMessage(globalMessage, "Successfully carved some meat");
				addItemInventory(player->inventory, cloneItem(ItemList[item_crabmeat]));
				increaseSkill(player->skills, SKILL_CARVING, 5);
			}
			increaseSkill(player->skills, SKILL_CARVING, 3);
			break;
	}
}

void fairyDeath(const monster_t* const monster){
	addMessage(globalMessage, "The fairy explodes, you get covered in dust");
	takeDamage(player, -10);
}

void dropsEquipment(const monster_t* const monster){
	int x;
	item_t* item;
	item_t* clone;
	int duraDmg;
	char str[40];
	standardDeath(monster);
	for( x=0; x<num_slots; ++x ){
		if( (item = getEquipment(monster->equipment, x)) ){
			clone = cloneItem(item);
			duraDmg = between(0, clone->maxDurability+2)-2,
			duraDmg = duraDmg < 0 ? 0 : duraDmg;
			clone->durability -= duraDmg;
			addMessage(globalMessage, "You found a %s", 
				getFullItemName(item, str));
			addItemInventory(player->inventory, clone);
		}
	}
}
