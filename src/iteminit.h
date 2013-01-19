#ifndef _ITEMINIT_H
#define _ITEMINIT_H

#include "moonmem.h"
#include "item.h"

extern Item** ItemList;

enum ITEM_ID{
	item_null,
	item_potion,
	item_sword,
	item_pomfhammer,
	num_items,
};


void ItemsInit();
void ItemsGet(enum ITEM_ID);
void ItemsUninit();

#endif