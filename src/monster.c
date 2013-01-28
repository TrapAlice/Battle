#include "monster.h"
#include "moonmem.h"
#include "rng.h"

Monster* player;

Monster* createPlayer(int x, int y){
	Object* obj = createObject('@',x,y);
	Monster* monster = malloc(sizeof(Monster));
	monster->name = "Player";
	monster->combat = createCombat(32,6,1);
	monster->object = obj;
	monster->inventory = createInventory();
	monster->equipment = createEquipmentSlots();
	monster->skills = createSkillSlots();
	return monster;
}

Monster* createMonster(char* name, int hp, int power, int defense, int xp, void(*mobdeath)(Monster*)){
	Monster* monster = malloc(sizeof(Monster));
	monster->name = name;
	monster->combat = createCombat(hp,power,defense);
	monster->xp = xp;
	monster->inventory = createInventory();
	monster->deathFunction = mobdeath;
	return monster;
}

Monster* cloneMonster(Monster* monster){
	Monster* clone = malloc(sizeof(Monster));
	clone->name = monster->name;
	clone->xp = monster->xp;
	clone->combat = createCombat(monster->combat->maxhp, monster->combat->power, monster->combat->defense);
	clone->deathFunction = monster->deathFunction;
	return clone;
}

int checkDead(Monster* monster){
	int dead = ( monster->combat->hp < 0  ? 1 : 0 );
	return dead;
}

void deleteMonster(Monster* monster){
	if( monster->object != NULL) deleteObject(monster->object);
	if( monster->inventory != NULL) deleteInventory(monster->inventory);
	if( monster->combat != NULL) deleteCombat(monster->combat);
	if( monster->equipment != NULL) deleteEquipmentSlots(monster->equipment);
	if( monster->skills != NULL ) deleteSkillSlots(monster->skills);
	free(monster);
}

static void _improvePlayerSkills(){
	if(player->equipment->equipped[E_Hand] != NULL){
			increaseSkill(player->skills, player->equipment->equipped[E_Hand]->relatedSkill, 1);
		}
}

void attackMonster(MessageList* messageLog, Monster* attacker, Monster* defender){
	int damage;
	int basepower = attacker->combat->power;
	int weaponpower = (attacker->equipment==NULL ? 0 : (getEquipment(attacker->equipment, E_Hand) == NULL ? 0 : getEquipment(attacker->equipment, E_Hand)->power));
	int basedefense = defender->combat->defense;
	int armordefense = (defender->equipment==NULL ? 0 : (getEquipment(defender->equipment, E_Chest) == NULL ? 0 : getEquipment(defender->equipment, E_Chest)->power));
	addMessage(messageLog, "%s attacks %s",attacker->name, defender->name);

	damage = roll(attacker->combat->hits, basepower);
	damage += (weaponpower < 0 ? 0 : roll(1, weaponpower));
	damage -= basedefense;
	damage -= roll(1,armordefense);
	
	damage = damage < 0 ? 0 : damage;

	if(attacker == player){
		_improvePlayerSkills();
	}

	takeDamage(messageLog, defender, damage);
}

void takeDamage(MessageList* messageLog, Monster* defender, int damage){
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