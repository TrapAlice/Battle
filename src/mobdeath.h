#ifndef _MOBDEATH_H
#define _MOBDEATH_H

struct monster_t;

void  standardDeath ( const struct monster_t* );
void  pigDeath      ( const struct monster_t* );
void  fairyDeath    ( const struct monster_t* );

#endif