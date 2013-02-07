#ifndef _SKILLS_H
#define _SKILLS_H

#include "libtcod.h"

typedef enum skills_e{
	SKILL_NONE,
	SKILL_SWORD,
	SKILL_HAMMER,
	SKILL_CARVING,
	SKILL_SHIELD,
	SKILL_STEALTH,
	SKILL_LOCKPICKING,
	num_skills,
}skills_e;

typedef struct skills_t{
	int  *skillLevel;
	int  *skillXP;
	char *skillActive;
} skills_t;

skills_t*    createSkillSlots      ( );
void         deleteSkillSlots      ( skills_t* );
void         increaseSkill         ( skills_t*, skills_e, int );
void         levelUpSkill          ( skills_t*, skills_e );
const char*  getSkillName          ( skills_e );
int          isSkillActive         ( const skills_t*, skills_e );
int          skillsCurrentlyActive ( const skills_t* );
void         increaseSkillifActive ( skills_t*, skills_e, int );
int          getSkillLevelifActive ( const skills_t*, skills_e );
int          skillCheck            ( const skills_t*, skills_e, int );
void         showSkills            ( const skills_t*, TCOD_console_t );


#endif