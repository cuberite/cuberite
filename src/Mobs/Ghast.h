
#pragma once

#include "AggressiveMonster.h"





class cGhast:
	public cAggressiveMonster
{
	using Super = cAggressiveMonster;

public:

	cGhast();

	CLASS_PROTODEF(cGhast)

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual bool Attack(std::chrono::milliseconds a_Dt) override;
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;

	bool IsCharging(void) const { return m_IsCharging; }

private:
	/** Specifies whether or not the ghast has started shooting a fireball. */
	bool m_IsCharging;

	/** Number of ticks until the ghast tries to fly to another position. */
	int m_FlightCooldown;

	/** Number of ticks until a projectile is created.
	Only used while m_IsCharging is true. */
	int m_TicksUntilShot;
} ;




