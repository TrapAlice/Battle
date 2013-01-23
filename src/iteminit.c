#include "iteminit.h"

Item** ItemList;

#define item(id, name, desc, type, type2, power, stackable) ItemList[id]=Item_create(name, desc, type, type2, power, stackable)

void ItemsInit(){
	ItemList=malloc(sizeof(Item)*num_items);
	item(item_null, "", "", 0, 0, 0, 0);
	item(item_potion, "Potion", "A simple healing potion", I_HEALING, IS_NONE, 6, 1);
	item(item_sword, "Sword", "A standard sword", I_EQUIPMENT, IS_WEAPON, 6, 0);
	item(item_pomfhammer, "Pomf Hammer", "In the name of Rorans, be pomf'd", I_EQUIPMENT, IS_WEAPON, 20, 0);
	item(item_leatherarmor, "Leather Armor", "A piece of leather armor", I_EQUIPMENT, IS_ARMOR, 3, 0);
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