#ifndef _MAP_H
#define _MAP_H

#include "tile.h"

typedef struct map_t{
	int width;
	int height;
	tile_t** mapTiles;
}map_t;

map_t* createMap(int, int);
void deleteMap(map_t*);
void makeMap(map_t*, int, int, int, int);
void createRoom(map_t*, int, int, int, int);
void createVTunnel(map_t*, int, int, int);
void createHTunnel(map_t*, int, int, int);
void renderMap(map_t*, int, int);

#endif