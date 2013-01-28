#include "skills.h"
#include "moonmem.h"
#include "msg.h"

skills_t* createSkillSlots(){
	skills_t* skills = malloc(sizeof(skills_t));
	skills->skillLevel = malloc(sizeof(int)*num_skills);
	skills->skillXP = malloc(sizeof(int)*num_skills);
	skills->skillActive = malloc(sizeof(char)*num_skills);
	return skills;
}

void deleteSkillSlots(skills_t* skills){
	free(skills->skillLevel);
	free(skills->skillXP);
	free(skills->skillActive);
	free(skills);
}

int increaseSkill(skills_t* skills, Skills_e skill, int xp){
	skills->skillXP[skill]+=xp;
	if(skills->skillXP[skill]>skills->skillLevel[skill]*10+5){
		return levelUpSkill(skills, skill);
	}
	return 0;
}

int levelUpSkill(skills_t* skills, Skills_e skill){
	skills->skillXP[skill]-=10;
	skills->skillLevel[skill]++;
	addMessage(globalMessage, "%s skill is now level %d.", getSkillName(skill), skills->skillLevel[skill]);
	return 1;
}

char* getSkillName(Skills_e skills){
	switch(skills){
		case SKILL_SWORD:
			return "Swords";
		case SKILL_HAMMER:
			return "Hammers";
		case SKILL_CARVING:
			return "Carving";
		case SKILL_NONE:
		case num_skills:
			return "";
	}
	return "";
}