#ifndef _INVENTORY_H
#define _INVENTORY_H

#include "item.h"

typedef struct inventory_t{
	item_t* item;
	int quantity;
	struct inventory_t* next;
}inventory_t;

inventory_t* createInventory();
void deleteInventory(inventory_t*);
void addItemInventory(inventory_t*, item_t*);
void removeItemInventory(inventory_t*, item_t*);


#endif