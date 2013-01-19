#ifndef _ITEM_H
#define _ITEM_H

typedef enum {
	I_HEALING,
	I_WEAPON,
} ItemType;

typedef struct{
	char* name;
	ItemType type;
	int power;
	int stackable;
}Item;

Item* Item_create(char*, int, int, int);
Item* Item_clone(const Item*);
void Item_delete(Item*);

#endif