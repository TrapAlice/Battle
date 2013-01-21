#ifndef _EQUIPPED_H
#define _EQUIPPED_H

#include "item.h"

enum Slot{
	E_Hand,
	num_slots,
};

typedef struct
{
	Item** equipped;
}Equipment;

Equipment* Equipment_slotsCreate();
void Equipment_delete(Equipment*);

#endif