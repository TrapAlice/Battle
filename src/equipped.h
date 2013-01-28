#ifndef _EQUIPPED_H
#define _EQUIPPED_H

#include "item.h"

typedef enum slot_e{
	E_HAND,
	E_HEAD,
	E_CHEST,
	num_slots,
}slot_e;

typedef struct
{
	Item** equipped;
}Equipment;

Equipment* createEquipmentSlots();
void deleteEquipmentSlots(Equipment*);
Item* getEquipment(Equipment*, slot_e);
void Equip(Equipment*, slot_e, Item*);

#endif