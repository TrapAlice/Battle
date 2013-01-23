#include "inventory.h"
#include "moonmem.h"

Inventory* createInventory(){
	Inventory* inventory = malloc(sizeof(Inventory));
	inventory->quantity=0;
	inventory->item=0;
	return inventory;
}

static Inventory* Inventory_add(Item* item){
	Inventory* inventory = malloc(sizeof(Inventory));
	inventory->quantity=1;
	inventory->item=item;
	return inventory;
}

void deleteInventory(Inventory* inventory){
	Inventory* temp = inventory;
	Inventory* head = inventory;
	while(temp!=NULL){
		head = head->next;
		deleteItem(temp->item);
		free(temp);
		temp = head;
	}
}

void addItemInventory(Inventory* inventory, Item* item){
	Inventory* temp = inventory; 
	Inventory* temp2 = inventory; 
	Inventory* tail;
	while(temp != NULL){ 
		if(temp->item!=NULL){
			if(temp->item->stackable && temp->item->name == item->name){
				deleteItem(item);
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

void removeItemInventory(Inventory* inventory, Item* item){
	Inventory* temp = inventory;
	Inventory* temp2 = inventory;
	while(temp != NULL){
		if(temp->item!=NULL){
			if(temp->item->name == item->name){
				temp->quantity--;
				if(temp->quantity<=0){
					deleteItem(temp->item);
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