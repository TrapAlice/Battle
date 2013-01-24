#include "mobdeath.h"
#include "monster.h"
#include "rng.h"
#include "skills.h"
#include "inventory.h"
#include "iteminit.h"

extern Monster* player;

void pigDeath(){
	int carving = player->skills->skillLevel[SKILL_CARVING];
	if(oneIn(5-carving)){
		addMessage(globalMessage, "Successfully carved some meat");
		addItemInventory(player->inventory, cloneItem(ItemList[item_meat]));
		increaseSkill(player->skills, SKILL_CARVING, 3);
	}
	increaseSkill(player->skills, SKILL_CARVING, 2);
}