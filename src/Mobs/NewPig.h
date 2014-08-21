
#pragma once

#include "NewMonster.h"





class cNewPig :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewPig(void);

	CLASS_PROTODEF(cNewPig)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;

	bool IsSaddled(void) const { return m_bIsSaddled; }

private:

	bool m_bIsSaddled;

} ;




