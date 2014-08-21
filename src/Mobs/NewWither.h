
#pragma once

#include "NewMonster.h"





class cNewWither :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewWither(void);

	CLASS_PROTODEF(cNewWither)

	/** Returns whether the wither is invulnerable to arrows. */
	bool IsArmored(void) const;
	int GetWitherInvulnerableTicks() const { return 0; }
	
	// cEntity overrides
	virtual bool Initialize(cWorld & a_World) override;
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;

} ;




