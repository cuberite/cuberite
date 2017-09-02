
#pragma once

#include "Monster.h"
#include "Behaviors/BehaviorWanderer.h"




class cBat :
	public cMonster
{
	typedef cMonster super;

public:
	cBat(void);

	CLASS_PROTODEF(cBat)

	bool IsHanging(void) const {return false; }

private:
	cBehaviorWanderer m_BehaviorWanderer;
} ;




