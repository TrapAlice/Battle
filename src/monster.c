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
	monster->combat = createCombat(32,2,1);
	monster->object = createObject('@',x,y);
	monster->inventory = createInventory();
	monster->equipment = createEquipmentSlots();
	monster->skills = createSkillSlots();
	return( monster );
}

monster_t* createMonster(const char* const name, int hp, int power, int defense, int xp, void(*mobbirth)(monster_t*), void(*mobattack)(monster_t*,monster_t*), void(*mobdeath)(const monster_t*)){
	monster_t* monster = malloc(sizeof(monster_t));
	monster->name = name;
	monster->combat = createCombat(hp,power,defense);
	monster->xp = xp;
	monster->inventory = createInventory();
	monster->birthFunction = mobbirth;
	monster->attackFunction = mobattack;
	monster->deathFunction = mobdeath;
	return( monster );
}

monster_t* cloneMonster(const monster_t* const monster){
	monster_t* clone = malloc(sizeof(monster_t));
	clone->name = monster->name;
	clone->xp = monster->xp;
	clone->combat = createCombat(monster->combat->maxhp, monster->combat->power, monster->combat->defense);
	if( monster->birthFunction ){
		(monster->birthFunction)(clone);
	}
	clone->attackFunction = monster->attackFunction;
	clone->deathFunction = monster->deathFunction;
	return( clone );
}

int checkDead(const monster_t* const monster){
	int dead = ( monster->combat->hp < 0  ? 1 : 0 );
	if( monster->deathFunction && dead ){
		(monster->deathFunction)(monster);
	}
	return( dead );
}

void deleteMonster(monster_t* const monster){
	if( monster->object ) deleteObject(monster->object);
	if( monster->inventory ) deleteInventory(monster->inventory);
	if( monster->combat ) deleteCombat(monster->combat);
	if( monster->equipment ) deleteEquipmentSlots(monster->equipment);
	if( monster->skills ) deleteSkillSlots(monster->skills);
	free(monster);
}

static void _improvePlayerSkills(){
	if( player->equipment->equipped[E_HAND] ){
		if( isSkillActive(player->skills, getEquipment(player->equipment, E_HAND)->relatedSkill) ){
			increaseSkill(player->skills, getEquipment(player->equipment, E_HAND)->relatedSkill, 10);
		}
	}
}

void attackMonster(messagelist_t* const messageLog, monster_t* const attacker, monster_t* const defender){
	if( attacker->attackFunction ){
		(attacker->attackFunction)(attacker, defender);
	} else {
		int damage;
		int basepower = attacker->combat->power;
		int weaponpower = (attacker->equipment ? (getEquipment(attacker->equipment, E_HAND) ? getEquipment(attacker->equipment, E_HAND)->power : 0 ) : 0);
		int power = basepower + weaponpower;
		int basedefense = defender->combat->defense;
		int armordefense = (defender->equipment ? (getEquipment(defender->equipment, E_CHEST) ? getEquipment(defender->equipment, E_CHEST)->power : 0) : 0);
		int defense = basedefense + armordefense;
		addMessage(messageLog, "%s attacks %s",attacker->name, defender->name);

		damage = roll(power, 6);
		damage -= roll(defense, 6);
		
		damage = damage < 0 ? 0 : damage;

		_improvePlayerSkills();

		takeDamage(messageLog, defender, damage);
	}
}

void takeDamage(messagelist_t* const messageLog, monster_t* const defender, int damage){
	if( damage>0 ){
		addMessage(messageLog, "%s takes %d damage", defender->name, damage);
	} else if (damage < 0){
		addMessage(messageLog, "%s is healed for %d", defender->name, -damage);
	} else {
		addMessage(messageLog, "%s avoids the attack", defender->name, damage);
	}
	defender->combat->hp -= damage;
	if( defender->combat->hp > defender->combat->maxhp ){
		defender->combat->hp = defender->combat->maxhp;
	}
}