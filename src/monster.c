#include "monster.h"
#include "moonmem.h"
#include "rng.h"

Monster* player;

Monster* Monster_playerCreate(int x, int y){
	Object* obj = Object_create('@',x,y);
	Monster* monster = malloc(sizeof(Monster));
	monster->name = "Player";
	monster->combat = Combat_create(32,6,1);
	monster->object = obj;
	monster->inventory = Inventory_create();
	monster->equipment = Equipment_slotsCreate();
	return monster;
}

Monster* Monster_create(char* name, int hp, int power, int defense, int xp){
	Monster* monster = malloc(sizeof(Monster));
	monster->name = name;
	monster->combat = Combat_create(hp,power,defense);
	monster->xp = xp;
	monster->inventory = Inventory_create();
	return monster;
}

Monster* Monster_clone(Monster* monster){
	Monster* clone = malloc(sizeof(Monster));
	clone->name = monster->name;
	clone->xp = monster->xp;
	clone->combat = Combat_create(monster->combat->maxhp, monster->combat->power, monster->combat->defense);
	return clone;
}

int Monster_checkDead(Monster* monster){
	return ( monster->combat->hp < 0 ) ? 1 : 0;
}

void Monster_delete(Monster* monster){
	if( monster->object != NULL) Object_delete(monster->object);
	if( monster->inventory != NULL) Inventory_delete(monster->inventory);
	if( monster->combat != NULL) Combat_delete(monster->combat);
	if( monster->equipment != NULL) Equipment_delete(monster->equipment);
	free(monster);
}

void Monster_attack(MessageList* messageLog, Monster* attacker, Monster* defender){
	int damage;
	Msg_addMessage(messageLog, "%s attacks %s",attacker->name, defender->name);
	if(attacker->equipment==NULL){
		damage = RNG_roll(attacker->combat->hits, attacker->combat->power);
	} else {
		if(attacker->equipment->equipped[E_Hand]!=NULL){
			damage = RNG_roll(attacker->combat->hits, attacker->equipment->equipped[E_Hand]->power);
		} else {
			damage = RNG_roll(attacker->combat->hits, attacker->combat->power);
		}
	}
	Monster_takeDamage(messageLog, defender, damage);
}

void Monster_takeDamage(MessageList* messageLog, Monster* defender, int damage){
	if(damage>0){
		Msg_addMessage(messageLog, "%s takes %d damage", defender->name, damage);
	} else if (damage < 0){
		Msg_addMessage(messageLog, "%s is healed for %d", defender->name, -damage);
	} else {
		Msg_addMessage(messageLog, "%s avoids the attack", defender->name, damage);
	}
	defender->combat->hp -= damage;
	if(defender->combat->hp > defender->combat->maxhp){
		defender->combat->hp = defender->combat->maxhp;
	}
}