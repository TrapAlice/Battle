#include "triggerfunctions.h"
#include "object.h"
#include "msg.h"
#include "monster.h"
#include "item.h"
#include "inventory.h"
#include "rng.h"


void openChest(object_t* self){
	int chance;
	item_t* item=ItemList[item_sword];
	char str[40];
	addMessage(globalMessage, "You open the treasure chest");
	chance =1;
	switch( chance ){
		case 1:
			randomItemEnchant(item, 5);
		case 2:
			randomItemEnchant(item, 5);
		case 3:
			randomItemEnchant(item, 5);
		case 4:
			randomItemEnchant(item, 5);
		case 5:
			randomItemEnchant(item, 5);
		case 6:
			break;
	}
	addItemInventory(player->inventory, item);
	addMessage(globalMessage, "You found a %s", getFullItemName(item, str));
	self->self = 't';
	self->trigger = 0;
}