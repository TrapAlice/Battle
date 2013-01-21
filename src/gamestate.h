#ifndef _GAMESTATE_H
#define _GAMESTATE_H

typedef enum State_e{
    STATE_MAP,
    STATE_BATTLE,
    STATE_BATTLEAFTERMATH,
    STATE_INVENTORY,
    STATE_INVENTORYDETAIL,
};

extern enum State_e GameState;


#endif