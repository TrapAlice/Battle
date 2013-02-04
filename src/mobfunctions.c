#include "mobfunctions.h"
#include "monster.h"
#include "msg.h"
#include "item.h"
#include "equipped.h"
#include "rng.h"
#include "combat.h"
#include "skills.h"
#include "inventory.h"

/*Birth functions*/
void equipMonster(monster_t* const monster){
	item_t* weapon;
	monster->equipment = createEquipmentSlots();
	switch(monster->id){
		case mob_kobold:
			weapon = ItemList[item_knife];
			Equip(monster->equipment, E_RHAND, weapon);
			break;
	}
}

/*Attack functions*/
void fairyAttack(monster_t* const attacker, monster_t* const defender){
	int damage;
	addMessage(globalMessage, "The fairy fires a fireball");
	damage = roll(attacker->combat->power, 6);
	takeDamage(defender, damage);
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
	standardDeath(monster);
	for( x=0; x<num_slots; ++x ){
		if( (item = getEquipment(monster->equipment, x)) ){
			clone = cloneItem(item);
			duraDmg = between(0, clone->maxDurability+2)-2,
			duraDmg = duraDmg < 0 ? 0 : duraDmg;
			clone->durability -= duraDmg;
			randomItemEnchant(clone, 0);
			addMessage(globalMessage, "You found a %s%s",getItemCondition(clone),clone->name);
			addItemInventory(player->inventory, clone);
		}
	}
}
