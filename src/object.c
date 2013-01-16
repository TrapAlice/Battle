#include "object.h"
#include "libtcod.h"
#include "moonmem.h"
#include "combat.h"

Object* Object_create(char self, int x, int y){
	Object* obj = malloc(sizeof(Object));
	obj->self = self;
	obj->x=x;
	obj->y=y;
	return obj;
}

void Object_draw(Object* obj){
	TCOD_console_put_char( NULL, obj->x, obj->y, obj->self, TCOD_BKGND_NONE);
}

void Object_move(Object* obj, int dx, int dy){
	obj->x+=dx;
	obj->y+=dy;
}

void Object_delete(Object* object){
	free(object);
}