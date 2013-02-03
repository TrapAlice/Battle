#ifndef _MOBFUNCTIONS_H
#define _MOBFUNCTIONS_H

struct monster_t;

/*Birth functions*/
void  equipMonster  ( struct monster_t* const );

/*Attack functions*/
void  fairyAttack   ( struct monster_t*, struct monster_t* );

/*Death functions*/
void  standardDeath ( const struct monster_t* );
void  carveOnDeath  ( const struct monster_t* );
void  fairyDeath    ( const struct monster_t* );

#endif