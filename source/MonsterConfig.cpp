
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MonsterConfig.h"
#include "Mobs/Monster.h"
#include "../iniFile/iniFile.h"
//#include <cstdio>





struct cMonsterConfig::sAttributesStruct
{
	AString m_name;
	float   m_SightDistance;
	float   m_AttackDamage;
	float   m_AttackRange;
	float   m_AttackRate;
	int     m_MaxHealth;
};





struct cMonsterConfig::sMonsterConfigState
{
	AString MonsterTypes;
	std::list< sAttributesStruct > AttributesList;
};





cMonsterConfig::cMonsterConfig(void)
	: m_pState( new sMonsterConfigState )
{
	Initialize();
}





cMonsterConfig::~cMonsterConfig() {
	delete m_pState;
}





void cMonsterConfig::Initialize() {
	
	sAttributesStruct Attributes;
	cIniFile SettingsIniFile("settings.ini");
	cIniFile MonstersIniFile("monsters.ini");
	
	if (!SettingsIniFile.ReadFile() || !MonstersIniFile.ReadFile())
	{
		LOGWARNING("cMonsterConfig: Must have both settings.ini and monsters.ini to configure attributes\n\tusing default attributes \n");
		return;
	}
	
	m_pState->MonsterTypes = SettingsIniFile.GetValue("Monsters","Types","");
	
	if ( m_pState->MonsterTypes.empty() )
	{
		LOGWARNING("cMonsterConfig: No Monster types listed in config file, using default attributes \n");
		return;
	}
	
	AStringVector SplitList = StringSplit(m_pState->MonsterTypes,",");
	for (unsigned int i = 0; i < SplitList.size(); ++i)
	{
		if (!SplitList[i].empty())
		{
			Attributes.m_name = SplitList[i].c_str();
			Attributes.m_AttackDamage  = (float)MonstersIniFile.GetValueF(SplitList[i].c_str(), "AttackDamage",  0);
			Attributes.m_AttackRange   = (float)MonstersIniFile.GetValueF(SplitList[i].c_str(), "AttackRange",   0);
			Attributes.m_SightDistance = (float)MonstersIniFile.GetValueF(SplitList[i].c_str(), "SightDistance", 0);
			Attributes.m_AttackRate    = (float)MonstersIniFile.GetValueF(SplitList[i].c_str(), "AttackRate",    0);
			Attributes.m_MaxHealth     =        MonstersIniFile.GetValueI(SplitList[i].c_str(), "MaxHealth",     0);
			m_pState->AttributesList.push_front(Attributes);
		}
	}  // for i - SplitList[]
}





void cMonsterConfig::AssignAttributes(cMonster *m, const char* n)
{
	std::list<sAttributesStruct>::const_iterator itr;
	for (itr = m_pState->AttributesList.begin(); itr != m_pState->AttributesList.end(); ++itr)
	{
		if(itr->m_name.compare(n) == 0)
		{
			m->SetAttackDamage (itr->m_AttackDamage);
			m->SetAttackRange  (itr->m_AttackRange);
			m->SetSightDistance(itr->m_SightDistance);
			m->SetAttackRate   ((int)itr->m_AttackRate);
			m->SetMaxHealth    ((short)itr->m_MaxHealth);
		}
	}  // for itr - m_pState->AttributesList[]
}





