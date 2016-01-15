
#pragma once

#include "AggressiveMonster.h"





class cWither :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;

public:
	cWither(void);

	CLASS_PROTODEF(cWither)

	unsigned int GetWitherInvulnerableTicks(void) const { return m_WitherInvulnerableTicks; }

	void SetWitherInvulnerableTicks(unsigned int a_Ticks) { m_WitherInvulnerableTicks = a_Ticks; }

	/** Returns whether the wither is invulnerable to arrows. */
	bool IsArmored(void) const;

	// cEntity overrides
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void KilledBy(TakeDamageInfo & a_TDI) override;

	virtual bool IsUndead(void) override { return true; }

private:

	/** The number of ticks of invulnerability left after being initially created. Zero once invulnerability has expired. */
	unsigned int m_WitherInvulnerableTicks;

} ;




