#pragma once

class cMonster;
class cMonsterConfig
{
public:
	cMonsterConfig(int TypeC);
	~cMonsterConfig();
	cMonsterConfig *Get();
	
	void AssignAttributes(cMonster *m, const char* n);
	
private:
	struct sAttributesStruct;
	struct sMonsterConfigState;
	sMonsterConfigState* m_pState;
	void Initialize();
};