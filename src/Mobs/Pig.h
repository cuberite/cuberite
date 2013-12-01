
#pragma once

#include "PassiveMonster.h"





// tolua_begin
class cPig :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	// tolua_end
	cPig(void);

	CLASS_PROTODEF(cPig);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	
	// tolua_begin
	bool IsSaddled(void) const { return m_bIsSaddled; }
	// tolua_end

private:

	bool m_bIsSaddled;
} ; // tolua_export



