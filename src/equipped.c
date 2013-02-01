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
	checknotnull(equipment);
	return( equipment->equipped[slot] );
}

void Equip(const equipment_t* const equipment, slot_e slot, item_t* const item){
	equipment->equipped[slot]=item;
}

int isEquipped(const equipment_t* const equipment, const item_t* const item){
	int x;
	for( x=0; x<num_slots; ++x){
		if( equipment->equipped[x] == item ){
			return( 1 );
		}
	}
	return( 0 );
}

int getEquipmentDefense(const equipment_t* const equipment){
	int x;
	int defense=0;
	checknotnull(equipment);
	for( x=1; x<num_slots; ++x){
		defense += (getEquipment(equipment, x) ? getEquipment(equipment, x)->power : 0);
	}
	return( defense );
}