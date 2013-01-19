#include "monsterinit.h"

Monster** MonsterList;

#define mon(id, name, hp, power, defense, exp) MonsterList[id]=Monster_create(name,hp,power,defense,exp)

void MonstersInit(){
	MonsterList=malloc(sizeof(Monster)*num_monsters);
	mon(mob_null, "", 0, 0, 0, 0);
	mon(mob_slime, "Slime", 10, 2, 1, 15);
	mon(mob_pig, "Pig", 12, 4, 1, 20);
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