#include "iteminit.h"

Item** ItemList;

#define item(id, name, desc, type, type2, power, stackable) ItemList[id]=createItem(name, desc, type, type2, power, stackable)

void initItems(){
	ItemList=malloc(sizeof(Item)*num_items);
	item(item_null, "", "", 0, 0, 0, 0);
	item(item_potion, "Potion", "A simple healing potion", I_HEALING, IS_NONE, 6, 1);
	item(item_sword, "Sword", "A standard sword", I_EQUIPMENT, IS_SWORD, 6, 0);
	item(item_pomfhammer, "Pomf Hammer", "In the name of Rorans, be pomf'd", I_EQUIPMENT, IS_HAMMER, 20, 0);
	item(item_leatherarmor, "Leather Armor", "A piece of leather armor", I_EQUIPMENT, IS_CHESTARMOR, 3, 0);
}

void uninitItems(){
	int x;
	for( x = 0; x < num_items; x++){
		deleteItem(ItemList[x]);
	}
	free(ItemList);
}