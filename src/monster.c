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
	if( player->equipment->equipped[E_RHAND] ){
		if( isSkillActive(player->skills, getEquipment(player->equipment, E_RHAND)->relatedSkill) ){
			increaseSkill(player->skills, getEquipment(player->equipment, E_RHAND)->relatedSkill, 3);
		}
	}
}

void attackMonster(messagelist_t* const messageLog, monster_t* const attacker, monster_t* const defender){
	if( attacker->attackFunction ){
		(attacker->attackFunction)(attacker, defender);
	} else {
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

		if( weaponpower ){
			weaponpower += getSkillLevelifActive(attacker->skills, weapon->relatedSkill)/3;
			damagemod += getSkillLevelifActive(attacker->skills, weapon->relatedSkill) + weapon->damageBonus;
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
			addMessage(messageLog, "You attack the %s", defender->name);
		} else {
			addMessage(messageLog, "The %s attacks you", attacker->name);
		}
		
		damage = roll(power, 6)+damagemod;
		damage -= roll(defense, 6);
		
		damage = damage < 0 ? 0 : damage;

		_improvePlayerSkills();

		takeDamage(messageLog, defender, damage);
	}
}

void takeDamage(messagelist_t* const messageLog, monster_t* const defender, int damage){
	if( damage>0 ){
		if( defender == player ){
			addMessage(messageLog, "You take %d damage", damage);
		} else {
			addMessage(messageLog, "The %s takes %d damage", defender->name, damage);
		}
	} else if (damage < 0){
		if( defender == player ){
			addMessage(messageLog, "You are healed for %d", -damage);
		} else {
			addMessage(messageLog, "The %s is healed for %d", defender->name, -damage);
		}
	} else {
		if( defender == player ){
			addMessage(messageLog, "You avoid the attack", defender->name);
		} else {
			addMessage(messageLog, "The %s avoids the attack", defender->name);
		}
		
	}
	defender->combat->hp -= damage;
	if( defender->combat->hp > defender->combat->maxhp ){
		defender->combat->hp = defender->combat->maxhp;
	}
}