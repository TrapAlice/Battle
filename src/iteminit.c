#include "iteminit.h"

Item** ItemList;

#define item(id, name, type, power, stackable) ItemList[id]=Item_create(name, type, power, stackable)

void ItemsInit(){
	ItemList=malloc(sizeof(Item)*num_items);
	item(item_null, "", 0, 0, 0);
	item(item_potion, "Potion", 1, 6, 1);
	item(item_sword, "Sword", 2, 6, 0);
}

void ItemsGet(enum ITEM_ID id){
	Item_clone(ItemList[id]);
}

void ItemsUninit(){
	int x;
	for( x = 0; x < num_items; x++){
		Item_delete(ItemList[x]);
	}
	free(ItemList);
}