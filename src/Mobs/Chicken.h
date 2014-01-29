#pragma once

#include "PassiveMonster.h"





class cChicken :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cChicken(void);

	CLASS_PROTODEF(cChicken);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	bool    IsBegging     (void) const { return m_IsBegging; }

private:


	int m_EggDropTimer;
	bool    m_IsBegging;
} ;



