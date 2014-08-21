
#pragma once

#include "Monster.h"





class cSkeleton :
	public cMonster
{
	typedef cMonster super;
	
public:
	cSkeleton(bool IsWither);

	CLASS_PROTODEF(cSkeleton)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) /*override*/;\
	virtual void SpawnOn(cClientHandle & a_ClientHandle) /*override*/;
	
	bool IsWither(void) const { return m_bIsWither; }

private:

	bool m_bIsWither;

} ;




