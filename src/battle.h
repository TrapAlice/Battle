#ifndef _BATTLE_H
#define _BATTLE_H

#define TITLE "Battle"

void init();
void uninit();
int handleInput();
void test();
void printUI();
void waitForPress();
void generateNewMap();
void changeFloor(int);
void positionPlayer();

typedef enum state_e{
	STATE_MAP,
	STATE_BATTLE,
	STATE_BATTLEAFTERMATH,
	STATE_INVENTORY,
	STATE_INVENTORYDETAIL,
	STATE_EQUIP,
	STATE_SKILLS,
}state_e;

#endif