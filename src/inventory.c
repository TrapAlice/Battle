#include "inventory.h"
#include "moonmem.h"

Inventory* Inventory_create(){
	Inventory* inventory = malloc(sizeof(Inventory));
	inventory->quantity=0;
	inventory->item=0;
	return inventory;
}

Inventory* Inventory_add(Item* item){
	Inventory* inventory = malloc(sizeof(Inventory));
	inventory->quantity=1;
	inventory->item=item;
	return inventory;
}

void Inventory_delete(Inventory* inventory){
	Inventory* temp = inventory;
	Inventory* head = inventory;
	while(temp!=NULL){
		head = head->next;
		Item_delete(temp->item);
		free(temp);
		temp = head;
	}
}

void Inventory_addItem(Inventory* inventory, Item* item){
	Inventory* temp = inventory; 
	Inventory* temp2 = inventory; 
	Inventory* tail;
	while(temp != NULL){ 
		if(temp->item!=NULL){
			if(temp->item->stackable && temp->item->name == item->name){
				Item_delete(item);
				temp->quantity++;
				return;
			}
		}
		temp2 = temp;
		temp = temp->next;
	}
	tail = Inventory_add(item);
	temp2->next = tail;
}

void Inventory_removeItem(Inventory* inventory, Item* item){
	Inventory* temp = inventory;
	Inventory* temp2 = inventory;
	while(temp != NULL){
		if(temp->item!=NULL){
			if(temp->item->name == item->name){
				temp->quantity--;
				if(temp->quantity<=0){
					Item_delete(temp->item);
					temp2->next = temp->next;
					free(temp);
					return;
				}
			}
		}
		temp2=temp;
		temp=temp->next;
	}
}