#include "iteminit.h"
#include "skills.h"

item_t** ItemList;

#define item(id, name, desc, type, type2, skill, power, stackable) ItemList[id]=createItem(name, desc, type, type2, skill, power, stackable)

void initItems(){
	ItemList=malloc(sizeof(item_t)*num_items);
	item(item_null, "", "", 0, 0, 0, 0, 0);
	item(item_potion, "Potion", "A simple healing potion", I_HEALING, IS_NONE, SKILL_NONE, 6, 1);
	item(item_sword, "Sword", "A standard sword", I_EQUIPMENT, IS_WEAPON, SKILL_SWORD, 6, 0);
	item(item_pomfhammer, "Pomf Hammer", "In the name of Rorans, be pomf'd", I_EQUIPMENT, IS_WEAPON, SKILL_HAMMER, 20, 0);
	item(item_leatherarmor, "Leather Armor", "A piece of leather armor", I_EQUIPMENT, IS_CHESTARMOR, SKILL_NONE, 3, 0);
	item(item_meat, "Chunk of meat", "A chunk of meat carved from a piggie", I_HEALING, IS_NONE, SKILL_NONE, 5, 1);
}

void uninitItems(){
	int x;
	for( x = 0; x < num_items; x++){
		deleteItem(ItemList[x]);
	}
	free(ItemList);
}