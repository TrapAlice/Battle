#include "monsterinit.h"
#include "monster.h"
#include "moonmem.h"
#include "mobdeath.h"
#include "mobbirth.h"
#include "mobattack.h"


#define mon(id, name, hp, power, defense, exp, birthfunc, attackfunc, deathfunc) MonsterList[id]=createMonster(name,hp,power,defense,exp,birthfunc,attackfunc,deathfunc)

void initMonsters(){
	MonsterList=malloc(sizeof(monster_t)*num_monsters);
	/*  ID           Name       HP   Power  Defense  Exp   BirthFunction   AttackFunction   DeathFunction*/
	mon(mob_null,    "",        0,   0,     0,       0,    NULL,           NULL,            NULL);
	mon(mob_slime,   "Slime",   10,  1,     1,       15,   NULL,           &standardAttack, &standardDeath);
	mon(mob_pig,     "Pig",     12,  2,     1,       20,   NULL,           &standardAttack, &pigDeath);
	mon(mob_fairy,   "Fairy",   5,   1,     1,       10,   NULL,           &fairyAttack,    &fairyDeath);
	mon(mob_kobold,  "Kobold",  20,  3,     2,       30,   &koboldEquip,   &standardAttack, &standardDeath);
}

void uninitMonsters(){
	int x;
	for( x = 0; x < num_monsters; x++){
		deleteMonster(MonsterList[x]);
	}
	free(MonsterList);
}