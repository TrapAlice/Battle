#ifndef _COMBAT_H
#define _COMBAT_H

typedef struct{
	int hp, maxhp;
	int defense;
	int power;
}Combat;

void Combat_attack(const Combat*, const char*, Combat*, const char*);
void Combat_takeDamage(Combat*, const char*, int);
Combat* Combat_create(int, int, int);

#endif//_COMBAT_H