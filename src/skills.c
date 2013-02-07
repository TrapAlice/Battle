#include "skills.h"
#include "moonmem.h"
#include "msg.h"
#include "rng.h"

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
		case SKILL_STEALTH:
			return( "Stealth" );
		case SKILL_LOCKPICKING:
			return( "Lockpicking" );
		case SKILL_NONE:
		case num_skills:
			return( "" );
	}
	return( "" );
}

int isSkillActive(const skills_t* const skills, skills_e skill){
	checknotnull(skills);
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
	checknotnull(skills);
	if( isSkillActive(skills, skill) ){
		return skills->skillLevel[skill];
	}
	return( 0 );
}

int skillCheck(const skills_t* const skills, skills_e skill, int dc){
	int skillLevel = getSkillLevelifActive(skills, skill);
	int result = roll(skillLevel < 1 ? 1 : skillLevel, 6);
	return( result > dc );
}

void showSkills(const skills_t* const skills, TCOD_console_t panel){
	int itemchar=0;
	int x;
	TCOD_console_set_default_foreground(panel,TCOD_yellow);
	TCOD_console_print(panel, 0, 0, "[Active]");
	TCOD_console_set_default_foreground(panel,TCOD_dark_grey);
	TCOD_console_print(panel, 10, 0, "[Inactive]");
	TCOD_console_print(panel, 0, 1, "Max active skills: 2");

	for( x=0; x<num_skills; ++x ){
		if( skills->skillLevel[x]>0 ){
			if( isSkillActive(skills, x) ){
				TCOD_console_set_default_foreground(panel, TCOD_yellow);
			}
			else{
				TCOD_console_set_default_foreground(panel,TCOD_dark_grey);
			}
			TCOD_console_print(panel, 0, itemchar+3, "%c] %s", 
			  itemchar+'A', getSkillName(x));
			TCOD_console_print(panel, 13, itemchar+3, "- %d", 
			  skills->skillLevel[x]);
			itemchar++;
		}
			}
}