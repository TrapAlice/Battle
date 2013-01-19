#ifndef _INVENTORY_H
#define _INVENTORY_H

#include "item.h"

typedef struct Inventory{
	Item* item;
	int quantity;
	struct Inventory* next;
}Inventory;

Inventory* Inventory_create();
Inventory* Inventory_add(Item*);
void Inventory_delete(Inventory*);
void Inventory_addItem(Inventory*, Item*);
void Inventory_removeItem(Inventory*, Item*);


#endif