
#pragma once

#include "AggressiveMonster.h"





// tolua_begin
class cCreeper :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	// tolua_end

	cCreeper(void);

	CLASS_PROTODEF(cCreeper);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;

	// tolua_begin
	bool IsBlowing(void) const {return m_bIsBlowing; }
	bool IsCharged(void) const {return m_bIsCharged; }
	// tolua_end

private:

	bool m_bIsBlowing, m_bIsCharged;
} ; // tolua_export



