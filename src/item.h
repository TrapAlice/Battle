#ifndef _ITEM_H
#define _ITEM_H
#include "libtcod.h"

enum ItemType{
	I_NONE,
	I_HEALING,
	I_EQUIPMENT
};

enum ItemSubType{
	IS_NONE,
	IS_WEAPON,
	IS_ARMOR,
};

typedef struct{
	char* name;
	char* desc;
	enum ItemType type;
	enum ItemSubType type2;
	int power;
	int stackable;
}Item;

Item* Item_create(char*, char*, int, int, int, int);
Item* Item_clone(const Item*);
void Item_delete(Item*);
void Item_description(Item*, TCOD_console_t);

#endif