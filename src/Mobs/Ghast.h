
#pragma once

#include "AggressiveMonster.h"





class cGhast :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;

public:
	cGhast(void);

	CLASS_PROTODEF(cGhast)

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual bool Attack(std::chrono::milliseconds a_Dt) override;

	bool IsCharging(void) const { return m_IsCharging; }

private:
	/** Specifies whether or not the ghast has started shooting a fireball. */
	bool m_IsCharging;
	
	/** Number of ticks until a projectile is created.
	Only used while m_IsCharging is true. */
	int m_TicksUntilShot;
} ;




