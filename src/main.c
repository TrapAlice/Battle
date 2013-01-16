#include "libtcod.h"
#include "tap.h"
#include <assert.h>
#include "moonmem.h"
#include "test_ids.h"
#include "object.h"
#include "monster.h"
#include "combat.h"

#define ever ;;

typedef struct{
int x;
int y;
}point;

Object* player;
Monster* monster;

int handleInput();

int main() {    
    //RunTests(1,MEMORY_TEST);
    int steps=25;
    MOONMEM_init(64);
    player = Object_create('@');
    TCOD_console_init_root(80,50,"libtcod C tutorial",false,false);
    for(ever){
        if( TCOD_console_is_window_closed() ){
            break;
        }
        TCOD_console_clear(NULL);
        Object_draw(player);
        TCOD_console_flush();
        if( handleInput() != 0 ){
            steps--;
        }
        if(steps < 26){
            steps = 25;
            monster = Monster_create("Slime");
            printf("A %s attacks!\nIt has %d power and %d defense\n",monster->name, monster->combat->power, monster->combat->defense);
            int x=0;
            while(!Monster_checkDead(monster)){
                x++;
                MOONMEM_memout();
                //MOONMEM_nodeout();
                Combat_attack(player->combat, monster->combat);
                if(x>3){
                    MOONMEM_memdump();
                }
                //Combat_attack(monster->combat, player->combat);
            }
            printf("You win!\n");
            break;
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
        case TCODK_KP7 : Object_move(player, -1, -1); action=1; break;
        case TCODK_KP9 : Object_move(player, 1, -1); action=1; break;
        case TCODK_KP8 : Object_move(player, 0, -1); action=1; break;
        case TCODK_KP1 : Object_move(player, -1, 1); action=1; break;
        case TCODK_KP2 : Object_move(player, 0, 1); action=1; break;
        case TCODK_KP3 : Object_move(player, 1, 1); action=1; break;
        case TCODK_KP4 : Object_move(player, -1, 0); action=1; break;
        case TCODK_KP6 : Object_move(player, 1, 0); action=1; break;
        default:break;
    }
    return action;
}