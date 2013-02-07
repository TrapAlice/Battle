#include "inventory.h"
#include "item.h"
#include "moonmem.h"

inventory_t* createInventory(){
	inventory_t* inventory = malloc(sizeof(inventory_t));
	inventory->quantity=0;
	inventory->item = cloneItem(ItemList[item_null]);
	return( inventory );
}

static inventory_t* Inventory_add(item_t* item){
	inventory_t* inventory = malloc(sizeof(inventory_t));
	inventory->quantity=1;
	inventory->item=item;
	return( inventory );
}

void deleteInventory(inventory_t* const inventory){
	inventory_t* temp = inventory;
	inventory_t* head = inventory;
	while( temp ){
		head = head->next;
		deleteItem(temp->item);
		free(temp);
		temp = head;
	}
}

void addItemInventory(inventory_t* const inventory, item_t* const item){
	inventory_t* temp = inventory; 
	inventory_t* temp2 = inventory; 
	inventory_t* tail;
	while( temp ){ 
		if( temp->item ){
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

void removeItemInventory(inventory_t* const inventory, item_t* const item){
	inventory_t* temp = inventory;
	inventory_t* temp2 = inventory;
	while( temp ){
		if( temp->item ){
			if( temp->item->name == item->name ){
				temp->quantity--;
				if( temp->quantity<=0 ){
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