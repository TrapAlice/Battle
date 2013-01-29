#ifndef _EQUIPPED_H
#define _EQUIPPED_H

/*#include "item.h"*/

struct item_t;

typedef enum slot_e{
	E_HAND,
	E_HEAD,
	E_CHEST,
	num_slots,
}slot_e;

typedef struct equipment_t{
	struct item_t** equipped;
}equipment_t;

equipment_t* createEquipmentSlots();
void deleteEquipmentSlots(equipment_t*);
struct item_t* getEquipment(equipment_t*, slot_e);
void Equip(equipment_t*, slot_e, struct item_t*);

#endif