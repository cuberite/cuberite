
#pragma once

#include "Monster.h"
#include "Behaviors/BehaviorDoNothing.h"

class cEnderDragon :
	public cMonster
{
	typedef cMonster super;

public:
	cEnderDragon(void);

	CLASS_PROTODEF(cEnderDragon)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;

private:
	cBehaviorDoNothing m_BehaviorDoNothing;
} ;




