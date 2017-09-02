
#pragma once

#include "Monster.h"
#include "Behaviors/BehaviorDoNothing.h"




class cSilverfish :
	public cMonster
{
	typedef cMonster super;

public:
	cSilverfish(void) :
		super(mtSilverfish, "entity.silverfish.hurt", "entity.silverfish.death", 0.3, 0.7)
	{
		m_BehaviorDoNothing.AttachToMonster(*this);
		GetMonsterConfig("Silverfish");
	}

	CLASS_PROTODEF(cSilverfish)

private:
	cBehaviorDoNothing m_BehaviorDoNothing;
} ;




