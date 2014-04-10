
#pragma once

#include "AggressiveMonster.h"





class cWither :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cWither(void);

	CLASS_PROTODEF(cWither);

	unsigned int GetNumInvulnerableTicks(void) const { return m_InvulnerableTicks; }

	void SetNumInvulnerableTicks(unsigned int a_Ticks) { m_InvulnerableTicks = a_Ticks; }

	/** Returns whether the wither is invulnerable to arrows. */
	bool IsArmored(void) const;
	
	// cEntity overrides
	virtual bool Initialize(cWorld * a_World) override;
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

private:

	/** The number of ticks of invulnerability left after being initially created. Zero once invulnerability has expired. */
	unsigned int m_InvulnerableTicks;

} ;




