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

typedef struct item_t{
	char* name;
	char* desc;
	enum ItemType type;
	enum ItemSubType type2;
	enum Skills_e relatedSkill;
	int power;
	int stackable;
}item_t;

item_t* createItem(char*, char*, int, int, int, int, int);
item_t* cloneItem(const item_t*);
void deleteItem(item_t*);
void getItemDescription(item_t*, TCOD_console_t);
int itemIsType(item_t*, enum ItemType);
int itemIsSubType(item_t*, enum ItemSubType);

#endif