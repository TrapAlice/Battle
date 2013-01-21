#include "controls.h"

void handleInput(TCOD_key_t* key){
    TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS, key, NULL, false);
}

void waitForPress(){
    TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS, NULL, NULL, false);
    TCOD_sys_wait_for_event(TCOD_EVENT_KEY_RELEASE, NULL, NULL, false);
}