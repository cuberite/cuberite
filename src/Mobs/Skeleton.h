
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
	virtual void MoveToPosition(const Vector3d & a_Position) override;
	virtual void Attack(float a_Dt) override;
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;

	virtual bool IsUndead(void) override { return true; }
	
	bool IsWither(void) const { return m_bIsWither; };

private:

	bool m_bIsWither;

} ;




