#include "object.h"
#include "libtcod.h"
#include "moonmem.h"

Object* Object_create(char self){
	Object* obj = malloc(sizeof(Object));
	obj->self = self;
	obj->x=20;
	obj->y=20;
	return obj;
}

void Object_draw(Object* obj){
	TCOD_console_put_char( NULL, obj->x, obj->y, obj->self, TCOD_BKGND_NONE);
}

void Object_move(Object* obj, int dx, int dy){
	obj->x+=dx;
	obj->y+=dy;
}