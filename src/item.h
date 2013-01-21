#ifndef _ITEM_H
#define _ITEM_H
#include "libtcod.h"

typedef enum {
	I_HEALING,
	I_WEAPON,
	I_ARMOR,
} ItemType;

typedef struct{
	char* name;
	char* desc;
	ItemType type;
	int power;
	int stackable;
}Item;

Item* Item_create(char*, char*, int, int, int);
Item* Item_clone(const Item*);
void Item_delete(Item*);
void Item_description(Item*, TCOD_console_t);

#endif