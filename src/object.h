#ifndef _OBJECT_H
#define _OBJECT_H
#include "combat.h"

typedef struct{
	int x, y;
	char self;
	Combat* combat;
}Object;

Object* Object_create(char);
void Object_draw(Object*);
void Object_move(Object*, int, int);

#endif//_OBJECT_H
