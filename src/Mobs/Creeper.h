
#pragma once

#include "AggressiveMonster.h"





class cCreeper :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cCreeper(void);

	CLASS_PROTODEF(cCreeper);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;

	bool IsBlowing(void) const {return m_bIsBlowing; }
	bool IsCharged(void) const {return m_bIsCharged; }

private:

	bool m_bIsBlowing, m_bIsCharged;

} ;




