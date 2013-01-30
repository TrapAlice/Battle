#include "monsterinit.h"
#include "monster.h"
#include "moonmem.h"
#include "mobdeath.h"
#include "mobbirth.h"


#define mon(id, name, hp, power, defense, exp, birthfunc, attackfunc, deathfunc) MonsterList[id]=createMonster(name,hp,power,defense,exp,birthfunc,attackfunc,deathfunc)

void initMonsters(){
	MonsterList=malloc(sizeof(monster_t)*num_monsters);
	/*  ID           Name       HP   Power  Defense  Exp   BirthFunction   AttackFunction   DeathFunction*/
	mon(mob_null,    "",        0,   0,     0,       0,    NULL,           NULL,            NULL);
	mon(mob_slime,   "Slime",   10,  2,     1,       15,   NULL,           NULL,            NULL);
	mon(mob_pig,     "Pig",     12,  4,     1,       20,   NULL,           NULL,            &pigDeath);
	mon(mob_fairy,   "Fairy",   5,   2,     0,       10,   NULL,           NULL,            &fairyDeath);
	mon(mob_kobold,  "Kobold",  20,  5,     2,       30,   &koboldEquip,   NULL,            NULL);
}

void uninitMonsters(){
	int x;
	for( x = 0; x < num_monsters; x++){
		deleteMonster(MonsterList[x]);
	}
	free(MonsterList);
}