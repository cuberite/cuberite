
#pragma once

#include "PassiveMonster.h"





class cMooshroom :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cMooshroom(void);

	CLASS_PROTODEF(cMooshroom);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	bool    IsBegging     (void) const { return m_IsBegging; }

private:

	bool m_IsBegging;

} ;




