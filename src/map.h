#ifndef _MAP_H
#define _MAP_H

#include "libtcod.h"

struct object_t;
struct tile_t;

typedef struct map_t{
	int               width;
	int               height;
	struct tile_t   **mapTiles;
	TCOD_map_t       *mapFov;
	struct object_t **objects;
	struct object_t  *stairsup;
	struct object_t  *stairsdown;
}map_t;

map_t*  createMap        ( int, int );
void    deleteMap        ( map_t* );
void    makeMap          ( map_t*, int, int, int, int );
void    createRoom       ( map_t*, int, int, int, int );
void    createVTunnel    ( map_t*, int, int, int );
void    createHTunnel    ( map_t*, int, int, int );
void    renderMap        ( const map_t*, int, int );
void    calculateFov     ( map_t*, const struct object_t* );
void    generateTreasure ( map_t* );
void    removeStairsDown ( map_t* );

#endif