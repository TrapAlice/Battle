#include "monster.h"
#include "moonmem.h"

Monster* player;

Monster* Monster_playerCreate(int x, int y){
	Object* obj = Object_create('@',x,y);
	Monster* monster = malloc(sizeof(Monster));
	monster->name = "Player";
	monster->combat = Combat_create(32,6,1);
	monster->object = obj;
	monster->inventory = Inventory_create();
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
	free(monster);
}