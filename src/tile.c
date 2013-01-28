#include "tile.h"
#include "moonmem.h"

tile_t* createTile(char self, int x, int y, char blocked, char blockSight){
	tile_t* tile = malloc(sizeof(tile_t));
	tile->self = self;
	tile->x = x;
	tile->y=y;
	tile->blocked = blocked;
	tile->blockSight = blockSight;
	return tile;
}

void deleteTile(tile_t* tile){
	free(tile);
}