#ifndef _OBJECT_H
#define _OBJECT_H

struct map_t;

typedef struct object_t{
	int x, y;
	char self;
}object_t;

object_t* createObject(char, int, int);
void drawObject(object_t*);
void moveObject(object_t*, struct map_t*, int, int);
void deleteObject(object_t*);
int isCollided(object_t*, object_t*);

#endif