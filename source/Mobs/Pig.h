
#pragma once

#include "PassiveMonster.h"





class cPig :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cPig(void);

	CLASS_PROTODEF(cPig);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	bool IsSaddled(void) const { return m_bIsSaddled; }

private:

	bool m_bIsSaddled;

} ;




