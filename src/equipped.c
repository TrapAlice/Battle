#include "equipped.h"
#include "moonmem.h"

Equipment* Equipment_slotsCreate(){
	Equipment* equipment = malloc(sizeof(Equipment));
	equipment->equipped = malloc(sizeof(Item)*num_slots);
	return equipment;
}

void Equipment_delete(Equipment* equipment){
	free(equipment->equipped);
	free(equipment);
}