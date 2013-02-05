#ifndef _BUFF_H
#define _BUFF_H

struct monster_t;

typedef struct buff_t buff_t;

struct buff_t{
	int      remaining;
	void   (*tick)(struct monster_t*);
	buff_t  *next;
};

buff_t*  createBuff   ( int, void(*)(struct monster_t*) );
void     deleteBuff   ( buff_t* );
void     addBuff      ( struct monster_t*, buff_t* );
void     triggerBuffs ( struct monster_t* );
void     removeBuff   ( struct monster_t*, buff_t* );

/*Temp place for tick functions*/

void healingTick(struct monster_t*);

#endif