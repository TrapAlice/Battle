#include "monster.h"
#include "moonmem.h"
#include "rng.h"

#include "combat.h"
#include "object.h"
#include "inventory.h"
#include "equipped.h"
#include "msg.h"
#include "skills.h"
#include "item.h"

monster_t* player;

monster_t** MonsterList;

monster_t* createPlayer(int x, int y){
	monster_t* monster = malloc(sizeof(monster_t));
	monster->name = "Player";
	monster->combat = createCombat(32,6,1);
	monster->object = createObject('@',x,y);
	monster->inventory = createInventory();
	monster->equipment = createEquipmentSlots();
	monster->skills = createSkillSlots();
	return monster;
}

monster_t* createMonster(char* name, int hp, int power, int defense, int xp, void(*mobbirth)(monster_t*), void(*mobattack)(monster_t*,monster_t*), void(*mobdeath)(monster_t*)){
	monster_t* monster = malloc(sizeof(monster_t));
	monster->name = name;
	monster->combat = createCombat(hp,power,defense);
	monster->xp = xp;
	monster->inventory = createInventory();
	monster->birthFunction = mobbirth;
	monster->attackFunction = mobattack;
	monster->deathFunction = mobdeath;
	return monster;
}

monster_t* cloneMonster(monster_t* monster){
	monster_t* clone = malloc(sizeof(monster_t));
	clone->name = monster->name;
	clone->xp = monster->xp;
	clone->combat = createCombat(monster->combat->maxhp, monster->combat->power, monster->combat->defense);
	if(monster->birthFunction){
		(monster->birthFunction)(clone);
	}
	clone->attackFunction = monster->attackFunction;
	clone->deathFunction = monster->deathFunction;
	return clone;
}

int checkDead(monster_t* monster){
	int dead = ( monster->combat->hp < 0  ? 1 : 0 );
	if(monster->deathFunction != NULL && dead){
		(monster->deathFunction)(monster);
	}
	return dead;
}

void deleteMonster(monster_t* monster){
	if( monster->object != NULL) deleteObject(monster->object);
	if( monster->inventory != NULL) deleteInventory(monster->inventory);
	if( monster->combat != NULL) deleteCombat(monster->combat);
	if( monster->equipment != NULL) deleteEquipmentSlots(monster->equipment);
	if( monster->skills != NULL ) deleteSkillSlots(monster->skills);
	free(monster);
}

static void _improvePlayerSkills(){
	if(player->equipment->equipped[E_HAND] != NULL){
		if(isSkillActive(player->skills, getEquipment(player->equipment, E_HAND)->relatedSkill)){
			increaseSkill(player->skills, getEquipment(player->equipment, E_HAND)->relatedSkill, 10);
		}
	}
}

void attackMonster(messagelist_t* messageLog, monster_t* attacker, monster_t* defender){
	if(attacker->attackFunction){
		(attacker->attackFunction)(attacker, defender);
	} else {
		int damage;
		int basepower = attacker->combat->power;
		int weaponpower = (attacker->equipment==NULL ? 0 : (getEquipment(attacker->equipment, E_HAND) == NULL ? 0 : getEquipment(attacker->equipment, E_HAND)->power));
		int basedefense = defender->combat->defense;
		int armordefense = (defender->equipment==NULL ? 0 : (getEquipment(defender->equipment, E_CHEST) == NULL ? 0 : getEquipment(defender->equipment, E_CHEST)->power));
		addMessage(messageLog, "%s attacks %s",attacker->name, defender->name);

		damage = roll(attacker->combat->hits, basepower);
		damage += (weaponpower < 0 ? 0 : roll(1, weaponpower));
		damage -= basedefense;
		damage -= roll(1,armordefense);
		
		damage = damage < 0 ? 0 : damage;

		_improvePlayerSkills();

		takeDamage(messageLog, defender, damage);
	}
}

void takeDamage(messagelist_t* messageLog, monster_t* defender, int damage){
	if(damage>0){
		addMessage(messageLog, "%s takes %d damage", defender->name, damage);
	} else if (damage < 0){
		addMessage(messageLog, "%s is healed for %d", defender->name, -damage);
	} else {
		addMessage(messageLog, "%s avoids the attack", defender->name, damage);
	}
	defender->combat->hp -= damage;
	if(defender->combat->hp > defender->combat->maxhp){
		defender->combat->hp = defender->combat->maxhp;
	}
}