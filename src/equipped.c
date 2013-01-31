#include "equipped.h"
#include "moonmem.h"
#include "item.h"
#include "skills.h"

equipment_t* createEquipmentSlots(){
	equipment_t* equipment = malloc(sizeof(equipment_t));
	equipment->equipped = malloc(sizeof(item_t)*num_slots);
	return( equipment );
}

void deleteEquipmentSlots(equipment_t* const equipment){
	free(equipment->equipped);
	free(equipment);
}

item_t* getEquipment(const equipment_t* const equipment, slot_e slot){
	return( equipment->equipped[slot] );
}

void Equip(const equipment_t* const equipment, slot_e slot, item_t* const item){
	equipment->equipped[slot]=item;
}