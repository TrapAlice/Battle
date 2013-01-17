#include "libtcod.h"
#include "moonmem.h"
#include "test_ids.h"
#include "object.h"
#include "monster.h"
#include "combat.h"
#include "rng.h"
#include "msg.h"

#define ever ;;

typedef struct{
int x;
int y;
}point;

Monster* player;
Monster* monster;
TCOD_console_t msgConsole;
TCOD_console_t combatConsole;

int movementInput();
void getKeyboardInput(TCOD_key_t* key);
void mainLoop();
int battleLoop();

int main() {    
    //RunTests(1,MEMORY_TEST);
    
    MOONMEM_init(1024);
    Msg_init();
    RNG_init(0);
    msgConsole = TCOD_console_new(80,20);
    TCOD_console_set_default_background(msgConsole,TCOD_red);
    combatConsole = TCOD_console_new(80,20);

    player = Monster_playerCreate(20,20);
    TCOD_console_init_root(80,50,"libtcod C tutorial",false,false);
    mainLoop();
    //battleLoop();
    Monster_delete(player);
    TCOD_console_delete(msgConsole);
    TCOD_console_delete(combatConsole);
    Msg_uninit();
    MOONMEM_uninit();

    return 0;
}

void mainLoop(){
    int steps=25;
    for(ever){
        if( TCOD_console_is_window_closed() ){
            break;
        }
        TCOD_console_clear(NULL);
        TCOD_console_clear(msgConsole);
        Object_draw(player->object);
        int x=0;
        while(x<=15){
            TCOD_console_print(msgConsole,0,x,Msg_getMessage(x));
            x++;
        }
        TCOD_console_blit(msgConsole,0,0,80,20,NULL,0,35,255,128);
        TCOD_console_flush();
        if( movementInput() != 0 ){
            steps--;
        }
        if(RNG_roll(1,steps)>0){
            steps = 50;
            monster = Monster_create("Slime");

            Msg_addMessage("A %s attacks!","Slime");

            /*for(ever){
                Combat_attack(player->combat, player->name, monster->combat, monster->name);
                if(Monster_checkDead(monster)) break;
                Combat_attack(monster->combat, monster->name, player->combat, player->name);
                if(Monster_checkDead(player)){
                    Msg_addMessage("You lose");
                    Monster_delete(monster);
                    return;
                }
            }*/
            int result = battleLoop();
            Monster_delete(monster);
            if(result == 0) break;
        }
    }
}

int battleLoop(){
    TCOD_key_t key;
    int actiontaken = 0;
    for(ever){
        actiontaken=0;
        if( TCOD_console_is_window_closed() ){
            break;
        }
        TCOD_console_clear(NULL);
        TCOD_console_clear(combatConsole);
        TCOD_console_print(combatConsole,0,0,"HP: %d/%d",player->combat->hp, player->combat->maxhp);
        TCOD_console_print(combatConsole,0,2,"[A] Attack");
        TCOD_console_print(combatConsole,0,3,"[H] Heal");
        TCOD_console_print(combatConsole,0,4,"[R] Run");
        TCOD_console_blit(combatConsole,0,0,80,20,NULL,0,35,128,255);
        TCOD_console_flush();
        getKeyboardInput(&key);
        if(key.c == 'a' || key.c == 'A'){
            Combat_attack(player->combat, player->name, monster->combat, monster->name);
            actiontaken=1;
        } else if(key.c == 'h' || key.c == 'H'){
            Combat_takeDamage(player->combat, player->name, -(RNG_roll(2,6)));
            actiontaken=1;
        } else if(key.c == 'r' || key.c == 'R'){
            Msg_addMessage("You ran away!");
            return 1;
            break;
        }
        if(actiontaken){
            if(Monster_checkDead(monster)){
                Msg_addMessage("You win!");
                return 1;
                break;
            }
            Combat_attack(monster->combat, monster->name, player->combat, player->name);
            if(Monster_checkDead(player)){
                Msg_addMessage("You lose");
                return 0;
            }
        }
    }
    return 0;
}

void getKeyboardInput(TCOD_key_t* key){
    TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS, key, NULL, false);
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