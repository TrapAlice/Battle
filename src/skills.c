#include "skills.h"
#include "moonmem.h"
#include "msg.h"

Skills* createSkillSlots(){
	Skills* skills = malloc(sizeof(Skills));
	skills->skillLevel = malloc(sizeof(int)*num_skills);
	skills->skillXP = malloc(sizeof(int)*num_skills);
	return skills;
}

void deleteSkillSlots(Skills* skills){
	free(skills->skillLevel);
	free(skills->skillXP);
	free(skills);
}

int increaseSkill(Skills* skills, enum Skills_e skill, int xp){
	skills->skillXP[skill]+=xp;
	if(skills->skillXP[skill]>skills->skillLevel[skill]*10+5){
		return levelUpSkill(skills, skill);
	}
	return 0;
}

int levelUpSkill(Skills* skills, enum Skills_e skill){
	skills->skillXP[skill]-=10;
	skills->skillLevel[skill]++;
	addMessage(globalMessage, " skill is now level %d.", skills->skillLevel[skill]);
	return 1;
}

char* getSkillName(enum Skills_e skills){
	switch(skills){
		case SKILL_SWORD:
			return "Swords";
			break;
		case SKILL_HAMMER:
			return "Hammers";
			break;
	}
	return "";
}