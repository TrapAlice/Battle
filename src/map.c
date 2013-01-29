#include "map.h"
#include "moonmem.h"
#include "rng.h"
#include "libtcod.h"

map_t* createMap(int width, int height){
	map_t* map = malloc(sizeof(map_t));
	map->width = width;
	map->height = height;
	map->mapTiles = malloc((sizeof(tile_t))*width*height);
	map->mapFov = TCOD_map_new(width, height);
	return map;
}

void deleteMap(map_t* map){
	int x, y;
	for(x=0; x<map->width; x++){
		for(y=0; y<map->height; y++){
			deleteTile(map->mapTiles[x+(y*map->width)]);
		}
	}
	free(map->mapTiles);
	TCOD_map_delete(map->mapFov);
	free(map);
}

void makeMap(map_t* map, int maxrooms, int minsize, int maxsize, int checkIntersect){
	int x, y;
	int w, h;
	int x2 =0, y2 =0;
	int numrooms;
	
	for(y=0; y<map->height; y++){
		for(x=0; x<map->width; x++){
			map->mapTiles[x+(y*map->width)] = createTile('~', 1, 1);
		}
	}

	for(numrooms=0; numrooms < maxrooms; numrooms++){
		w = between(minsize, maxsize);
		h = between(minsize, maxsize);
		x = between(1, map->width-w-1);
		y = between(1, map->height-h-1);


		createRoom(map, x,y,w,h);

		if(numrooms){
			createVTunnel(map, y2, y, x2);
			createHTunnel(map, x2, x, y);
		}

		x2 = x;
		y2 = y;
	}
}

static void _digTile(map_t* map, int x, int y){
	tile_t* tile;
	tile = map->mapTiles[x+(y*map->width)];
	tile->ops = tile->ops & 1<<0;
	tile->ops = tile->ops & 1<<1;
	tile->self='.';
	TCOD_map_set_properties(map->mapFov, x,y, 1,1);
}

void createRoom(map_t* map, int x, int y, int w, int h){
	int i, j;
	for(j=y;j<y+h;j++){
		for(i=x;i<x+w;i++){
			_digTile(map,i,j);
		}
	}
}

void createVTunnel(map_t* map, int y1, int y2, int x){
	int ymin = y1 < y2 ? y1 : y2;
	int ymax = y1 < y2 ? y2+1 : y1+1;
	for(;ymin<ymax;ymin++){
		_digTile(map,x,ymin);
	}
}

void createHTunnel(map_t* map, int x1, int x2, int y){
	int xmin = x1 < x2 ? x1 : x2;
	int xmax = x1 < x2 ? x2+1 : x1+1;
	for(;xmin<xmax;xmin++){
		_digTile(map,xmin,y);
	}
}

void renderMap(map_t* map, int centerx, int centery){
	tile_t *tile;
	int x, y;
	for(y=0; y<map->height; y++){
		for(x=0; x<map->width; x++){
        	if(y-centery+15>32) break;
        	if(TCOD_map_is_in_fov(map->mapFov,x,y)){
	            tile = map->mapTiles[x+(y*map->width)];
	            TCOD_console_put_char( NULL, x-centerx+40, y-centery+15, tile->self, TCOD_BKGND_NONE);
	        }
        }
    }
}