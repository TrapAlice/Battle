#include "libtcod.h"
#include "moonmem.h"
#include "test_ids.h"
#include "object.h"
#include "monster.h"
#include "combat.h"
#include "rng.h"
#include "msg.h"
#include "monsterinit.h"
#include "item.h"
#include "inventory.h"
#include "iteminit.h"

#include <stdio.h>

#define ever ;;

extern Monster* player;
Monster* monster;
TCOD_console_t msgConsole;
int GameState;
TCOD_console_t combatConsole;
MessageList* consoleLog;
MessageList* combatLog;
TCOD_console_t statusPanel;
TCOD_console_t inventoryPanel;
TCOD_key_t key;
Item* items[26];
int keyPressed;

enum{
    STATE_MAP,
    STATE_BATTLE,
    STATE_BATTLEAFTERMATH,
    STATE_INVENTORY,
    STATE_INVENTORYDETAIL,
}state;

void init();
void uninit();
void test();
int handleInput();
void mainLoop();
void printUI();
int battleLoop();
void waitForPress();

int main() {    
    init();

    test();
    
    
    mainLoop();

    
    uninit();

    return 0;
}

void test(){
    Inventory_addItem(player->inventory, Item_clone(ItemList[item_potion]));
    Inventory_addItem(player->inventory, Item_clone(ItemList[item_potion]));
    Inventory_addItem(player->inventory, Item_clone(ItemList[item_potion]));
    Inventory_addItem(player->inventory, Item_clone(ItemList[item_pomfhammer]));
}

void init(){
    MOONMEM_init(2048);
    RNG_init(0);
    MonstersInit();
    ItemsInit();
    consoleLog = Msg_create(15);
    msgConsole = TCOD_console_new(80,20);
    GameState = 0;
    combatLog = Msg_create(15);
    combatConsole = TCOD_console_new(80,40);
    statusPanel = TCOD_console_new(80,2);
    inventoryPanel = TCOD_console_new(80,50);

    player = Monster_playerCreate(20,20);
    TCOD_console_init_root(80,50,"libtcod C tutorial",false,false);
}

void uninit(){
    Monster_delete(player);
    TCOD_console_delete(statusPanel);
    TCOD_console_delete(msgConsole);
    Msg_delete(consoleLog);
    TCOD_console_delete(combatConsole);
    Msg_delete(combatLog);
    TCOD_console_delete(inventoryPanel);
    ItemsUninit();
    MonstersUninit();
    MOONMEM_uninit();
}

void mainLoop(){
    int steps=25;
    int battleCooldown=0;
    int done=0;
    int result;
    
    while(!done){
        if( TCOD_console_is_window_closed() ){
            return;
        }
        printUI();
        if( handleInput(&key) != 0 ){
            steps--;
            battleCooldown--;
        }
        switch(GameState){
            case STATE_MAP:
                if(key.c=='i'){
                    GameState = STATE_INVENTORY;
                }
                if(battleCooldown<=0){
                    if(RNG_roll(1,steps) == 1){
                        steps = 50;
                        battleCooldown=10;
                        if(RNG_roll(1,6)>4){
                            monster = Monster_clone(MonsterList[mob_pig]);
                        } else {
                            monster = Monster_clone(MonsterList[mob_slime]);
                        }

                        Msg_addMessage(consoleLog,"A wild %s appears!",monster->name);
                        printUI();
                        TCOD_sys_wait_for_event(TCOD_EVENT_KEY_RELEASE, NULL, NULL, false);
                        GameState=STATE_BATTLE;

                        result = battleLoop();
                        GameState=STATE_MAP;
                        Monster_delete(monster);
                        if(result == 1) done=1;
                    }
                } 
                break;
            case STATE_INVENTORY:
                keyPressed = key.c-'a';
                if(items[keyPressed]!=NULL){
                    GameState = STATE_INVENTORYDETAIL;
                }
                break;
            case STATE_INVENTORYDETAIL:
                /*waitForPress();*/
                GameState=STATE_MAP;
                break;
        }
        
    }
    printUI();
    waitForPress();
}

void printUI(){
    int x;
    Inventory* head;
    Item* item;

    TCOD_console_clear(NULL);
    TCOD_console_clear(msgConsole);
    TCOD_console_clear(combatConsole);
    TCOD_console_clear(statusPanel);
    TCOD_console_clear(inventoryPanel);
    switch(GameState){
        case STATE_MAP:
            Object_draw(player->object);
            x=0;
            while(x<=15){
                TCOD_console_print(msgConsole,0,x,Msg_getMessage(consoleLog, x));
                x++;
            }
            TCOD_console_print(statusPanel,0,0,"HP: %d/%d  XP: %d",player->combat->hp, player->combat->maxhp, player->xp);
            TCOD_console_blit(msgConsole,0,0,80,20,NULL,0,33,128,128);
            TCOD_console_blit(statusPanel,0,0,80,1,NULL,0,48,128,0);
            break;
        case STATE_BATTLE:
            x=0;
            while(x<=15){
                TCOD_console_print(combatConsole,0,x,Msg_getMessage(combatLog, x));
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
            while(x<=15){
                TCOD_console_print(combatConsole,30,x,Msg_getMessage(combatLog, x));
                x++;
            }
            TCOD_console_blit(combatConsole,0,0,80,40,NULL,0,20,128,255);
            break;
        case STATE_INVENTORY:
            head = player->inventory->next;
            
            int itemchar = 0;
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
            Item_description(item, inventoryPanel);
            TCOD_console_blit(inventoryPanel,0,0,80,50,NULL,5,5,128,255);
            break;
    }
    TCOD_console_flush();
}

int battleLoop(){
    TCOD_key_t key;
    int actiontaken = 0;
    int done = 0;
    int itemchar;
    int keyPressed;

    while(!done){
        actiontaken=0;
        if( TCOD_console_is_window_closed() ){
            break;
        }
        printUI();
        Msg_clear(combatLog);
        
        TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS, &key, NULL, false);
        TCOD_sys_wait_for_event(TCOD_EVENT_KEY_RELEASE, NULL, NULL, false);
        if(key.c == 'a' || key.c == 'A'){
            Msg_addMessage(combatLog, "You attack!");
            Combat_attack(combatLog, player->combat, player->name, monster->combat, monster->name);
            actiontaken=1;
        } else if(key.c == 'h' || key.c == 'H'){
            itemchar=0;
            Inventory* head = player->inventory->next;
            /*Item* items[26];*/
            Msg_addMessage(combatLog,"Use which item?");
            while(head!=NULL){
                Msg_addMessage(combatLog, "[%c] %s x%d",'A'+itemchar,head->item->name, head->quantity);
                items[itemchar] = head->item;
                itemchar++;
                head=head->next;
            }
            Msg_addMessage(combatLog,"[1] Back");
            printUI();
            Msg_clear(combatLog);
            TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS, &key, NULL, false);
            keyPressed = key.c-'a';
            if(items[keyPressed]!=NULL){
                if(items[keyPressed]->type==I_HEALING){
                    Combat_takeDamage(combatLog, player->combat, player->name, -(RNG_roll(1,items[keyPressed]->power)));
                    Inventory_removeItem(player->inventory, items[keyPressed]);
                    actiontaken=1;
                }
            }
           
            
            
        } else if(key.c == 'r' || key.c == 'R'){
            Msg_addMessage(consoleLog,"You ran away!");
            Msg_addMessage(combatLog,"You ran away!");
            done=3;
            break;
        }
        if(actiontaken){
            if(Monster_checkDead(monster)){
                Msg_addMessage(consoleLog,"You win!");
                Msg_addMessage(combatLog,"You win!");
                done= 2;
                break;
            }
            Msg_addMessage(combatLog, "The %s attacks!",monster->name);
            Combat_attack(combatLog, monster->combat, monster->name, player->combat, player->name);
            if(Monster_checkDead(player)){
                Msg_addMessage(consoleLog,"You lose");
                Msg_addMessage(combatLog,"You lose");
                done = 1;
                break;
            }
        }
    }
    printUI();
    waitForPress();
    Msg_clear(combatLog);
    switch(done){
        case 1:
            Msg_addMessage(combatLog, "You've died >:");
            break;
        case 2:
            Msg_addMessage(combatLog, "You win!");
            Msg_addMessage(combatLog, "You gained %d EXP", monster->xp);
            player->xp += monster->xp;
            break;
        case 3:
            Msg_addMessage(combatLog, "You ran away safely");
            break;
    }
    GameState = STATE_BATTLEAFTERMATH;
    printUI();
    Msg_clear(combatLog);
    waitForPress();
    return done;
}

int handleInput(TCOD_key_t* key){
    int action = 0;
    
    
    TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS, key, NULL, false);
    switch(key->vk) {
        case TCODK_KP7 : Object_move(player->object, -1, -1); action=1; break;
        case TCODK_KP9 : Object_move(player->object, 1, -1); action=1; break;
        case TCODK_KP8 : Object_move(player->object, 0, -1); action=1; break;
        case TCODK_KP1 : Object_move(player->object, -1, 1); action=1; break;
        case TCODK_KP2 : Object_move(player->object, 0, 1); action=1; break;
        case TCODK_KP3 : Object_move(player->object, 1, 1); action=1; break;
        case TCODK_KP4 : Object_move(player->object, -1, 0); action=1; break;
        case TCODK_KP6 : Object_move(player->object, 1, 0); action=1; break;
        default:break;
    }
    return action;
}

void waitForPress(){
    TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS, NULL, NULL, false);
    TCOD_sys_wait_for_event(TCOD_EVENT_KEY_RELEASE, NULL, NULL, false);
}