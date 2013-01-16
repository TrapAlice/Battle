#include "monster.h"
#include "moonmem.h"

Monster* Monster_create(char* name){
	Monster* Monster = malloc(sizeof(Monster));
	Monster->name = name;
	Monster->combat = Combat_create(16,2,1);
	return Monster;
}

int Monster_checkDead(Monster* monster){
	return ( monster->combat->hp < 0 ) ? 1 : 0;
}