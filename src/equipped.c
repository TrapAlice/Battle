#include "equipped.h"
#include "moonmem.h"

Equipment* createEquipmentSlots(){
	Equipment* equipment = malloc(sizeof(Equipment));
	equipment->equipped = malloc(sizeof(Item)*num_slots);
	return equipment;
}

void deleteEquipmentSlots(Equipment* equipment){
	free(equipment->equipped);
	free(equipment);
}

Item* getEquipment(Equipment* equipment, enum Slot slot){
	return equipment->equipped[slot];
}

void Equip(Equipment* equipment, enum Slot slot, Item* item){
	equipment->equipped[slot]=item;
}