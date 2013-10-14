
#pragma once

#include "AggressiveMonster.h"





class cSkeleton :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cSkeleton(bool IsWither);

	CLASS_PROTODEF(cSkeleton);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	bool IsWither(void) const { return m_bIsWither; };

private:

	bool m_bIsWither;

} ;




