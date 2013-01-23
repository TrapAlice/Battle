#ifndef _BATTLE_H
#define _BATTLE_H

#define TITLE "Battle"

void init();
void uninit();
int handleInput();
void test();
void printUI();
int battleLoop();
void waitForPress();

enum{
    STATE_MAP,
    STATE_BATTLE,
    STATE_BATTLEAFTERMATH,
    STATE_INVENTORY,
    STATE_INVENTORYDETAIL,
}State_e;

#endif