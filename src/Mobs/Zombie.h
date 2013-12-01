#pragma once

#include "AggressiveMonster.h"






// tolua_begin
class cZombie :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	// tolua_end
	cZombie(bool IsVillagerZombie);

	CLASS_PROTODEF(cZombie);
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void MoveToPosition(const Vector3f & a_Position) override;

	// tolua_begin
	bool IsVillagerZombie(void) const {return m_bIsVillagerZombie; }
	bool IsConverting(void) const {return m_bIsConverting; }
	// tolua_end

private:

	bool m_bIsVillagerZombie, m_bIsConverting;

} ; // tolua_export




