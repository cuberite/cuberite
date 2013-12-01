
#pragma once

#include "AggressiveMonster.h"





// tolua_begin
class cSkeleton :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	// tolua_end
	cSkeleton(bool IsWither);

	CLASS_PROTODEF(cSkeleton);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void MoveToPosition(const Vector3f & a_Position) override;
	virtual void Attack(float a_Dt) override;
	
	// tolua_begin
	bool IsWither(void) const { return m_bIsWither; };
	// tolua_end

private:

	bool m_bIsWither;

} ; // tolua_export




