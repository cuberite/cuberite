
#pragma once

#include "AggressiveMonster.h"





class cBlaze :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;

public:
	cBlaze(void);

	CLASS_PROTODEF(cBlaze)

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual bool Attack(std::chrono::milliseconds a_Dt) override;

private:
	// Specifies whether or not the blaze has started shooting fireballs
	bool m_bIsCharging;

	// Used for timing when shooting three fireballs in a row
	int m_ChargeTimer;
} ;
