#include "map.h"
#include "moonmem.h"

map_t* createMap(int width, int height){
	map_t* map = malloc(sizeof(map_t));
	map->width = width;
	map->height = height;
	map->mapTiles = malloc(sizeof(tile_t)*width*height);
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
	free(map);
}

void makeMap(map_t* map, int maxrooms, int minsize, int maxsize, int checkIntersect){
	int x, y;
	int w, h;
	int x2, y2;
	int numrooms = 0;
	
	for(x=0; x<map->width; x++){
		for(y=0; y<map->height; y++){
			map->mapTiles[x+(y*map->width)] = createTile('#', x, y, 0, 0);
		}
	}

	for(numrooms=0; numrooms < maxrooms; numrooms++){
		x = rand() % map->width;
		y = rand() % map->height;
		w = (rand() % (maxsize-minsize))+minsize;
		h = (rand() % (maxsize-minsize))+minsize;

		x = x+w>map->width ? map->width-x : x;
		y = y+w>map->height ? map->height-y : y;

		createRoom(map, x,y,w,h);

		if(numrooms){
			createVTunnel(map, y2, y, x2);
			createHTunnel(map, x2, x, y);
		}

		x2 = x;
		y2 = y;
	}

	

}

void createRoom(map_t* map, int x, int y, int w, int h){
	tile_t* tile;
	int i, j;
	for(i=x;i<x+w;i++){
		for(j=y;j<y+h;j++){
			tile = map->mapTiles[i+(j*map->width)];
			tile->blocked=1;
			tile->blockSight=1;
			tile->object->self='.';
		}
	}
}

void createVTunnel(map_t* map, int y1, int y2, int x){
	tile_t* tile;
	int ymin = y1 < y2 ? y1 : y2;
	int ymax = y1 < y2 ? y2+1 : y1+1;
	for(;ymin<ymax;ymin++){
		tile = map->mapTiles[x+(ymin*map->width)];
		tile->blocked=1;
		tile->blockSight=1;
		tile->object->self='.';
	}
}

void createHTunnel(map_t* map, int x1, int x2, int y){
	tile_t* tile;
	int xmin = x1 < x2 ? x1 : x2;
	int xmax = x1 < x2 ? x2+1 : x1+1;
	for(;xmin<xmax;xmin++){
		tile = map->mapTiles[xmin+(y*map->width)];
		tile->blocked=1;
		tile->blockSight=1;
		tile->object->self='.';
	}
}