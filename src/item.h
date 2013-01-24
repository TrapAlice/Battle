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
	IS_SWORD,
	IS_HAMMER,
	IS_CHESTARMOR,
};

typedef struct{
	char* name;
	char* desc;
	enum ItemType type;
	enum ItemSubType type2;
	int power;
	int stackable;
}Item;

Item* createItem(char*, char*, int, int, int, int);
Item* cloneItem(const Item*);
void deleteItem(Item*);
void getItemDescription(Item*, TCOD_console_t);
int itemIsType(Item*, enum ItemType);
int itemIsSubType(Item*, enum ItemSubType);

#endif