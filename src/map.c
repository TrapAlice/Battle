#include "map.h"
#include "moonmem.h"
#include "rng.h"
#include "libtcod.h"
#include "object.h"
#include "tile.h"

map_t* createMap(int width, int height){
	int x,y;
	map_t* map = malloc(sizeof(map_t));
	map->width = width;
	map->height = height;
	map->mapTiles = malloc((sizeof(tile_t))*width*height);
	map->mapFov = TCOD_map_new(width, height);
	map->objects = malloc(sizeof(object_t)*5);

	for( y=0; y<map->height; ++y ){
		for( x=0; x<map->width; ++x ){
			map->mapTiles[x+(y*map->width)] = createTile('#', 1, 1);
		}
	}

	return( map );
}

void deleteMap(map_t* const map){
	int x, y;
	for( x=0; x<map->width; ++x ){
		for( y=0; y<map->height; ++y ){
			deleteTile(map->mapTiles[x+(y*map->width)]);
		}
	}
	for( x=0; x<5; ++x ){
		if(map->objects[x]){
			deleteObject(map->objects[x]);
		}
	}
	free(map->objects);
	free(map->mapTiles);
	TCOD_map_delete(map->mapFov);
	free(map);
}

static void _fillTile(map_t* const  map, int x, int y){
	tile_t* tile;
	tile = map->mapTiles[x+(y*map->width)];
	tile->ops |= 1<<0;
	tile->ops |= 1<<1;
	tile->self='#';
	TCOD_map_set_properties(map->mapFov, x,y, 0,0);
}

static void _digTile(map_t* const map, int x, int y){
	tile_t* tile;
	tile = map->mapTiles[x+(y*map->width)];
	tile->ops &= 0<<0;
	tile->ops &= 0<<1;
	tile->self='.';
	TCOD_map_set_properties(map->mapFov, x,y, 1,1);
}

void makeMap(map_t* const map, int maxrooms, int minsize, int maxsize, int checkIntersect){
	int x, y;
	int w, h;
	int x2 =0, y2 =0;
	int numrooms;
	int downstairs =0;
	int upstairs=0;
	
	for( y=0; y<map->height; ++y ){
		for( x=0; x<map->width; ++x ){
			_fillTile(map, x, y);	
		}
	}

	for( numrooms=0; numrooms < maxrooms; numrooms++ ){
		w = between(minsize, maxsize);
		h = between(minsize, maxsize);
		x = between(1, map->width-w-1);
		y = between(1, map->height-h-1);


		createRoom(map, x,y,w,h);

		if( !downstairs ){
			downstairs=1;
			map->objects[0] = createObject('>', between(x,x+w), between(y,y+h));
		}

		if( !upstairs && numrooms == maxrooms-1 ){
			upstairs = 1;
			map->objects[1] = createObject('<', between(x,x+w), between(y,y+h));
		}

		if( numrooms ){
			createVTunnel(map, y2, y, x2);
			createHTunnel(map, x2, x, y);
		}

		x2 = x;
		y2 = y;
	}
}



void createRoom(map_t* const map, int x, int y, int w, int h){
	int i, j;
	for( j=y; j<y+h; j++ ){
		for( i=x; i<x+w; i++ ){
			_digTile(map,i,j);
		}
	}
}

void createVTunnel(map_t* const map, int y1, int y2, int x){
	int ymin = y1 < y2 ? y1 : y2;
	int ymax = y1 < y2 ? y2+1 : y1+1;
	for( ; ymin<ymax; ++ymin ){
		_digTile(map,x,ymin);
	}
}

void createHTunnel(map_t* const map, int x1, int x2, int y){
	int xmin = x1 < x2 ? x1 : x2;
	int xmax = x1 < x2 ? x2+1 : x1+1;
	for( ; xmin<xmax; ++xmin ){
		_digTile(map,xmin,y);
	}
}

void renderMap(const map_t* const map, int centerx, int centery){
	tile_t *tile;
	int x, y;
	int i;
	object_t *object;
	for( y=0; y<map->height; ++y ){
		if( y-centery+15>32 ) break;
		for( x=0; x<map->width; ++x ){
			if( TCOD_map_is_in_fov(map->mapFov,x,y) ){
				tile = map->mapTiles[x+(y*map->width)];
				TCOD_console_put_char( NULL, x-centerx+40, y-centery+15, tile->self, TCOD_BKGND_NONE);
				for( i=0; i<5; i++ ){
					object = map->objects[i];
					if( object ){
						if( TCOD_map_is_in_fov(map->mapFov, object->x, object->y) ){
							TCOD_console_put_char( NULL, object->x-centerx+40, object->y-centery+15, object->self, TCOD_BKGND_NONE);
						}
					}
				}
			}
		}
	}
}

void calculateFov(map_t* const map, const object_t* const player){
	TCOD_map_compute_fov(map->mapFov, player->x, player->y, 
	                     0, 1, FOV_BASIC);
}