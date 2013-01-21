#include "combat.h"
#include "moonmem.h"
#include "rng.h"
#include "msg.h"

void Combat_takeDamage(MessageList* messageLog, Combat* defender, const char* defender_name, int damage){
	if(damage>0){
		Msg_addMessage(messageLog, "%s takes %d damage", defender_name, damage);
	} else if (damage < 0){
		Msg_addMessage(messageLog, "%s is healed for %d", defender_name, -damage);
	} else {
		Msg_addMessage(messageLog, "%s avoids the attack", defender_name, damage);
	}
	defender->hp -= damage;
	if(defender->hp > defender->maxhp){
		defender->hp = defender->maxhp;
	}
}

Combat* Combat_create(int maxhp, int power, int defense){
	Combat* combat = malloc(sizeof(Combat));
	combat->hp = maxhp;
	combat->maxhp = maxhp;
	combat->power = power;
	combat->defense = defense;
	combat->hits = 1;
	return combat;
}

void Combat_delete(Combat* combat){
	free(combat);
}