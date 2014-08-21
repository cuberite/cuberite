
#pragma once

#include "NewMonster.h"





class cNewCreeper :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewCreeper(void);

	CLASS_PROTODEF(cNewCreeper)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;

	bool IsBlowing(void) const {return m_bIsBlowing; }
	bool IsCharged(void) const {return m_bIsCharged; }

private:

	bool m_bIsBlowing, m_bIsCharged/*, m_BurnedWithFlintAndSteel*/;
	int m_ExplodingTimer;

} ;




