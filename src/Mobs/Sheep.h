
#pragma once

#include "PassiveMonster.h"





// tolua_begin
class cSheep :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	// tolua_end
	cSheep(int a_Color);
	
	CLASS_PROTODEF(cSheep);
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	
	// tolua_begin
	bool IsSheared(void) const { return m_IsSheared; }
	int GetFurColor(void) const { return m_WoolColor; }
	// tolua_end

private:
	
	bool m_IsSheared;
	int m_WoolColor;
} ; // tolua_export




