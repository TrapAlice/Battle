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
#include "map.h"
#include "tile.h"
#include "skills.h"

#include <stdio.h>


extern monster_t* player;
extern messagelist_t* globalMessage;
monster_t* monster;
TCOD_console_t msgConsole;
enum State_e GameState;
TCOD_console_t combatConsole;
messagelist_t* consoleLog;
messagelist_t* combatLog;
TCOD_console_t statusPanel;
TCOD_console_t inventoryPanel;
TCOD_key_t key;
item_t* items[26];
int keyPressed;
map_t* map;
int currentFloor;

int main(int argc, char *argv[]) {    
	char done=0;
	int steps=25;
	int battleCooldown=10;
	int battleResult=0;
	int battleActionTaken;
	int itemchar;
	int x;


	init();
	test(); 
	TCOD_map_compute_fov(map->mapFov, player->object->x, player->object->y, 0, 1, FOV_BASIC);
	while( !done ){
		if( TCOD_console_is_window_closed() ){
			break;
		}
		printUI();
		
		switch( GameState ){
			case STATE_MAP:
				if( handleInput(&key) ){
					steps--;
					battleCooldown--;
					TCOD_map_compute_fov(map->mapFov, player->object->x, player->object->y, 0, 1, FOV_BASIC);
				}
				if( key.c=='i' ){
					GameState = STATE_INVENTORY;
				}
				if( key.c=='e' ){
					GameState = STATE_EQUIP;
				}
				if( key.c=='s' ){
					GameState = STATE_SKILLS;
				}
				if( key.c=='>' ){
					if( isCollided(player->object, map->objects[0]) ){
						generateNewMap();
						currentFloor++;
						positionPlayer();
					}
				}
				if( battleCooldown<=0 ){
					if( oneIn(steps) ){
						steps = 50;
						battleCooldown=10;
						if( oneIn(5) ){
							monster = cloneMonster(MonsterList[mob_fairy]);
						} else if( oneIn(4) ){
							monster = cloneMonster(MonsterList[mob_pig]);
						} else if( oneIn(8) ){
							monster = cloneMonster(MonsterList[mob_kobold]);
						} else {
							monster = cloneMonster(MonsterList[mob_slime]);
						}

						addMessage(consoleLog, "A wild %s appears!", monster->name);
						printUI();
						TCOD_sys_wait_for_event(TCOD_EVENT_KEY_RELEASE, NULL, NULL, false);
						GameState=STATE_BATTLE;

						
					}
				} 
				
				break;

			case STATE_BATTLE:
				handleInput(&key);
				battleActionTaken=0;

				if( key.c == 'a' || key.c == 'A' ){
					addMessage(consoleLog, "You attack!");
					attackMonster(consoleLog, player, monster);
					battleActionTaken=1;
				} else if( key.c == 'h' || key.c == 'H' ){
					itemchar=0;
					inventory_t* head = player->inventory->next;
					addMessage(combatLog, "Use which item?");
					while(head){
						if(itemIsType(head->item, I_HEALING)){
							addMessage(combatLog, "[%c] %s x%d",'A'+itemchar,head->item->name, head->quantity);
							items[itemchar] = head->item;
						}
						itemchar++;
						head=head->next;
					}
					addMessage(combatLog, "[1] Back");
					printUI();
					clearMessageList(combatLog);
					handleInput(&key);
					keyPressed = key.c-'a';
					if( items[keyPressed]!=NULL ){
						if( itemIsType(items[keyPressed], I_HEALING) ){
							takeDamage(consoleLog, player, -(roll(items[keyPressed]->power,6)));
							removeItemInventory(player->inventory, items[keyPressed]);
							battleActionTaken=1;
						}
					}
				   
					
					
				} else if( key.c == 'r' || key.c == 'R' ){
					addMessage(consoleLog, "You ran away!");
					
					printUI();
					GameState=STATE_BATTLEAFTERMATH;
					battleResult=3;
					
					waitForPress();
					break;
				}
				if( battleActionTaken ){
					if( checkDead(monster) ){
						addMessage(consoleLog, "You win!");
						battleResult=2;
						printUI();
						GameState=STATE_BATTLEAFTERMATH;
						
						waitForPress();
						
						
					} else {
						attackMonster(consoleLog, monster, player);
						if( checkDead(player) ){
							addMessage(consoleLog, "You lose");
							printUI();
							battleResult=1;
							GameState=STATE_BATTLEAFTERMATH;
							
							waitForPress();
						}
					}
				}
				break;

			case STATE_BATTLEAFTERMATH:
				switch( battleResult ){
					case 1:
						break;
					case 2:
						addMessage(consoleLog, "You gained %d EXP", monster->xp);
						player->xp += monster->xp;
						break;
					case 3:
						break;
				}
				GameState = STATE_BATTLEAFTERMATH;
				printUI();
				addMessage(consoleLog, "");
				GameState=STATE_MAP;
				deleteMonster(monster);
				if( battleResult == 1 ) done=1;
				break;

			case STATE_INVENTORY:
				handleInput(&key);
				keyPressed = key.c-'a';
				if( items[keyPressed] ){
					GameState = STATE_INVENTORYDETAIL;
				} else {
					GameState = STATE_MAP;
				}
				break;

			case STATE_INVENTORYDETAIL:
				GameState=STATE_MAP;
				handleInput(&key);
				if( key.c == 'u' ){
					useItem(items[keyPressed]);
				}
				break;

			case STATE_EQUIP:
				GameState = STATE_MAP;
				handleInput(&key);
				keyPressed = key.c-'a';
				if( items[keyPressed] ){
					if( itemIsType(items[keyPressed], I_EQUIPMENT) ){
						if( itemIsSubType(items[keyPressed], IS_WEAPON) ){
							Equip(player->equipment, E_RHAND, items[keyPressed]);
						} else if( itemIsSubType(items[keyPressed], IS_CHESTARMOR) ){
							Equip(player->equipment, E_CHEST, items[keyPressed]);
						} else if( itemIsSubType(items[keyPressed], IS_SHIELD) ){
							Equip(player->equipment, E_LHAND, items[keyPressed]);
						}
						GameState = STATE_EQUIP;
					}
				}
				
				break;

			case STATE_SKILLS:
				GameState = STATE_MAP;
				handleInput(&key);
				keyPressed = key.c-'a';
				itemchar = -1;
				for( x=1; x<num_skills;++x ){
					if( player->skills->skillLevel[x]>0 ){
						itemchar++;
					}
					if( itemchar == keyPressed ){
						if( !player->skills->skillActive[x] )
						{
							if( skillsCurrentlyActive(player->skills)<2 ){
								player->skills->skillActive[x] = 1;
							}
						} else {
							player->skills->skillActive[x] = 0;
						}

						GameState = STATE_SKILLS;
						break;
					}
					
				}
				break;
		}
		
	}
	

	uninit();
	return 0;
}

void test(){
	item_t* armor = cloneItem(ItemList[item_leatherarmor]);
	item_t* shield = cloneItem(ItemList[item_shield]);
	
	addItemInventory(player->inventory, cloneItem(ItemList[item_potion]));
	addItemInventory(player->inventory, cloneItem(ItemList[item_potion]));
	addItemInventory(player->inventory, cloneItem(ItemList[item_potion]));
	addItemInventory(player->inventory, cloneItem(ItemList[item_sword]));
	
	addItemInventory(player->inventory, shield);
	
	addItemInventory(player->inventory, armor);
}

void printUI(){
	int x;
	inventory_t* head;
	item_t* item;
	int itemchar;

	TCOD_console_clear(NULL);
	TCOD_console_clear(msgConsole);
	TCOD_console_clear(combatConsole);
	TCOD_console_clear(statusPanel);
	TCOD_console_clear(inventoryPanel); TCOD_console_set_default_foreground(inventoryPanel, TCOD_white);
	switch(GameState){
		case STATE_MAP:
			
			x=-1;
			while( x++<getMessageListSize(consoleLog) ){
				TCOD_console_print(msgConsole, 0, x, getMessage(consoleLog, x));
			}

			renderMap(map, player->object->x, player->object->y);
			TCOD_console_print(NULL, 40, 15, "@");
			TCOD_console_print(statusPanel, 0, 0, "HP: %d/%d  XP: %d  Floor: %d", player->combat->hp, player->combat->maxhp, player->xp, currentFloor);
			TCOD_console_blit(msgConsole, 0, 0, 80, 20, NULL, 0, 33, 128, 128);
			TCOD_console_blit(statusPanel, 0, 0, 80, 1, NULL, 0, 48, 128, 0);
			break;
		case STATE_BATTLE:
			x=-1;
			while( x++<getMessageListSize(consoleLog) ){
				TCOD_console_print(msgConsole, 0, x, getMessage(consoleLog, x));
			}
			x=-1;
			while( x++<getMessageListSize(combatLog) ){
				TCOD_console_print(combatConsole, 21, 22+x, getMessage(combatLog, x));
			}
			TCOD_console_print(combatConsole, 0, 20, "HP: %d/%d", player->combat->hp, player->combat->maxhp);
			TCOD_console_print(combatConsole, 0, 22, "[A] Attack");
			TCOD_console_print(combatConsole, 0, 23, "[H] Use Item");
			TCOD_console_print(combatConsole, 0, 24, "[R] Run");
			TCOD_console_blit(combatConsole, 0, 0, 80, 40, NULL, 0, 5, 128, 255);
			TCOD_console_blit(msgConsole, 0, 0, 80, 20, NULL, 0, 33, 128, 0);
			break;
		case STATE_BATTLEAFTERMATH:
			x=-1;
			while( x++<getMessageListSize(combatLog) ){
				TCOD_console_print(combatConsole, 30, x, getMessage(combatLog, x));
			}
			TCOD_console_blit(combatConsole, 0, 0, 80, 40, NULL, 0, 20, 128, 255);
			break;
		case STATE_INVENTORY:
			head = player->inventory->next;
			
			itemchar = 0;
			while( head ){
				if( head->item->stackable ){
					TCOD_console_print(inventoryPanel, 0, itemchar, "[%c] %s x%d",'A'+itemchar, head->item->name, head->quantity);
				} else {
					TCOD_console_print(inventoryPanel, 0, itemchar, "[%c] %s%s",'A'+itemchar, getItemCondition(head->item), head->item->name);
				}
				items[itemchar]=head->item;
				head=head->next;
				
				itemchar++;
			}
			TCOD_console_blit(inventoryPanel, 0, 0, 80, 50, NULL, 5, 5, 128, 255);
			break;
		case STATE_INVENTORYDETAIL:
			item = items[keyPressed];
			getItemDescription(item, inventoryPanel);
			TCOD_console_blit(inventoryPanel, 0, 0, 80, 50, NULL, 5, 5, 128, 255);
			break;
		case STATE_EQUIP:
			head = player->inventory->next;
			itemchar = 0;
			x = 0;
			while( head ){
				item = head->item;
				if( itemIsType(item, I_EQUIPMENT) ){
					if( !isEquipped(player->equipment, item) ){
						TCOD_console_print(inventoryPanel, 0, x, "%c] %s%s", 'A'+itemchar, getItemCondition(item), item->name);
						x++;
					}
				}
				items[itemchar]=item;
				head=head->next;
				itemchar++;
			}
			TCOD_console_print(inventoryPanel, 50, 0,"R Hand:  %s", (getEquipment(player->equipment, E_RHAND) ? getEquipment(player->equipment, E_RHAND)->name : ""));
			TCOD_console_print(inventoryPanel, 50, 1,"L Hand:  %s", (getEquipment(player->equipment, E_LHAND) ? getEquipment(player->equipment, E_LHAND)->name : ""));
			TCOD_console_print(inventoryPanel, 50, 2,"Chest :  %s", (getEquipment(player->equipment, E_CHEST) ? getEquipment(player->equipment, E_CHEST)->name : ""));
			TCOD_console_blit(inventoryPanel, 0, 0, 80, 50, NULL, 5, 5, 128, 255);
			break;
		case STATE_SKILLS:
			itemchar=0;

			TCOD_console_set_default_foreground(inventoryPanel,TCOD_yellow);
			TCOD_console_print(inventoryPanel, 0, 0, "[Active]");
			TCOD_console_set_default_foreground(inventoryPanel,TCOD_dark_grey);
			TCOD_console_print(inventoryPanel, 10, 0, "[Inactive]");

			for( x=0; x<num_skills; ++x ){
				if( player->skills->skillLevel[x]>0 ){
					if( isSkillActive(player->skills, x) ){ TCOD_console_set_default_foreground(inventoryPanel, TCOD_yellow);}
					else{TCOD_console_set_default_foreground(inventoryPanel,TCOD_dark_grey);}
					TCOD_console_print(inventoryPanel, 0, itemchar+2, "%c] %s", itemchar+'A', getSkillName(x));
					TCOD_console_print(inventoryPanel, 13, itemchar+2, "- %d", player->skills->skillLevel[x]);
					itemchar++;
				}
			}
			TCOD_console_blit(inventoryPanel, 0, 0, 80, 50, NULL, 5, 5, 128, 255);
			break;
	}
	TCOD_console_flush();
}

int handleInput(TCOD_key_t* key){
	int action = 0;
	
	
	TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS, key, NULL, false);
	switch( key->vk ) {
		case TCODK_KP7 : moveObject(player->object, map, -1, -1); action=1; break;
		case TCODK_KP9 : moveObject(player->object, map, 1, -1); action=1; break;
		case TCODK_KP8 : moveObject(player->object, map, 0, -1); action=1; break;
		case TCODK_KP1 : moveObject(player->object, map, -1, 1); action=1; break;
		case TCODK_KP2 : moveObject(player->object, map, 0, 1); action=1; break;
		case TCODK_KP3 : moveObject(player->object, map, 1, 1); action=1; break;
		case TCODK_KP4 : moveObject(player->object, map, -1, 0); action=1; break;
		case TCODK_KP6 : moveObject(player->object, map, 1, 0); action=1; break;
		default:break;
	}
	return action;
}

void waitForPress(){
	TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS, NULL, NULL, false);
	TCOD_sys_wait_for_event(TCOD_EVENT_KEY_RELEASE, NULL, NULL, false);
}

void generateNewMap(){
	if( map ) deleteMap(map);
	map = createMap(between(30,50), between(30,50));
	makeMap(map, between(5,15), between(2,5), between(6,10), 0);
}

void positionPlayer(){
	tile_t *tile;
	int x=0;
	int y=0;
	tile = map->mapTiles[0];
	while( (tile->ops & 1 << 1) ){
		x= between(0, map->width);
		y= between(0, map->height);
		tile = map->mapTiles[x+(y*map->width)];
	}
	player->object->x = x;
	player->object->y = y;
	TCOD_map_compute_fov(map->mapFov, player->object->x, player->object->y, 0, 1, FOV_BASIC);
}

void init(){
	initMoonMem(64000);
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
	currentFloor = 0;
	TCOD_console_init_root(80,50,TITLE,false,false);
	generateNewMap();
	positionPlayer();
}

void uninit(){
	deleteMap(map);
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