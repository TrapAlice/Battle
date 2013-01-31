#include "mobbirth.h"
#include "monster.h"
#include "item.h"
#include "equipped.h"
#include "inventory.h"


void playerBirth(monster_t* player){

}

void koboldEquip(monster_t* const monster){
	item_t* knife = ItemList[item_knife];
	monster->equipment = createEquipmentSlots();
	Equip(monster->equipment, E_RHAND, knife);
}