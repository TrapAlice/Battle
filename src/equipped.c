#include "equipped.h"
#include "moonmem.h"
#include "item.h"
#include "skills.h"

equipment_t* createEquipmentSlots(){
	equipment_t* equipment = malloc(sizeof(equipment_t));
	equipment->equipped = malloc(sizeof(item_t)*num_slots);
	return equipment;
}

void deleteEquipmentSlots(equipment_t* equipment){
	free(equipment->equipped);
	free(equipment);
}

item_t* getEquipment(equipment_t* equipment, slot_e slot){
	return equipment->equipped[slot];
}

void Equip(equipment_t* equipment, slot_e slot, item_t* item){
	equipment->equipped[slot]=item;
}