#include "cMonsterConfig.h"
#include "cMonster.h"
#include "../iniFile/iniFile.h"
#include <list>
#include <vector>
#include <cstdio>
//#include "../source/cprintf.h"
#include <string>
using namespace std;

extern std::vector<std::string> StringSplit(std::string str, std::string delim);

struct cMonsterConfig::sAttributesStruct
{
	string m_name;
	float m_SightDistance;
	float m_AttackDamage;
	float m_AttackRange;
	float m_AttackRate;
	int m_MaxHealth;
};

struct cMonsterConfig::sMonsterConfigState
{
	int TypeCount;
	string MonsterTypes;
	list< sAttributesStruct > AttributesList;
};

cMonsterConfig::cMonsterConfig(int TypeC)
	: m_pState( new sMonsterConfigState )
{
	m_pState->TypeCount = TypeC;
	Initialize();
}

cMonsterConfig::~cMonsterConfig() {
	delete m_pState;
}

void cMonsterConfig::Initialize() {
	
	sAttributesStruct Attributes;
	cIniFile SettingsIniFile("settings.ini");
	cIniFile MonstersIniFile("monsters.ini");
	
	if(!SettingsIniFile.ReadFile() || !MonstersIniFile.ReadFile()) {
		printf("Error: Must have both settings.ini and monsters.ini to configure attributes\n\tusing default attributes \n");
		return;
	}
	
	m_pState->MonsterTypes = SettingsIniFile.GetValue("Monsters","Types","");
	
	if( m_pState->MonsterTypes.empty() ) {
		printf("Error: No Monster types listed in config file, using default attributes \n");
		return;
	}
	
	vector<string> SplitList = StringSplit(m_pState->MonsterTypes,",");
	for(unsigned int i = 0; i < SplitList.size(); ++i) {
		if(!SplitList[i].empty()) {
			printf("Getting Attributes for: %s \n",SplitList[i].c_str());
			Attributes.m_name = SplitList[i].c_str();
			Attributes.m_AttackDamage = (float)MonstersIniFile.GetValueF(SplitList[i].c_str(),"AttackDamage",0);
			printf("Got AttackDamage: %3.3f \n",Attributes.m_AttackDamage);
			Attributes.m_AttackRange = (float)MonstersIniFile.GetValueF(SplitList[i].c_str(),"AttackRange",0);
			printf("Got AttackRange: %3.3f \n",Attributes.m_AttackRange);
			Attributes.m_SightDistance = (float)MonstersIniFile.GetValueF(SplitList[i].c_str(),"SightDistance",0);
			printf("Got SightDistance: %3.3f \n",Attributes.m_SightDistance);
			Attributes.m_AttackRate = (float)MonstersIniFile.GetValueF(SplitList[i].c_str(),"AttackRate",0);
			printf("Got AttackRate: %3.3f \n",Attributes.m_AttackRate);
			Attributes.m_MaxHealth = MonstersIniFile.GetValueI(SplitList[i].c_str(),"MaxHealth",0);
			printf("Got MaxHealth: %d \n",Attributes.m_MaxHealth);
			m_pState->AttributesList.push_front(Attributes);
		}
	}
	
}

void cMonsterConfig::AssignAttributes(cMonster *m, const char* n)
{
	list<sAttributesStruct>::iterator itr;
	for(itr = m_pState->AttributesList.begin(); itr != m_pState->AttributesList.end(); ++itr) {
		if(itr->m_name.compare(n) == 0) {
			//printf("found my attribs: %s :\n",itr->m_name.c_str());
			m->SetAttackDamage(itr->m_AttackDamage);
			m->SetAttackRange(itr->m_AttackRange);
			m->SetSightDistance(itr->m_SightDistance);
			m->SetAttackRate((int)itr->m_AttackRate);
			m->SetMaxHealth((int)itr->m_MaxHealth);
		}
	}
}

cMonsterConfig *cMonsterConfig::Get() {
	return this;
}
