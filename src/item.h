#ifndef _ITEM_H
#define _ITEM_H
#include "libtcod.h"
#include "skills.h"

enum ItemType{
	I_NONE,
	I_HEALING,
	I_EQUIPMENT
};

enum ItemSubType{
	IS_NONE,
	IS_WEAPON,
	IS_CHESTARMOR,
};

typedef struct{
	char* name;
	char* desc;
	enum ItemType type;
	enum ItemSubType type2;
	enum Skills_e relatedSkill;
	int power;
	int stackable;
}Item;

Item* createItem(char*, char*, int, int, int, int, int);
Item* cloneItem(const Item*);
void deleteItem(Item*);
void getItemDescription(Item*, TCOD_console_t);
int itemIsType(Item*, enum ItemType);
int itemIsSubType(Item*, enum ItemSubType);

#endif