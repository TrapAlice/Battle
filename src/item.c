#include "item.h"
#include "moonmem.h"

Item* Item_create(char* name, int type, int power, int stackable){
	Item* item = malloc(sizeof(Item));
	item->name = name;
	item->type = type;
	item->power = power;
	item->stackable = stackable;
	
	return item;
}

Item* Item_clone(const Item* item){
	Item* clone = malloc(sizeof(Item));
	clone->name = item->name;
	clone->type = item->type;
	clone->power = item->power;
	clone->stackable = item->stackable;

	return clone;
}

void Item_delete(Item* item){
	free(item);
}