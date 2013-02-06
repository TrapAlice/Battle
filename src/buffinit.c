#include "buffinit.h"
#include "buff.h"
#include "buffid.h"
#include "moonmem.h"

#define buff(id,time,tick) BuffList[id]=createBuff(id,time,tick);

void initBuffs(){
	BuffList = malloc(sizeof(buff_t)*num_buffs);
	buff(buff_healing, 5, &healingTick);
}

void uninitBuffs(){
	int x;
	for( x = 0; x < num_buffs; x++ ){
		deleteBuff(BuffList[x]);
	}
	free(BuffList);
}