
#pragma once

#include "NewMonster.h"





class cNewSkeleton :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewSkeleton(bool IsWither);

	CLASS_PROTODEF(cNewSkeleton)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;\
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	
	bool IsWither(void) const { return m_bIsWither; }

private:

	bool m_bIsWither;

} ;




