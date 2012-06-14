#pragma once

class cMonster;
class cMonsterConfig
{
public:
	cMonsterConfig(void);
	~cMonsterConfig();
	
	void AssignAttributes(cMonster *m, const char* n);
	
private:
	struct sAttributesStruct;
	struct sMonsterConfigState;
	sMonsterConfigState* m_pState;
	void Initialize();
};