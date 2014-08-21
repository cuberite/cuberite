
#pragma once

#include "Monster.h"





class cWither :
	public cMonster
{
	typedef cMonster super;
	
public:
	cWither(void);

	CLASS_PROTODEF(cWither)

	/** Returns whether the wither is invulnerable to arrows. */
	bool IsArmored(void) const;
	int GetWitherInvulnerableTicks() const { return m_WitherInvulnerableTicks; }
	void SetWitherInvulnerableTicks(unsigned int a_Ticks) { m_WitherInvulnerableTicks = a_Ticks; }
	
	// cEntity overrides
	virtual bool Initialize(cWorld & a_World) override;
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;

private:

	/** The number of ticks of invulnerability left after being initially created. Zero once invulnerability has expired. */
	unsigned int m_WitherInvulnerableTicks;

} ;




