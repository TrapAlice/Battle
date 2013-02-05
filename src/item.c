#include "item.h"
#include "moonmem.h"
#include "skills.h"
#include "monster.h"
#include "msg.h"
#include "inventory.h"
#include "rng.h"
#include "equipped.h"

#include <stdio.h>

item_t** ItemList;

item_t* createItem(const char* const name, const char* const desc, 
  int type, int type2, int relatedSkill, int power, int stackable, 
  float durability){
	item_t* item = malloc(sizeof(item_t));
	item->name = name;
	item->desc = desc;
	item->type = type;
	item->type2 = type2;
	item->relatedSkill = relatedSkill;
	item->power = power;
	item->stackable = stackable;
	item->maxDurability = durability;
	item->durability = durability;

	return( item );
}

item_t* cloneItem(const item_t* const item){
	item_t* clone = malloc(sizeof(item_t));
	clone->name = item->name;
	clone->desc = item->desc;
	clone->type = item->type;
	clone->type2 = item->type2;
	clone->relatedSkill = item->relatedSkill;
	clone->power = item->power;
	clone->stackable = item->stackable;
	clone->maxDurability = item->maxDurability;
	clone->durability = item->durability;

	return( clone );
}

void deleteItem(item_t* const item){
	free(item);
}

void getItemDescription(const item_t* const item, TCOD_console_t panel){
	TCOD_console_print(panel, 0, 0, "%s%s", getItemCondition(item), item->name);
	TCOD_console_print(panel, 0, 2, "%s", item->desc);

#ifndef NDEBUG
	TCOD_console_print(panel, 0, 10, "Durability: %f/%f", item->durability, item->maxDurability);
#endif

	switch( item->type ){
		case I_HEALING:
			TCOD_console_print(panel, 0, 3, "It has a healing factor of %d.",
			  item->power);
			TCOD_console_print(panel, 0, 5, "u) Use the %s", item->name);
			break;
		case I_EQUIPMENT:
			switch( item->type2 ){
				case IS_WEAPON:
					TCOD_console_print(panel, 0, 3, "Gives a bonus %d power",
					  item->power);
					if( !isEquipped(player->equipment, item) ){
						TCOD_console_print(panel, 0, 5, "u) Equip the %s", 
					  item->name);
					} else {
						TCOD_console_print(panel, 0, 5, "u) Unequip the %s", 
					  item->name);
					}
					break;
				case IS_CHESTARMOR:
					TCOD_console_print(panel, 0, 3, "Gives a bonus %d defense",
					  item->power);
					if( !isEquipped(player->equipment, item) ){
						TCOD_console_print(panel, 0, 5, "u) Equip the %s", item->name);
					} else {
						TCOD_console_print(panel, 0, 5, "u) Unequip the %s", item->name);
					}
					break;
				case IS_SHIELD:
					TCOD_console_print(panel, 0, 3, "Gives a bonus %d defense with a chance to block weapons",
					  item->power);
					if( !isEquipped(player->equipment, item) ){
						TCOD_console_print(panel, 0, 5, "u) Equip the %s", item->name);
					} else {
						TCOD_console_print(panel, 0, 5, "u) Unequip the %s", item->name);
					}
					break;
				case IS_NONE:
					break;
			}
			break;
		case I_NONE:
			break;
	}
}

int itemIsType(const item_t* const item, enum itemType_e type){
	return( item->type == type );
}

int itemIsSubType(const item_t* const item, enum itemSubType_e subType){
	return ( item->type2 == subType );
}

void useItem(item_t* const item){
	if( !item->durability ){
		addMessage(globalMessage, "Can't use that, it's broken");
		return;
	}
	switch( item->type ){
		case I_HEALING:
			addMessage(globalMessage, "You use the %s", item->name);
			takeDamage(player, -(roll(item->power,6)));
			removeItemInventory(player->inventory, item);
			break;
		case I_EQUIPMENT:
			switch( item->type2 ){
				case IS_WEAPON:
					if( !isEquipped(player->equipment, item) ){
						Equip(player->equipment, E_RHAND, item);
					} else {
						player->equipment->equipped[E_RHAND] = 0;
					}
					break;
				case IS_CHESTARMOR:
					if( !isEquipped(player->equipment, item) ){
						Equip(player->equipment, E_CHEST, item);
					} else {
						player->equipment->equipped[E_CHEST] = 0;
					}
					break;
				case IS_SHIELD:
					if( !isEquipped(player->equipment, item) ){
						Equip(player->equipment, E_LHAND, item);
					} else {
						player->equipment->equipped[E_LHAND] = 0;
					}
					break;
				case IS_NONE:
					break;
			}
			break;
		case I_NONE:
			break;
	}
}

int itemDamage(item_t* const item){
	if( oneIn(item->durability*5) ){
		item->durability--;
		if( !item->durability ){
			return( 1 );
		}
	}
	return( 0 );
}

char* getItemCondition(const item_t* const item){
	float conditionPercent = (item->durability/item->maxDurability)*100;
	if( conditionPercent > 90 ){
		return( "" );
	} else if( conditionPercent > 75){
		return( "Slightly worn " );
	} else if( conditionPercent > 40){
		return( "Worn " );
	} else if( conditionPercent > 0){
		return( "Damaged " );
	} else {
		return( "Broken " );
	}
}

void randomItemEnchant(item_t* const item, int chance){
	int damageBonus;
	int durabilityBonus;
	int x;
	while(chance--){
		if( oneIn(10-chance) ){
			x = between(0, 2);
			damageBonus += x;
			x = between(0, 3);
			durabilityBonus += x;
		}
	}
	item->damageBonus+=damageBonus;
	item->durability+=durabilityBonus;
	item->maxDurability+=durabilityBonus;
}

char* getItemBonus(const item_t* const item, char* const str){
	if( item->damageBonus <=0 ) return "";
	sprintf(str, " +%d", item->damageBonus);
	return str;
}

char* getFullItemName(const item_t* const item, char* const str){
	char bonus[4];
	sprintf(str, "%s%s%s",
		getItemCondition(item), item->name, getItemBonus(item, bonus));
	return str;
}