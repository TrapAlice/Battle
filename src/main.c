#include "libtcod.h"
#include "moonmem.h"
#include "test_ids.h"
#include "object.h"
#include "monster.h"
#include "combat.h"
#include "rng.h"

#define ever ;;

typedef struct{
int x;
int y;
}point;

Monster* player;
Monster* monster;

int handleInput();

int main() {    
    //RunTests(1,MEMORY_TEST);
    
    MOONMEM_init(128);
    RNG_init(0);
    
    player = Monster_playerCreate(20,20);
    TCOD_console_init_root(80,50,"libtcod C tutorial",false,false);
    int steps=25;
    for(ever){
        if( TCOD_console_is_window_closed() ){
            break;
        }
        TCOD_console_clear(NULL);
        Object_draw(player->object);
        TCOD_console_flush();
        if( handleInput() != 0 ){
            steps--;
        }
        if(RNG_roll(1,steps)==1){
            steps = 50;
            monster = Monster_create("Slime");

            printf("A %s attacks!\n", monster->name);
            for(ever){
                Combat_attack(player->combat, player->name, monster->combat, monster->name);
                if(Monster_checkDead(monster)) break;
                Combat_attack(monster->combat, monster->name, player->combat, player->name);
                if(Monster_checkDead(player)){
                    printf("You died!\n");
                    exit(0);
                }
            }
            printf("You win!\n");
            Monster_delete(monster);
        }
    }

    MOONMEM_uninit();
    return 0;
}

int handleInput(){
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