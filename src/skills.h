#ifndef _SKILLS_H
#define _SKILLS_H

typedef enum Skills_e{
	SKILL_NONE,
	SKILL_SWORD,
	SKILL_HAMMER,
	SKILL_CARVING,
	num_skills,
}Skills_e;

typedef struct{
	int* skillLevel;
	int* skillXP;
} Skills;

Skills* createSkillSlots();
void deleteSkillSlots(Skills*);
int increaseSkill(Skills*, Skills_e, int);
int levelUpSkill(Skills*, Skills_e);
char* getSkillName( Skills_e);


#endif