#pragma once

class cMonster;
class cMonsterConfig
{
public:
	cMonsterConfig(void);
	~cMonsterConfig();
	
	// _X: WTF? shouldn't this be static? Setting to OBSOLETE
	OBSOLETE cMonsterConfig *Get();
	
	void AssignAttributes(cMonster *m, const char* n);
	
private:
	struct sAttributesStruct;
	struct sMonsterConfigState;
	sMonsterConfigState* m_pState;
	void Initialize();
};