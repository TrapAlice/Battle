#ifndef _INVENTORY_H
#define _INVENTORY_H

#include "item.h"

typedef struct Inventory{
	Item* item;
	int quantity;
	struct Inventory* next;
}Inventory;

Inventory* createInventory();
void deleteInventory(Inventory*);
void addItemInventory(Inventory*, Item*);
void removeItemInventory(Inventory*, Item*);


#endif