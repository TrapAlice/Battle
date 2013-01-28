#ifndef _OBJECT_H
#define _OBJECT_H

#include "map.h"

typedef struct object_t{
	int x, y;
	char self;
}object_t;

object_t* createObject(char, int, int);
void drawObject(object_t*);
void moveObject(object_t*, map_t*, int, int);
void deleteObject(object_t*);

#endif