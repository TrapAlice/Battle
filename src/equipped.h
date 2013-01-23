#ifndef _EQUIPPED_H
#define _EQUIPPED_H

#include "item.h"

enum Slot{
	E_Hand,
	E_Head,
	E_Chest,
	num_slots,
};

typedef struct
{
	Item** equipped;
}Equipment;

Equipment* createEquipmentSlots();
void deleteEquipmentSlots(Equipment*);
Item* getEquipment(Equipment*, enum Slot);
void Equip(Equipment*, enum Slot, Item*);

#endif