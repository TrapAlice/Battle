#include "map.h"
#include "moonmem.h"
#include "rng.h"
#include "libtcod.h"
#include "object.h"
#include "tile.h"

typedef struct rect_t{
	int x,y,w,h;
}rect_t;

rect_t rect(int x, int y, int w, int h){
	rect_t r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	return( r );
}

int rectIntersect(rect_t a, rect_t b){
	return( a.x <= b.x+b.w && a.x+a.w >= b.x &&
	        a.y <= b.y+b.h && b.y+b.h >= b.y );
}

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
	int i;
	int numrooms;
	int newroom=1;
	int totalrooms=0;
	int attempt;
	rect_t rooms[maxrooms];
	
	for( y=0; y<map->height; ++y ){
		for( x=0; x<map->width; ++x ){
			_fillTile(map, x, y);	
		}
	}
	for( numrooms=0; numrooms < maxrooms; numrooms++ ){
		newroom=1;
		attempt = 0;
		w = between(minsize, maxsize);
		h = between(minsize, maxsize);
		x = between(1, map->width-w-1);
		y = between(1, map->height-h-1);
		rect_t room = rect(x,y,w,h);
		if( checkIntersect ){
			while( attempt < 5 ){
				for( i=0; i<numrooms; ++i ){
					if( rectIntersect(rooms[i], room) ){
						newroom=0;
					}
				}
				if( !newroom ){
					w = between(minsize, maxsize);
					h = between(minsize, maxsize);
					x = between(1, map->width-w-1);
					y = between(1, map->height-h-1);
					room = rect(x,y,w,h);
					attempt++;
				} else {
					break;
				}
			}
		}
		
		if( newroom ){
			
			rooms[totalrooms] = room;
			
			totalrooms++;
			createRoom(map, x,y,w,h);

			

			if( numrooms ){
				createVTunnel(map, y2, y, x2);
				createHTunnel(map, x2, x, y);
			}

			x2 = between(x, x+w);
			y2 = between(y, y+h);
		}
	}

	rect_t room0 = rooms[0];
	map->stairsup = createObject('<', between(room0.x,room0.x+room0.w), between(room0.y,room0.y+room0.h));
	

	rect_t room1 = rooms[totalrooms-1];
	map->stairsdown = createObject('>', between(room1.x,room1.x+room1.w), between(room1.y,room1.y+room1.h));
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
		if( map->stairsup ){
			if( TCOD_map_is_in_fov(map->mapFov, map->stairsup->x, map->stairsup->y) ){
					TCOD_console_put_char( NULL, map->stairsup->x-centerx+40, map->stairsup->y-centery+15, map->stairsup->self, TCOD_BKGND_NONE);
				}
		}
		if( map->stairsdown ){
			if( TCOD_map_is_in_fov(map->mapFov, map->stairsdown->x, map->stairsdown->y) ){
					TCOD_console_put_char( NULL, map->stairsdown->x-centerx+40, map->stairsdown->y-centery+15, map->stairsdown->self, TCOD_BKGND_NONE);
				}
		}
	}
}

void calculateFov(map_t* const map, const object_t* const player){
	TCOD_map_compute_fov(map->mapFov, player->x, player->y, 
	                     0, 1, FOV_BASIC);
}