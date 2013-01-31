#ifndef _SKILLS_H
#define _SKILLS_H

typedef enum skills_e{
	SKILL_NONE,
	SKILL_SWORD,
	SKILL_HAMMER,
	SKILL_CARVING,
	num_skills,
}skills_e;

typedef struct skills_t{
	int  *skillLevel;
	int  *skillXP;
	char *skillActive;
} skills_t;

skills_t*    createSkillSlots      ( );
void         deleteSkillSlots      ( skills_t* );
int          increaseSkill         ( skills_t*, skills_e, int );
int          levelUpSkill          ( skills_t*, skills_e );
const char*  getSkillName          ( skills_e );
int          isSkillActive         ( const skills_t*, skills_e );
int          skillsCurrentlyActive ( const skills_t* );


#endif