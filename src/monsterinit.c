#include "monsterinit.h"

Monster** MonsterList;

void MonstersInit(){
	MonsterList=malloc(sizeof(Monster)*num_monsters);
	MonsterList[null] = Monster_create("",0);
	MonsterList[mob_slime] = Monster_create("Slime",15);
	MonsterList[mob_pig] = Monster_create("Pig",20);
}

void MonstersGet(enum MON_ID id){
	Monster_clone(MonsterList[id]);
}

void MonstersUninit(){
	int x;
	for( x = 0; x < num_monsters; x++){
		Monster_delete(MonsterList[x]);
	}
	free(MonsterList);
}