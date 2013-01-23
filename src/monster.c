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
	return monster;
}

Monster* createMonster(char* name, int hp, int power, int defense, int xp){
	Monster* monster = malloc(sizeof(Monster));
	monster->name = name;
	monster->combat = createCombat(hp,power,defense);
	monster->xp = xp;
	monster->inventory = createInventory();
	return monster;
}

Monster* cloneMonster(Monster* monster){
	Monster* clone = malloc(sizeof(Monster));
	clone->name = monster->name;
	clone->xp = monster->xp;
	clone->combat = createCombat(monster->combat->maxhp, monster->combat->power, monster->combat->defense);
	return clone;
}

int checkDead(Monster* monster){
	return ( monster->combat->hp < 0 ) ? 1 : 0;
}

void deleteMonster(Monster* monster){
	if( monster->object != NULL) deleteObject(monster->object);
	if( monster->inventory != NULL) deleteInventory(monster->inventory);
	if( monster->combat != NULL) deleteCombat(monster->combat);
	if( monster->equipment != NULL) deleteEquipmentSlots(monster->equipment);
	free(monster);
}

void attackMonster(MessageList* messageLog, Monster* attacker, Monster* defender){
	int damage;
	addMessage(messageLog, "%s attacks %s",attacker->name, defender->name);
	if(attacker->equipment==NULL){
		damage = roll(attacker->combat->hits, attacker->combat->power);
	} else {
		if(attacker->equipment->equipped[E_Hand]!=NULL){
			damage = roll(attacker->combat->hits, attacker->equipment->equipped[E_Hand]->power);
		} else {
			damage = roll(attacker->combat->hits, attacker->combat->power);
		}
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