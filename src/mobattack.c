#include "mobattack.h"
#include "monster.h"
#include "msg.h"
#include "item.h"
#include "equipped.h"
#include "rng.h"
#include "combat.h"
#include "skills.h"

void fairyAttack(monster_t* const attacker, monster_t* const defender){
	int damage;
	addMessage(globalMessage, "The fairy fires a fireball");
	damage = roll(attacker->combat->power, 6);
	takeDamage(globalMessage, defender, damage);
}