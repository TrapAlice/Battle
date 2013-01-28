#ifndef _TILE_H
#define _TILE_H

#include "object.h"

typedef struct tile_t{
	object_t* object;
	char blocked;
	char blockSight;
	char visible;
	char explored;
}tile_t;

tile_t* createTile(char, int, int, char, char);
void deleteTile(tile_t*);

#endif