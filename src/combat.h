#ifndef _COMBAT_H
#define _COMBAT_H

typedef struct{
	int hp, maxhp;
	int defense;
	int power;
}Combat;

void Combat_attack(Combat*, Combat*);
void Combat_takeDamage(Combat*, int);
Combat* Combat_create(int, int, int);

#endif//_COMBAT_H