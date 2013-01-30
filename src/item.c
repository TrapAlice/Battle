#include "item.h"
#include "moonmem.h"
#include "skills.h"

item_t* createItem(char* name, char* desc, int type, int type2, int relatedSkill, int power, int stackable){
	item_t* item = malloc(sizeof(item_t));
	item->name = name;
	item->desc = desc;
	item->type = type;
	item->type2 = type2;
	item->relatedSkill = relatedSkill;
	item->power = power;
	item->stackable = stackable;
	
	return item;
}

item_t* cloneItem(const item_t* item){
	item_t* clone = malloc(sizeof(item_t));
	clone->name = item->name;
	clone->desc = item->desc;
	clone->type = item->type;
	clone->type2 = item->type2;
	clone->relatedSkill = item->relatedSkill;
	clone->power = item->power;
	clone->stackable = item->stackable;

	return clone;
}

void deleteItem(item_t* item){
	free(item);
}

void getItemDescription(item_t* item, TCOD_console_t panel){
	TCOD_console_print(panel,0,0,"%s",item->name);
	TCOD_console_print(panel,0,2,"%s",item->desc);
	switch(item->type){
		case I_HEALING:
			TCOD_console_print(panel,0,3,"It heals for about 1d%d.",item->power);
			break;
		case I_EQUIPMENT:
			switch(item->type2){
				case IS_WEAPON:
					TCOD_console_print(panel,0,3,"It deals about 1d%d.",item->power);
					break;
				case IS_CHESTARMOR:
					TCOD_console_print(panel,0,3,"It blocks about 1d%d damage.",item->power);
					break;
				case IS_NONE:
					break;
			}
			break;
		case I_NONE:
			break;
	}
}

int itemIsType(item_t* item, enum itemType_e type){
	return item->type == type;
}

int itemIsSubType(item_t* item, enum itemSubType_e subType){
	return item->type2 == subType;
}