#include "battle.h"
#include "libtcod.h"
#include "moonmem.h"
#include "object.h"
#include "monster.h"
#include "combat.h"
#include "rng.h"
#include "msg.h"
#include "monsterinit.h"
#include "item.h"
#include "inventory.h"
#include "iteminit.h"
#include "equipped.h"

#include <stdio.h>


extern Monster* player;
extern MessageList* globalMessage;
Monster* monster;
TCOD_console_t msgConsole;
enum State_e GameState;
TCOD_console_t combatConsole;
MessageList* consoleLog;
MessageList* combatLog;
TCOD_console_t statusPanel;
TCOD_console_t inventoryPanel;
TCOD_key_t key;
Item* items[26];
int keyPressed;

int main(int argc, char *argv[]) {    
    char done=0;
    int steps=25;
    int battleCooldown=10;
    int battleResult=0;
    int battleActionTaken;
    int itemchar;

    init();
    test(); 

    while(!done){
        if( TCOD_console_is_window_closed() ){
            break;
        }
        printUI();
        
        switch(GameState){
            case STATE_MAP:
            	if( handleInput(&key) != 0 ){
		            steps--;
		            battleCooldown--;
		        }
                if(key.c=='i'){
                    GameState = STATE_INVENTORY;
                }
                if(key.c=='e'){
                	GameState = STATE_EQUIP;
                }
                if(key.c=='@'){
                    GameState = STATE_STATS;
                }
                if(battleCooldown<=0){
                    if(oneIn(steps)){
                        steps = 50;
                        battleCooldown=10;
                        if(oneIn(3)){
                            monster = cloneMonster(MonsterList[mob_pig]);
                        } else {
                            monster = cloneMonster(MonsterList[mob_slime]);
                        }

                        addMessage(consoleLog,"A wild %s appears!",monster->name);
                        printUI();
                        TCOD_sys_wait_for_event(TCOD_EVENT_KEY_RELEASE, NULL, NULL, false);
                        GameState=STATE_BATTLE;

                        
                    }
                } 
                break;

            case STATE_BATTLE:
            	handleInput(&key);
            	battleActionTaken=0;
		        clearMessageList(combatLog);

		        if(key.c == 'a' || key.c == 'A'){
		            addMessage(combatLog, "You attack!");
		            attackMonster(combatLog, player, monster);
		            battleActionTaken=1;
		        } else if(key.c == 'h' || key.c == 'H'){
		            itemchar=0;
		            Inventory* head = player->inventory->next;
		            addMessage(combatLog,"Use which item?");
		            while(head!=NULL){
		                addMessage(combatLog, "[%c] %s x%d",'A'+itemchar,head->item->name, head->quantity);
		                items[itemchar] = head->item;
		                itemchar++;
		                head=head->next;
		            }
		            addMessage(combatLog,"[1] Back");
		            printUI();
		            clearMessageList(combatLog);
		            handleInput(&key);
		            keyPressed = key.c-'a';
		            if(items[keyPressed]!=NULL){
		                if(itemIsType(items[keyPressed], I_HEALING)){
		                    takeDamage(combatLog, player, -(roll(1,items[keyPressed]->power)));
		                    removeItemInventory(player->inventory, items[keyPressed]);
		                    battleActionTaken=1;
		                }
		            }
		           
		            
		            
		        } else if(key.c == 'r' || key.c == 'R'){
		            addMessage(consoleLog,"You ran away!");
		            addMessage(combatLog,"You ran away!");
		            GameState=STATE_BATTLEAFTERMATH;
		            printUI();
		            battleResult=3;
		            
		            waitForPress();
			    	clearMessageList(combatLog);
		            break;
		        }
		        if(battleActionTaken){
		            if(checkDead(monster)){
		                addMessage(consoleLog,"You win!");
		                addMessage(combatLog,"You win!");
		                battleResult=2;
		                printUI();
		                GameState=STATE_BATTLEAFTERMATH;
		            	
		                waitForPress();
			    		clearMessageList(combatLog);
		            } else {
			            addMessage(combatLog, "The %s attacks!",monster->name);
			            attackMonster(combatLog, monster, player);
			            if(checkDead(player)){
			                addMessage(consoleLog,"You lose");
			                addMessage(combatLog,"You lose");
			                printUI();
			                battleResult=1;
			                GameState=STATE_BATTLEAFTERMATH;
		          			
			                waitForPress();
			    			clearMessageList(combatLog);
			            }
			        }
		        }
            	break;

            case STATE_BATTLEAFTERMATH:
			    switch(battleResult){
			        case 1:
			            addMessage(combatLog, "You've died >:");
			            break;
			        case 2:
			            addMessage(combatLog, "You win!");
			            addMessage(combatLog, "You gained %d EXP", monster->xp);
			            player->xp += monster->xp;
			            break;
			        case 3:
			            addMessage(combatLog, "You ran away safely");
			            break;
			    }
			    GameState = STATE_BATTLEAFTERMATH;
			    printUI();
			    clearMessageList(combatLog);
			    waitForPress();
			    GameState=STATE_MAP;
                deleteMonster(monster);
                if(battleResult == 1) done=1;
            	break;

            case STATE_INVENTORY:
            	handleInput(&key);
                keyPressed = key.c-'a';
                if(items[keyPressed]!=NULL){
                    GameState = STATE_INVENTORYDETAIL;
                } else {
                    GameState = STATE_MAP;
                }
                break;

            case STATE_INVENTORYDETAIL:
                GameState=STATE_MAP;
                waitForPress();
                break;

            case STATE_EQUIP:
            	handleInput(&key);
                keyPressed = key.c-'a';
                if(items[keyPressed]!=NULL){
                	if(itemIsType(items[keyPressed], I_EQUIPMENT)){
                		if(itemIsSubType(items[keyPressed], IS_SWORD) || itemIsSubType(items[keyPressed], IS_HAMMER)){
	                		Equip(player->equipment, E_Hand, items[keyPressed]);
	                	} else if(itemIsSubType(items[keyPressed], IS_CHESTARMOR)){
                            Equip(player->equipment, E_Chest, items[keyPressed]);
	                	}
                	}
                }
                GameState = STATE_MAP;
            	break;

            case STATE_STATS:
                GameState = STATE_MAP;
                waitForPress();
                break;
        }
        
    }
    

    uninit();
    return 0;
}

void test(){
    Item* armor = cloneItem(ItemList[item_leatherarmor]);
    Item* hammer = cloneItem(ItemList[item_pomfhammer]);
    
    addItemInventory(player->inventory, cloneItem(ItemList[item_potion]));
    addItemInventory(player->inventory, cloneItem(ItemList[item_potion]));
    addItemInventory(player->inventory, cloneItem(ItemList[item_potion]));
    addItemInventory(player->inventory, cloneItem(ItemList[item_sword]));
    
    addItemInventory(player->inventory, hammer);
    
    addItemInventory(player->inventory, armor);
}

void printUI(){
    int x;
    Inventory* head;
    Item* item;
    int itemchar;

    TCOD_console_clear(NULL);
    TCOD_console_clear(msgConsole);
    TCOD_console_clear(combatConsole);
    TCOD_console_clear(statusPanel);
    TCOD_console_clear(inventoryPanel);
    switch(GameState){
        case STATE_MAP:
            drawObject(player->object);
            x=0;
            while(x<getMessageListSize(consoleLog)){
                TCOD_console_print(msgConsole,0,x,getMessage(consoleLog, x));
                x++;
            }
            TCOD_console_print(statusPanel,0,0,"HP: %d/%d  XP: %d",player->combat->hp, player->combat->maxhp, player->xp);
            TCOD_console_blit(msgConsole,0,0,80,20,NULL,0,33,128,128);
            TCOD_console_blit(statusPanel,0,0,80,1,NULL,0,48,128,0);
            break;
        case STATE_BATTLE:
            x=0;
            while(x<getMessageListSize(combatLog)){
                TCOD_console_print(combatConsole,0,x,getMessage(combatLog, x));
                x++;
            }
            TCOD_console_print(combatConsole,0,20,"HP: %d/%d",player->combat->hp, player->combat->maxhp);
            TCOD_console_print(combatConsole,0,22,"[A] Attack");
            TCOD_console_print(combatConsole,0,23,"[H] Use Item");
            TCOD_console_print(combatConsole,0,24,"[R] Run");
            TCOD_console_blit(combatConsole,0,0,80,40,NULL,0,5,128,255);
            break;
        case STATE_BATTLEAFTERMATH:
            x=0;
            while(x<getMessageListSize(combatLog)){
                TCOD_console_print(combatConsole,30,x,getMessage(combatLog, x));
                x++;
            }
            TCOD_console_blit(combatConsole,0,0,80,40,NULL,0,20,128,255);
            break;
        case STATE_INVENTORY:
            head = player->inventory->next;
            
            itemchar = 0;
            while( head != NULL ){
                if(head->item->stackable){
                    TCOD_console_print(inventoryPanel,0,itemchar,"[%c] %s x%d",'A'+itemchar, head->item->name, head->quantity);
                } else {
                    TCOD_console_print(inventoryPanel,0,itemchar,"[%c] %s",'A'+itemchar, head->item->name);
                }
                items[itemchar]=head->item;
                head=head->next;
                
                itemchar++;
            }
            TCOD_console_blit(inventoryPanel,0,0,80,50,NULL,5,5,128,255);
            break;
        case STATE_INVENTORYDETAIL:
            item = items[keyPressed];
            getItemDescription(item, inventoryPanel);
            TCOD_console_blit(inventoryPanel,0,0,80,50,NULL,5,5,128,255);
            break;
        case STATE_EQUIP:
        	head = player->inventory->next;
        	itemchar = 0;
        	x = 0;
        	while(head != NULL){
        		item = head->item;
        		if(itemIsType(item, I_EQUIPMENT)){
        			if(getEquipment(player->equipment, E_Hand) != item &&
        			   getEquipment(player->equipment, E_Chest) != item){
	        			TCOD_console_print(inventoryPanel,0,x,"[%c] %s",'A'+itemchar, item->name);
	        			x++;
	        		}
        		}
        		items[itemchar]=item;
        		head=head->next;
        		itemchar++;
        	}
        	TCOD_console_print(inventoryPanel,50,0,"Hand:  %s", (getEquipment(player->equipment, E_Hand) != NULL ? getEquipment(player->equipment, E_Hand)->name : ""));
        	TCOD_console_print(inventoryPanel,50,1,"Chest: %s", (getEquipment(player->equipment, E_Chest) != NULL ? getEquipment(player->equipment, E_Chest)->name : ""));
        	TCOD_console_blit(inventoryPanel,0,0,80,50,NULL,5,5,128,255);
        	break;
        case STATE_STATS:
            itemchar=0;
            for(x=0; x<num_skills; x++){
                if(player->skills->skillLevel[x]>0){
                    TCOD_console_print(inventoryPanel,0,itemchar,"%s- %d",getSkillName(x),player->skills->skillLevel[x]);
                    itemchar++;
                }
            }
            TCOD_console_blit(inventoryPanel,0,0,80,50,NULL,5,5,128,255);
            break;
    }
    TCOD_console_flush();
}

int handleInput(TCOD_key_t* key){
    int action = 0;
    
    
    TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS, key, NULL, false);
    switch(key->vk) {
        case TCODK_KP7 : moveObject(player->object, -1, -1); action=1; break;
        case TCODK_KP9 : moveObject(player->object, 1, -1); action=1; break;
        case TCODK_KP8 : moveObject(player->object, 0, -1); action=1; break;
        case TCODK_KP1 : moveObject(player->object, -1, 1); action=1; break;
        case TCODK_KP2 : moveObject(player->object, 0, 1); action=1; break;
        case TCODK_KP3 : moveObject(player->object, 1, 1); action=1; break;
        case TCODK_KP4 : moveObject(player->object, -1, 0); action=1; break;
        case TCODK_KP6 : moveObject(player->object, 1, 0); action=1; break;
        default:break;
    }
    return action;
}

void waitForPress(){
    TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS, NULL, NULL, false);
    TCOD_sys_wait_for_event(TCOD_EVENT_KEY_RELEASE, NULL, NULL, false);
}

void init(){
    initMoonMem(2048);
    initSeed(0);
    initMonsters();
    initItems();
    consoleLog = createMessageList(15);
    globalMessage = consoleLog;
    msgConsole = TCOD_console_new(80,20);
    GameState = 0;
    combatLog = createMessageList(15);
    combatConsole = TCOD_console_new(80,40);
    statusPanel = TCOD_console_new(80,2);
    inventoryPanel = TCOD_console_new(80,50);

    player = createPlayer(20,20);
    TCOD_console_init_root(80,50,TITLE,false,false);
}

void uninit(){
    deleteMonster(player);
    TCOD_console_delete(statusPanel);
    TCOD_console_delete(msgConsole);
    deleteMessageList(consoleLog);
    TCOD_console_delete(combatConsole);
    deleteMessageList(combatLog);
    TCOD_console_delete(inventoryPanel);
    uninitItems();
    uninitMonsters();
    uninitMoonMem();
}