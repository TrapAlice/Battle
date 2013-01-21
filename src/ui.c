#include "ui.h"
#include "gamestate.h"
#include "monster.h"
#include "msg.h"
#include "item.h"
#include "inventory.h"
#include "controls.h"

extern Monster* player;
extern enum State_e GameState;
extern MessageList* consoleLog;
extern MessageList* combatLog;
extern TCOD_key_t key;

TCOD_console_t msgConsole;
TCOD_console_t combatConsole;
TCOD_console_t statusPanel;
TCOD_console_t inventoryPanel;

void printUI(){
	int x;
    Inventory* head;
    Item* item;
    int itemchar = 0;

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
            while( head != NULL ){
                if(head->item->stackable){
                    TCOD_console_print(inventoryPanel,0,itemchar,"[%c] %s x%d",'A'+itemchar, head->item->name, head->quantity);
                } else {
                    TCOD_console_print(inventoryPanel,0,itemchar,"[%c] %s",'A'+itemchar, head->item->name);
                }
                /*items[itemchar]=head->item;*/
                head=head->next;
                
                itemchar++;
            }
            TCOD_console_blit(inventoryPanel,0,0,80,50,NULL,5,5,128,255);
            break;
        case STATE_INVENTORYDETAIL:
            item = items[key.c-'a'];
            Item_description(item, inventoryPanel);
            TCOD_console_blit(inventoryPanel,0,0,80,50,NULL,5,5,128,255);
            break;
    }
    TCOD_console_flush();
}