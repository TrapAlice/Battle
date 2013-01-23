#ifndef _OBJECT_H
#define _OBJECT_H

typedef struct{
	int x, y;
	char self;
}Object;

Object* createObject(char, int, int);
void drawObject(Object*);
void moveObject(Object*, int, int);
void deleteObject(Object*);

#endif