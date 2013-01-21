#ifndef _COMBAT_H
#define _COMBAT_H

#include "msg.h"

typedef struct{
	int hp, maxhp;
	int defense;
	int power;
	int hits;
}Combat;

void Combat_takeDamage(MessageList*, Combat*, const char*, int);
Combat* Combat_create(int, int, int);
void Combat_delete(Combat*);

#endif