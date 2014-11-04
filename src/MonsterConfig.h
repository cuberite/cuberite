
#pragma once





// fwd:
class cMonster;





class cMonsterConfig
{
public:
	cMonsterConfig(void);
	~cMonsterConfig();

	struct sReturnAttributes
	{
		int     m_SightDistance;
		int     m_AttackDamage;
		int     m_AttackRange;
		double  m_AttackRate;
		int     m_MaxHealth;
		bool    m_IsFireproof;
	};
	
	sReturnAttributes ReturnAttributes(const AString & a_Name);
	
private:
	struct sAttributesStruct
	{
		AString m_Name;
		int     m_SightDistance;
		int     m_AttackDamage;
		int     m_AttackRange;
		double  m_AttackRate;
		int     m_MaxHealth;
		bool    m_IsFireproof;
	};

	struct sMonsterConfigState
	{
		AString MonsterTypes;
		std::list< sAttributesStruct > AttributesList;
	};

	sMonsterConfigState* m_pState;
	void Initialize();
} ;




