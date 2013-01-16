#include "monster.h"
#include "moonmem.h"

Monster* Monster_playerCreate(int x, int y){
	Object* obj = Object_create('@',x,y);
	Monster* monster = malloc(sizeof(Monster));
	monster->name = "Player";
	monster->combat = Combat_create(32,6,1);
	monster->object = obj;
	return monster;
}

Monster* Monster_create(char* name){
	Monster* monster = malloc(sizeof(Monster));
	monster->name = name;
	monster->combat = Combat_create(16,4,1);
	return monster;
}

int Monster_checkDead(Monster* monster){
	return ( monster->combat->hp < 0 ) ? 1 : 0;
}

void Monster_delete(Monster* monster){
	if( monster->object != NULL) Object_delete(monster->object);
	if( monster->combat != NULL) Combat_delete(monster->combat);
	free(monster);
}