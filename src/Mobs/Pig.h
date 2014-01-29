
#pragma once

#include "PassiveMonster.h"





class cPig :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cPig(void);

	CLASS_PROTODEF(cPig);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	bool IsSaddled(void) const { return m_bIsSaddled; }
	bool    IsBegging     (void) const { return m_IsBegging; }

private:

	bool m_IsBegging;
	bool m_bIsSaddled;

} ;




