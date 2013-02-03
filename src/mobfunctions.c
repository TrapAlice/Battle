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
	addMessage(globalMessage, "The %s dies", monster->name);
	int carving = getSkillLevelifActive(player->skills, SKILL_CARVING);
	switch( monster->id ){
		case mob_pig:
			if( oneIn(5-carving) ){
				addMessage(globalMessage, "Successfully carved some meat");
				addItemInventory(player->inventory, cloneItem(ItemList[item_meat]));
				increaseSkill(player->skills, SKILL_CARVING, 3);
			}
			increaseSkill(player->skills, SKILL_CARVING, 2);
			break;
	}
}

void fairyDeath(const monster_t* const monster){
	addMessage(globalMessage, "The fairy explodes, you get covered in dust");
	takeDamage(player, -10);
}