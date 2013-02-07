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
#include "buff.h"
#include "mobfunctions.h"

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

monster_t* createMonster(int id, const char* const name, int hp, int power, int defense, int xp, 
  void(*mobbirth)(monster_t*), void(*mobattack)(monster_t*,monster_t*), void(*mobdeath)(const monster_t*)){
	monster_t* monster = malloc(sizeof(monster_t));
	monster->id = id;
	monster->name = name;
	monster->combat = createCombat(hp,power,defense);
	monster->xp = xp;
	monster->birthFunction = mobbirth;
	monster->attackFunction = mobattack;
	monster->deathFunction = mobdeath;
	return( monster );
}

monster_t* cloneMonster(const monster_t* const monster){
	monster_t* clone = malloc(sizeof(monster_t));
	clone->id = monster->id;
	clone->name = monster->name;
	clone->xp = monster->xp;
	clone->combat = cloneCombat(monster->combat);
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
	if( monster->buff ) deleteBuff(monster->buff);
	free(monster);
}

static void _improvePlayerSkills(){
	if( player->equipment->equipped[E_RHAND] ){
		increaseSkillifActive(player->skills, getEquipment(player->equipment, E_RHAND)->relatedSkill, 3);
	}
}

void attackMonster(monster_t* const attacker, monster_t* const defender){
	if( attacker->attackFunction ){
		(attacker->attackFunction)(attacker, defender);
	} else {
		standardAttack(attacker, defender);
		if( attacker == player ){
			_improvePlayerSkills();
		}
	}
}

void takeDamage(monster_t* const defender, int damage){
	if( damage>0 ){
		if( defender == player ){
			addMessage(globalMessage, "You take %d damage", damage);
		} else {
			addMessage(globalMessage, "The %s takes %d damage", defender->name, damage);
		}
	} else if (damage < 0){
		if( defender == player ){
			addMessage(globalMessage, "You are healed for %d", -damage);
		} else {
			addMessage(globalMessage, "The %s is healed for %d", defender->name, -damage);
		}
	} else {
		if( defender == player ){
			addMessage(globalMessage, "You avoid the attack", defender->name);
		} else {
			addMessage(globalMessage, "The %s avoids the attack", defender->name);
		}
		
	}
	defender->combat->hp -= damage;
	if( defender->combat->hp > defender->combat->maxhp ){
		defender->combat->hp = defender->combat->maxhp;
	}
}