
#pragma once

#include "Monster.h"





class cCreeper :
	public cMonster
{
	typedef cMonster super;
	
public:
	cCreeper(void);

	CLASS_PROTODEF(cCreeper)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;

	bool IsBlowing(void) const {return m_bIsBlowing; }
	bool IsCharged(void) const {return m_bIsCharged; }

private:

	bool m_bIsBlowing, m_bIsCharged/*, m_BurnedWithFlintAndSteel*/;
	int m_ExplodingTimer;

} ;




