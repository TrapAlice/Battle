#include "triggerfunctions.h"
#include "object.h"
#include "msg.h"
#include "monster.h"
#include "item.h"
#include "inventory.h"
#include "rng.h"
#include "lockpicking.h"
#include "moonmem.h"


void openChest(object_t* self){
	int chance;
	item_t* item = cloneItem(ItemList[item_sword]);
	char str[40];
	addMessage(globalMessage, "You open the treasure chest");
	if( startLockpicking() ){
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
		self->trigger = &emptyChest;
	} else {
		addMessage(globalMessage, "You failed to pick the lock");
		free(item);
	}
}

void emptyChest(object_t* self){
	addMessage(globalMessage, "The chest is empty");
}