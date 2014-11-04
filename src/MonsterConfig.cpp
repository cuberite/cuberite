
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MonsterConfig.h"
#include "Mobs/Monster.h"
#include "inifile/iniFile.h"





cMonsterConfig::cMonsterConfig(void)
	: m_pState(new sMonsterConfigState)
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





cMonsterConfig::sReturnAttributes cMonsterConfig::ReturnAttributes(const AString & a_Name)
{
	std::list<sAttributesStruct>::const_iterator itr;
	for (itr = m_pState->AttributesList.begin(); itr != m_pState->AttributesList.end(); ++itr)
	{
		if (itr->m_Name == a_Name)
		{
			cMonsterConfig::sReturnAttributes Attr {
				itr->m_SightDistance,
				itr->m_AttackDamage,
				itr->m_AttackRange,
				itr->m_AttackRate,
				itr->m_MaxHealth,
				itr->m_IsFireproof
			};
			return Attr;
		}
	}  // for itr - m_pState->AttributesList[]
}





