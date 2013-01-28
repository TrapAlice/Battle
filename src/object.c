#include "object.h"
#include "libtcod.h"
#include "moonmem.h"
#include "combat.h"

object_t* createObject(char self, int x, int y){
	object_t* obj = malloc(sizeof(object_t));
	obj->self = self;
	obj->x=x;
	obj->y=y;
	return obj;
}

void drawObject(object_t* obj){
	TCOD_console_put_char( NULL, obj->x, obj->y, obj->self, TCOD_BKGND_NONE);
}

void moveObject(object_t* obj, map_t* map, int dx, int dy){
	tile_t* tile = map->mapTiles[obj->x+dx+((obj->y+dy)*map->width)];
	if(!(tile->ops & 2)){
		obj->x+=dx;
		obj->y+=dy;
	}
}

void deleteObject(object_t* object){
	free(object);
}