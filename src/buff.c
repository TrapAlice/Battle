#include "buff.h"
#include "monster.h"
#include "moonmem.h"
#include "dbg.h"

buff_t** BuffList;

buff_t* createBuff(int id, int turns, void(*tick)(monster_t*)){
	buff_t* buff = malloc(sizeof(buff_t));
	buff->id = id;
	buff->remaining = turns;
	buff->tick = tick;
	return( buff );
}

buff_t* cloneBuff(const buff_t* buff){
	buff_t* clone = malloc(sizeof(buff_t));
	clone->id = buff->id;
	clone->remaining = buff->remaining;
	clone->tick = buff->tick;
	return( clone );
}

void deleteBuff(buff_t* buff){
	free(buff);
}

void addBuff(monster_t* monster, buff_t* buff){
	buff_t* temp;
	if( !monster->buff ){
		monster->buff = buff;
	} else {
		temp = monster->buff;
		while( temp->next ){
			temp = temp->next;
		}
		temp->next = buff;
	}
}

void triggerBuffs(monster_t* monster){
	buff_t* buff = monster->buff;
	if( !buff ) return;
	buff->remaining--;
	if( buff->remaining ){
		if( !buff->tick ) return;
		(buff->tick)(monster);
	} else {
		removeBuff(monster, buff);
	}
}

void removeBuff(monster_t* monster, buff_t* buff){
	buff_t *temp1, *temp2;
	temp1=monster->buff;
	temp2=temp1;
	if( monster->buff == buff ){
		deleteBuff(buff);
		monster->buff = 0;
	}
	while( temp1 != buff ){
		temp2 = temp1;
		temp1 = temp1->next;
	}
	temp2->next = temp1->next;
	deleteBuff(temp1);
}

/*Temp place for tick functions*/

void healingTick(monster_t* monster){
	takeDamage(monster, -2);
}