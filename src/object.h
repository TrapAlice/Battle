#ifndef _OBJECT_H
#define _OBJECT_H
#include "combat.h"

typedef struct{
	int x, y;
	char self;
}Object;

Object* Object_create(char, int, int);
void Object_draw(Object*);
void Object_move(Object*, int, int);
void Object_delete(Object*);

#endif//_OBJECT_H
