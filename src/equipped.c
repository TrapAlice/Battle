#include "equipped.h"
#include "moonmem.h"

Equipment* Equipment_slotsCreate(){
	Equipment* equipment = malloc(sizeof(Equipment));
	equipment->equipped = malloc(sizeof(num_slots));
	return equipment;
}