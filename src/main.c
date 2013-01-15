#include "libtcod.h"
#include "tap.h"
#include <assert.h>
#include "moonmem.h"
#include "test_ids.h"
#include "object.h"

#define ever ;;

typedef struct{
int x;
int y;
}point;

Object* player;

void handleInput();

int main() {    
    //RunTests(1,MEMORY_TEST);
    MOONMEM_init(32);
    player = Object_create('@');
    TCOD_console_init_root(80,50,"libtcod C tutorial",false,false);
    for(ever){
        if( TCOD_console_is_window_closed() ){
            break;
        }
        TCOD_console_clear(NULL);
        //TCOD_console_put_char( NULL, 20, 20, '@', TCOD_BKGND_NONE);
        Object_draw(player);
        TCOD_console_flush();
        handleInput();
    }

    MOONMEM_uninit();
    return 0;
}

void handleInput(){
    TCOD_key_t key;
    TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS, &key, NULL, false);
    switch(key.vk) {
        case TCODK_KP7 : Object_move(player, -1, -1); break;
        case TCODK_KP9 : Object_move(player, 1, -1); break;
        case TCODK_KP8 : Object_move(player, 0, -1); break;
        case TCODK_KP1 : Object_move(player, -1, 1); break;
        case TCODK_KP2 : Object_move(player, 0, 1); break;
        case TCODK_KP3 : Object_move(player, 1, 1); break;
        case TCODK_KP4 : Object_move(player, -1, 0); break;
        case TCODK_KP6 : Object_move(player, 1, 0); break;
        default:break;
    }
}