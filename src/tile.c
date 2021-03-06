#include "tile.h"
#include "moonmem.h"
#include <assert.h>

tile_t* createTile(char self, char blocked, char blockSight){
	tile_t* tile = malloc(sizeof(tile_t));
	tile->self = self;
	tile->ops |= blocked<<0;
	tile->ops |= blockSight<<1;
	return( tile );
}

void deleteTile(tile_t* const tile){
	free(tile);
}