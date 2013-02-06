#include "object.h"
#include "libtcod.h"
#include "moonmem.h"
#include "combat.h"
#include "map.h"
#include "tile.h"
#include "triggerfunctions.h"

object_t* createObject(char self, int x, int y){
	object_t* obj = malloc(sizeof(object_t));
	obj->self = self;
	obj->x=x;
	obj->y=y;
	return( obj );
}

void drawObject(const object_t* const obj){
	TCOD_console_put_char( NULL, obj->x, obj->y, obj->self, TCOD_BKGND_NONE);
}

void moveObject(object_t* const obj, const map_t* const map, int dx, int dy){
	tile_t* tile = map->mapTiles[obj->x+dx+((obj->y+dy)*map->width)];
	if( !(tile->ops & 2) ){
		obj->x+=dx;
		obj->y+=dy;
	}
}

void deleteObject(object_t* const object){
	free(object);
}

int isCollided(const object_t* const a, const object_t* const b){
	checknotnull(a);
	checknotnull(b);
	if( a->x == b->x ){
		if( a->y == b->y ){
			return( 1 );
		}
	}
	return( 0 );
}

void triggerObject(object_t* const object){
	if( !object->trigger ) return;
	(object->trigger)(object);
}

object_t* createTreasure(int x, int y){
	object_t* chest = createObject('T',x,y);
	chest->trigger = &openChest;
	return chest;
}