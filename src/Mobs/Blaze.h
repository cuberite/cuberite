
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
	/** Specifies whether or not the blaze has started shooting fireballs. */
	bool m_IsCharging;

	/** Number of ticks since the blaze started charging.
	Used to create 3 successive projectiles. */
	int m_ChargeTimer;
} ;
