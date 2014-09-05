
#pragma once

#include "Monster.h"





class cPig :
	public cMonster
{
	typedef cMonster super;
	
public:
	cPig(void);

	CLASS_PROTODEF(cPig)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;

	bool IsSaddled(void) const { return m_bIsSaddled; }

private:

	bool m_bIsSaddled;

} ;




