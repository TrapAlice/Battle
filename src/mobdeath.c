#include "mobdeath.h"
#include "monster.h"
#include "rng.h"
#include "skills.h"
#include "inventory.h"
#include "iteminit.h"
#include "msg.h"

extern monster_t* player;

void pigDeath(monster_t* monster){
	int carving = player->skills->skillLevel[SKILL_CARVING];
	if(oneIn(5-carving)){
		addMessage(globalMessage, "Successfully carved some meat");
		addItemInventory(player->inventory, cloneItem(ItemList[item_meat]));
		increaseSkill(player->skills, SKILL_CARVING, 3);
	}
	increaseSkill(player->skills, SKILL_CARVING, 2);
}

void fairyDeath(monster_t* monster){
	addMessage(globalMessage, "The fairy explodes, you get covered in dust");
	takeDamage(NULL, player, -10);
}