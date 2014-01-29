
#pragma once

#include "PassiveMonster.h"





class cCow :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cCow();

	CLASS_PROTODEF(cCow);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	bool    IsBegging     (void) const { return m_IsBegging; }

private:

	bool m_IsBegging;

} ;




