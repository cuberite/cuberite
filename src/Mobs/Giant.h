
#pragma once

#include "Monster.h"
#include "Behaviors/BehaviorDoNothing.h"




class cGiant :
	public cMonster
{
	typedef cMonster super;

public:
	cGiant(void);

	CLASS_PROTODEF(cGiant)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;

private:
	cBehaviorDoNothing m_BehaviorDoNothing;
} ;




