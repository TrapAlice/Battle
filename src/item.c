#include "item.h"
#include "moonmem.h"

Item* createItem(char* name, char* desc, int type, int type2, int relatedSkill, int power, int stackable){
	Item* item = malloc(sizeof(Item));
	item->name = name;
	item->desc = desc;
	item->type = type;
	item->type2 = type2;
	item->relatedSkill = relatedSkill;
	item->power = power;
	item->stackable = stackable;
	
	return item;
}

Item* cloneItem(const Item* item){
	Item* clone = malloc(sizeof(Item));
	clone->name = item->name;
	clone->desc = item->desc;
	clone->type = item->type;
	clone->type2 = item->type2;
	clone->relatedSkill = item->relatedSkill;
	clone->power = item->power;
	clone->stackable = item->stackable;

	return clone;
}

void deleteItem(Item* item){
	free(item);
}

void getItemDescription(Item* item, TCOD_console_t panel){
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

int itemIsType(Item* item, enum ItemType type){
	return item->type == type;
}

int itemIsSubType(Item* item, enum ItemSubType subType){
	return item->type2 == subType;
}