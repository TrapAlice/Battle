#include "item.h"
#include "moonmem.h"

Item* Item_create(char* name, char* desc, int type, int type2, int power, int stackable){
	Item* item = malloc(sizeof(Item));
	item->name = name;
	item->desc = desc;
	item->type = type;
	item->type2 = type2;
	item->power = power;
	item->stackable = stackable;
	
	return item;
}

Item* Item_clone(const Item* item){
	Item* clone = malloc(sizeof(Item));
	clone->name = item->name;
	clone->desc = item->desc;
	clone->type = item->type;
	clone->type2 = item->type2;
	clone->power = item->power;
	clone->stackable = item->stackable;

	return clone;
}

void Item_delete(Item* item){
	free(item);
}

void Item_description(Item* item, TCOD_console_t panel){
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
            	case IS_ARMOR:
            		TCOD_console_print(panel,0,3,"It blocks about 1d%d damage.",item->power);
            		break;
        	}
            break;
    }
}