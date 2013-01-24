#include "monsterinit.h"
#include "mobdeath.h"

Monster** MonsterList;

#define mon(id, name, hp, power, defense, exp, deathfunc) MonsterList[id]=createMonster(name,hp,power,defense,exp,deathfunc)

void initMonsters(){
	MonsterList=malloc(sizeof(Monster)*num_monsters);
	mon(mob_null, "", 0, 0, 0, 0, NULL);
	mon(mob_slime, "Slime", 10, 2, 1, 15, NULL);
	mon(mob_pig, "Pig", 12, 4, 1, 20, &pigDeath);
}

void uninitMonsters(){
	int x;
	for( x = 0; x < num_monsters; x++){
		deleteMonster(MonsterList[x]);
	}
	free(MonsterList);
}