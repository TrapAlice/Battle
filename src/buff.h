#ifndef _BUFF_H
#define _BUFF_H

#include "buffid.h"

struct monster_t;

typedef struct buff_t buff_t;

struct buff_t{
	int      id;
	int      remaining;
	void   (*tick)(struct monster_t*);
	buff_t  *next;
};

buff_t*  createBuff   ( int, int, void(*)(struct monster_t*) );
buff_t*  cloneBuff    ( const buff_t* );
void     deleteBuff   ( buff_t* );
void     addBuff      ( struct monster_t*, buff_t* );
void     triggerBuffs ( struct monster_t* );
void     removeBuff   ( struct monster_t*, buff_t* );

extern buff_t** BuffList;

/*Temp place for tick functions*/

void healingTick(struct monster_t*);

#endif