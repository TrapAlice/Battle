#ifndef _SKILLS_H
#define _SKILLS_H

enum Skills_e{
	SKILL_SWORD,
	SKILL_HAMMER,
	num_skills,
};

typedef struct{
	int* skillLevel;
	int* skillXP;
} Skills;

Skills* createSkillSlots();
void deleteSkillSlots(Skills*);
int increaseSkill(Skills*, enum Skills_e, int);
int levelUpSkill(Skills*, enum Skills_e);
char* getSkillName(enum Skills_e);


#endif