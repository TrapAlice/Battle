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
#include "dungeon.h"
#include "buffinit.h"

#include "dbg.h"

#include <stdio.h>


extern monster_t* player;
extern messagelist_t* globalMessage;
monster_t* monster;
TCOD_console_t msgConsole;
state_e GameState;
TCOD_console_t combatConsole;
messagelist_t* consoleLog;
messagelist_t* combatLog;
TCOD_console_t statusPanel;
TCOD_console_t inventoryPanel;
TCOD_key_t key;
item_t* items[26];
int keyPressed;
map_t* map;
dungeon_t* dungeon;
int currentFloor;

int main(int argc, char *argv[]) {    
	char done=0;
	int steps=25;
	int battleCooldown=10;
	int battleResult=0;
	int battleActionTaken;
	int itemchar;
	int x;

	currentFloor=0;

	init();
	test(); 
	calculateFov(map, player->object);
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
					calculateFov(map, player->object);
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
					if( isCollided(player->object, map->stairsdown) ){
						changeFloor(1);
					}
				}
				if( key.c=='<' ){
					if( isCollided(player->object, map->stairsup) ){
						changeFloor(-1);
					}
				}
				if( key.c=='u' ){
					/*Use something on the ground*/
				}
				if( battleCooldown<=0 ){
					if( oneIn(steps) ){
						steps = 50;
						battleCooldown=10;
						if( isSkillActive(player->skills, SKILL_STEALTH) ){
							increaseSkill(player->skills, SKILL_STEALTH, 1);
							if( skillCheck(player->skills, SKILL_STEALTH, 6) ){
								increaseSkill(player->skills, SKILL_STEALTH, 2);
								addMessage(consoleLog, "You avoid a monster!");
								break;
							}
						}
						
						/*if( oneIn(5) ){
							monster = cloneMonster(MonsterList[mob_fairy]);
						} else if( oneIn(4) ){
							monster = cloneMonster(MonsterList[mob_pig]);
						} else if( oneIn(3) ){
							monster = cloneMonster(MonsterList[mob_kobold]);
						} else {
							monster = cloneMonster(MonsterList[mob_slime]);*/
							monster = cloneMonster(MonsterList[mob_knight]);
						/*}*/

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
					attackMonster(player, monster);
					battleActionTaken=1;
				} else if( key.c == 'h' || key.c == 'H' ){
					itemchar=0;
					inventory_t* head = player->inventory->next;
					addMessage(combatLog, "Use which item?");
					while( head ){
						if( itemIsType(head->item, I_HEALING) ){
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
					if( !(keyPressed < 0 || keyPressed > 25) ){
						if( items[keyPressed]!=NULL ){
							if( itemIsType(items[keyPressed], I_HEALING) ){
								useItem(items[keyPressed],player);
								battleActionTaken=1;
							}
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
						attackMonster(monster, player);
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
				GameState = STATE_MAP;
				handleInput(&key);
				keyPressed = key.c-'a';
				if( keyPressed < 0 || keyPressed > 25 ) break;
				if( items[keyPressed] ){
					GameState = STATE_INVENTORYDETAIL;
				}
				break;

			case STATE_INVENTORYDETAIL:
				GameState=STATE_MAP;
				handleInput(&key);
				if( key.c == 'u' ){
					useItem(items[keyPressed],player);
				}
				break;

			case STATE_EQUIP:
				GameState = STATE_MAP;
				handleInput(&key);
				keyPressed = key.c-'a';
				if( keyPressed < 0 || keyPressed > 25 ) break;
				if( items[keyPressed] ){
					if( itemIsType(items[keyPressed], I_EQUIPMENT) ){
						useItem(items[keyPressed],player);
						GameState = STATE_EQUIP;
					}
				}
				
				break;

			case STATE_SKILLS:
				GameState = STATE_MAP;
				handleInput(&key);
				keyPressed = key.c-'a';
				if( keyPressed < 0 || keyPressed > 25 ) break;
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
	item_t* sword = cloneItem(ItemList[item_sword]);
	randomItemEnchant(sword, 10);
	randomItemEnchant(armor, 10);

	addItemInventory(player->inventory, cloneItem(ItemList[item_potion]));
	addItemInventory(player->inventory, cloneItem(ItemList[item_potion]));
	addItemInventory(player->inventory, cloneItem(ItemList[item_potion]));
	addItemInventory(player->inventory, sword);
	
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
	TCOD_console_clear(inventoryPanel); 
	TCOD_console_set_default_foreground(inventoryPanel, TCOD_white);
	switch(GameState){
		case STATE_MAP:
			x=-1;
			while( x++<getMessageListSize(consoleLog) ){
				TCOD_console_print(msgConsole, 0, x, getMessage(consoleLog, x));
			}

			renderMap(map, player->object->x, player->object->y);
			TCOD_console_print(NULL, 40, 15, "@");
			TCOD_console_print(statusPanel, 0, 0, "HP: %d/%d  XP: %d  Floor: %d",
			  player->combat->hp, player->combat->maxhp, player->xp, currentFloor);
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
			TCOD_console_print(combatConsole, 0, 20, "HP: %d/%d", 
			  player->combat->hp, player->combat->maxhp);
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
			while( head && itemchar<26 ){
				if( head->item->stackable ){
					TCOD_console_print(inventoryPanel, 0, itemchar, "[%c] %s x%d",
					  'A'+itemchar, head->item->name, head->quantity);
				} else {
					char str[40];
					TCOD_console_print(inventoryPanel, 0, itemchar, "[%c] %s",
					  'A'+itemchar, getFullItemName(head->item, str));
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
			while( head && itemchar<26 ){
				item = head->item;
				if( itemIsType(item, I_EQUIPMENT) ){
					char str[40];
					TCOD_console_print(inventoryPanel, 0, x, "%c] %s", 
					  'A'+itemchar, getFullItemName(item, str));
					if( isEquipped(player->equipment, item) ){
						TCOD_console_print(inventoryPanel, 40, x, "[Equipped]");
					}
					x++;
					items[itemchar]=item;
				}
				itemchar++;
				head=head->next;	
			}
			TCOD_console_blit(inventoryPanel, 0, 0, 80, 50, NULL, 5, 5, 128, 255);
			break;
		case STATE_SKILLS:
			showSkills(player->skills, inventoryPanel);
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
		case TCODK_UP  :
		case TCODK_KP8 : moveObject(player->object, map, 0, -1); action=1; break;
		case TCODK_KP1 : moveObject(player->object, map, -1, 1); action=1; break;
		case TCODK_DOWN:
		case TCODK_KP2 : moveObject(player->object, map, 0, 1); action=1; break;
		case TCODK_KP3 : moveObject(player->object, map, 1, 1); action=1; break;
		case TCODK_LEFT:
		case TCODK_KP4 : moveObject(player->object, map, -1, 0); action=1; break;
		case TCODK_RIGHT:
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
	map = newDungeonFloor(dungeon);
}

void changeFloor(int dfloor){
	currentFloor+=dfloor;
	if( dfloor > 0 ){
		map = getDungeonFloor(dungeon, currentFloor);
		player->object->x = map->stairsup->x;
		player->object->y = map->stairsup->y;
	} else {
		map = getDungeonFloor(dungeon, currentFloor);
		player->object->x = map->stairsdown->x;
		player->object->y = map->stairsdown->y;
	}
	calculateFov(map, player->object);
}

void positionPlayer(){

	player->object->x = map->stairsup->x;
	player->object->y = map->stairsup->y;
	
}

void init(){
	initMoonMem(128000);
	initSeed(0);
	initMonsters();
	initItems();
	initBuffs();
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
	dungeon = createDungeon(10);
	generateNewMap();
	positionPlayer();
}

void uninit(){
	deleteMap(map);
	deleteMonster(player);
	deleteDungeon(dungeon);
	TCOD_console_delete(statusPanel);
	TCOD_console_delete(msgConsole);
	deleteMessageList(consoleLog);
	TCOD_console_delete(combatConsole);
	deleteMessageList(combatLog);
	TCOD_console_delete(inventoryPanel);
	uninitBuffs();
	uninitItems();
	uninitMonsters();
	uninitMoonMem();
}