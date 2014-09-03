
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MonsterConfig.h"
#include "Mobs/Monster.h"
#include "inifile/iniFile.h"





struct cMonsterConfig::sAttributesStruct
{
	AString m_Name;
	int     m_SightDistance;
	int     m_AttackDamage;
	int     m_AttackRange;
	double  m_AttackRate;
	int     m_MaxHealth;
	bool    m_IsFireproof;
};





struct cMonsterConfig::sMonsterConfigState
{
	AString MonsterTypes;
	std::list< sAttributesStruct > AttributesList;
};





cMonsterConfig::cMonsterConfig(void)
	: m_pState( new sMonsterConfigState)
{
	Initialize();
}





cMonsterConfig::~cMonsterConfig()
{
	delete m_pState;
	m_pState = NULL;
}





void cMonsterConfig::Initialize()
{
	cIniFile MonstersIniFile;
	
	if (!MonstersIniFile.ReadFile("monsters.ini"))
	{
		LOGWARNING("%s: Cannot read monsters.ini file, monster attributes not available", __FUNCTION__);
		return;
	}
	
	for (int i = (int)MonstersIniFile.GetNumKeys(); i >= 0; i--)
	{
		sAttributesStruct Attributes;
		AString Name = MonstersIniFile.GetKeyName(i);
		Attributes.m_Name = Name;
		Attributes.m_AttackDamage  = MonstersIniFile.GetValueI(Name, "AttackDamage",  0);
		Attributes.m_AttackRange   = MonstersIniFile.GetValueI(Name, "AttackRange",   0);
		Attributes.m_SightDistance = MonstersIniFile.GetValueI(Name, "SightDistance", 0);
		Attributes.m_AttackRate    = MonstersIniFile.GetValueF(Name, "AttackRate",    0);
		Attributes.m_MaxHealth     = MonstersIniFile.GetValueI(Name, "MaxHealth",     1);
		Attributes.m_IsFireproof   = MonstersIniFile.GetValueB(Name, "IsFireproof",   false);
		m_pState->AttributesList.push_front(Attributes);
	}  // for i - SplitList[]
}





void cMonsterConfig::AssignAttributes(cMonster * a_Monster, const AString & a_Name)
{
	std::list<sAttributesStruct>::const_iterator itr;
	for (itr = m_pState->AttributesList.begin(); itr != m_pState->AttributesList.end(); ++itr)
	{
		if (itr->m_Name.compare(a_Name) == 0)
		{
			a_Monster->SetAttackDamage (itr->m_AttackDamage);
			a_Monster->SetAttackRange  (itr->m_AttackRange);
			a_Monster->SetSightDistance(itr->m_SightDistance);
			a_Monster->SetAttackRate   ((float)itr->m_AttackRate);
			a_Monster->SetMaxHealth    (itr->m_MaxHealth);
			a_Monster->SetIsFireproof  (itr->m_IsFireproof);
			return;
		}
	}  // for itr - m_pState->AttributesList[]
}





