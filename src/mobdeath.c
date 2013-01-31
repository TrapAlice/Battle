#include "mobdeath.h"
#include "monster.h"
#include "rng.h"
#include "skills.h"
#include "inventory.h"
#include "item.h"
#include "msg.h"

extern monster_t* player;

void standardDeath(const monster_t* const monster){
	addMessage(globalMessage, "The %s dies",monster->name);
}

void pigDeath(const monster_t* const monster){
	int carving = player->skills->skillLevel[SKILL_CARVING];
	if(oneIn(5-carving)){
		addMessage(globalMessage, "Successfully carved some meat");
		addItemInventory(player->inventory, cloneItem(ItemList[item_meat]));
		increaseSkill(player->skills, SKILL_CARVING, 3);
	}
	increaseSkill(player->skills, SKILL_CARVING, 2);
}

void fairyDeath(const monster_t* const monster){
	addMessage(globalMessage, "The fairy explodes, you get covered in dust");
	takeDamage(NULL, player, -10);
}