#ifndef _OBJECT_H
#define _OBJECT_H

struct map_t;

typedef struct object_t object_t;

struct object_t{
	int     x, y;
	char    self;
	void  (*trigger)(object_t*);
};

object_t*  createObject   ( char, int, int );
void       drawObject     ( const object_t* );
void       moveObject     ( object_t*, const struct map_t*, int, int );
void       deleteObject   ( object_t* );
int        isCollided     ( const object_t*, const object_t* );
void       triggerObject  ( object_t* );
object_t*  createTreasure ( int, int );

#endif