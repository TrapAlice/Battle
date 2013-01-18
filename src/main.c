#include "libtcod.h"
#include "moonmem.h"
#include "test_ids.h"
#include "object.h"
#include "monster.h"
#include "combat.h"
#include "rng.h"
#include "msg.h"

#define ever ;;

extern Monster* player;
Monster* monster;
TCOD_console_t msgConsole;
int GameState;
TCOD_console_t combatConsole;
MessageList* consoleLog;
MessageList* combatLog;
TCOD_console_t statusPanel;

int movementInput();
void getKeyboardInput(TCOD_key_t* key);
void mainLoop();
void printUI();
int battleLoop();
void waitForPress();

int main() {    
    //RunTests(1,MEMORY_TEST);
    
    MOONMEM_init(1024);
    RNG_init(0);
    consoleLog = Msg_create(15);
    msgConsole = TCOD_console_new(80,20);
    GameState = 0;
    TCOD_console_set_default_background(msgConsole,TCOD_red);
    combatLog = Msg_create(15);
    combatConsole = TCOD_console_new(80,40);
    statusPanel = TCOD_console_new(20,30);
    TCOD_console_set_default_background(statusPanel,TCOD_blue);

    player = Monster_playerCreate(20,20);
    TCOD_console_init_root(80,50,"libtcod C tutorial",false,false);
    mainLoop();
    //battleLoop();
    Monster_delete(player);
    TCOD_console_delete(statusPanel);
    TCOD_console_delete(msgConsole);
    Msg_delete(consoleLog);
    TCOD_console_delete(combatConsole);
    Msg_delete(combatLog);
    MOONMEM_uninit();

    return 0;
}

void mainLoop(){
    int steps=25;
    int battleCooldown=0;
    int done=0;
    while(!done){
        if( TCOD_console_is_window_closed() ){
            break;
        }
        printUI();
        if( movementInput() != 0 ){
            steps--;
            battleCooldown--;
        }
        if(battleCooldown<=0){
            if(RNG_roll(1,steps) == 1){
                steps = 50;
                battleCooldown=10;
                monster = Monster_create("Slime",15);

                Msg_addMessage(consoleLog,"A %s appears!",monster->name);
                printUI();
                TCOD_sys_wait_for_event(TCOD_EVENT_KEY_RELEASE, NULL, NULL, false);
                GameState=1;

                int result = battleLoop();
                GameState=0;
                Monster_delete(monster);
                if(result == 1) done=1;
            }
        } 
    }
    printUI();
    waitForPress();
}

void printUI(){
    TCOD_console_clear(NULL);
    TCOD_console_clear(msgConsole);
    TCOD_console_clear(combatConsole);
    TCOD_console_clear(statusPanel);
    int x;
    switch(GameState){
        case 0:
            Object_draw(player->object);
            x=0;
            while(x<=15){
                TCOD_console_print(msgConsole,0,x,Msg_getMessage(consoleLog, x));
                x++;
            }
            TCOD_console_print(statusPanel,0,0,"HP: %d/%d",player->combat->hp, player->combat->maxhp);
            TCOD_console_print(statusPanel,0,1,"EXP: %d",0);
            TCOD_console_blit(msgConsole,0,0,80,20,NULL,0,35,255,128);
            TCOD_console_blit(statusPanel,0,0,20,30,NULL,60,10,128,0);
            break;
        case 1:
            x=0;
            while(x<=15){
                TCOD_console_print(combatConsole,0,x,Msg_getMessage(combatLog, x));
                x++;
            }
            TCOD_console_print(combatConsole,0,20,"HP: %d/%d",player->combat->hp, player->combat->maxhp);
            TCOD_console_print(combatConsole,0,22,"[A] Attack");
            TCOD_console_print(combatConsole,0,23,"[H] Heal");
            TCOD_console_print(combatConsole,0,24,"[R] Run");
            TCOD_console_blit(combatConsole,0,0,80,40,NULL,0,5,128,255);
            break;
        case 2:
            x=0;
            while(x<=15){
                TCOD_console_print(combatConsole,30,x,Msg_getMessage(combatLog, x));
                x++;
            }
            TCOD_console_blit(combatConsole,0,0,80,40,NULL,0,20,128,255);
            break;
    }
    TCOD_console_flush();
}

int battleLoop(){
    TCOD_key_t key;
    int actiontaken = 0;
    int done = 0;
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
            Combat_takeDamage(combatLog, player->combat, player->name, -(RNG_roll(2,6)));
            actiontaken=1;
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
    GameState = 2;
    printUI();
    Msg_clear(combatLog);
    waitForPress();
    return done;
}

int movementInput(){
    int action = 0;
    TCOD_key_t key;
    TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS, &key, NULL, false);
    switch(key.vk) {
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