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