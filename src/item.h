#ifndef _ITEM_H
#define _ITEM_H
#include "libtcod.h"
#include "itemid.h"

typedef enum itemType_e{
	I_NONE,
	I_HEALING,
	I_EQUIPMENT
}itemType_e;

typedef enum itemSubType_e{
	IS_NONE,
	IS_WEAPON,
	IS_CHESTARMOR,
}itemSubType_e;

typedef struct item_t{
	const char    *name;
	const char    *desc;
	itemType_e     type;
	itemSubType_e  type2;
	int            relatedSkill;
	int            power;
	int            stackable;
}item_t;

extern item_t** ItemList;

item_t*  createItem         ( const char*, const char*, int, int, int, int, int );
item_t*  cloneItem          ( const item_t* );
void     deleteItem         ( item_t* );
void     getItemDescription ( const item_t*, TCOD_console_t );
int      itemIsType         ( const item_t*, itemType_e );
int      itemIsSubType      ( const item_t*, itemSubType_e );

#endif