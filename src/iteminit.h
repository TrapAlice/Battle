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
	item_leatherarmor,
	item_meat,
	num_items,
};


void initItems();
void uninitItems();

#endif