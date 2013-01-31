#include "skills.h"
#include "moonmem.h"
#include "msg.h"

skills_t* createSkillSlots(){
	skills_t* skills = malloc(sizeof(skills_t));
	skills->skillLevel = malloc(sizeof(int)*num_skills);
	skills->skillXP = malloc(sizeof(int)*num_skills);
	skills->skillActive = malloc(sizeof(char)*num_skills);
	return( skills );
}

void deleteSkillSlots(skills_t* const skills){
	free(skills->skillLevel);
	free(skills->skillXP);
	free(skills->skillActive);
	free(skills);
}

void increaseSkill(skills_t* const skills, skills_e skill, int xp){
	skills->skillXP[skill]+=xp;
	if( skills->skillXP[skill]>skills->skillLevel[skill]*10+5 ){
		levelUpSkill(skills, skill);
	}
}

void levelUpSkill(skills_t* const skills, skills_e skill){
	skills->skillXP[skill]-=10;
	skills->skillLevel[skill]++;
	if( skills->skillLevel[skill]==1 ){
		addMessage(globalMessage, "You learnt the skill: %s.", getSkillName(skill));
	} else {
		addMessage(globalMessage, "%s skill is now level %d.", getSkillName(skill), skills->skillLevel[skill]);
	}
}

const char* getSkillName(skills_e skills){
	switch(skills){
		case SKILL_SWORD:
			return( "Swords" );
		case SKILL_HAMMER:
			return( "Hammers" );
		case SKILL_CARVING:
			return( "Carving" );
		case SKILL_SHIELD:
			return( "Shield" );
		case SKILL_NONE:
		case num_skills:
			return( "" );
	}
	return( "" );
}

int isSkillActive(const skills_t* const skills, skills_e skill){
	if( skills->skillLevel[skill] < 1 ){
		return (1);
	}
	return( skills->skillActive[skill] );
}

int skillsCurrentlyActive(const skills_t* const skills){
	int x=1;
	int amount=0;
	for( ; x<num_skills; ++x ){
		amount+=skills->skillActive[x];
	}
	return( amount );
}

void increaseSkillifActive(skills_t* const skills, skills_e skill, int xp ){
	if( isSkillActive(skills, skill) ){
		increaseSkill(skills, skill, xp);
	}
}

int getSkillLevelifActive(const skills_t* const skills, skills_e skill){
	if( !skills ) return( 0 );
	if( isSkillActive(skills, skill) ){
		return skills->skillLevel[skill];
	}
	return( 0 );
}