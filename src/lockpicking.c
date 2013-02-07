#include "lockpicking.h"
#include "libtcod.h"
#include "moonmem.h"
#include "rng.h"
int x=5;
char* locks;
void startLockpicking(){
	TCOD_key_t key;
	int picked=0;
	int attempts=0;
	int y;
	locks = malloc(sizeof(char)*10);
	int unlock = between(1,9);
	
	for( y=0; y<10; ++y ){
		locks[y]='*';
	}

	while( !picked ){
		if( TCOD_console_is_window_closed() ){
			break;
		}
		displayLockpicking();
		TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS, &key, NULL, false);
		switch( key.vk ){
			case TCODK_KP8:
				x--;
				break;
			case TCODK_KP2:
				x++;
				break;
			case TCODK_KP6:
				if( unlock == x ){
					picked=1;
				} else {
					if( unlock > x ){
						locks[x] = '\\';
					} else {
						locks[x] = '/';
					}
				}
				break;
			default:
				break;
		}
		x = x > 0 ? x : 0;
		x = x < 9 ? x : 9;
	}

	free(locks);
	
}

void displayLockpicking(){
	int y;
	TCOD_console_clear(NULL);
	TCOD_console_put_char(NULL, 40, 10+x, '>', TCOD_BKGND_NONE);
	for( y=0; y<10; ++y){
		if( locks[y]=='/' || locks[y]=='\\' ){
			TCOD_console_put_char_ex(NULL, 41, 10+y, locks[y], TCOD_black, TCOD_red );
		} else {
			TCOD_console_put_char(NULL, 41, 10+y, locks[y], TCOD_BKGND_NONE );
		}
	}
	
	TCOD_console_flush();
}