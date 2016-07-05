
#pragma once





// fwd:
class cMonster;





class cMonsterConfig
{
public:
	cMonsterConfig(void);
	~cMonsterConfig();

	void AssignAttributes(cMonster * a_Monster, const AString & a_Name);

private:
	struct sAttributesStruct;
	struct sMonsterConfigState;
	sMonsterConfigState * m_pState;
	void Initialize();
} ;




