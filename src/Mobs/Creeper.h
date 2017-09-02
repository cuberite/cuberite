
#pragma once

#include "Monster.h"
#include "Behaviors/BehaviorAttackerSuicideBomber.h"




class cCreeper :
	public cMonster
{
	typedef cMonster super;

public:
	cCreeper(void);

	CLASS_PROTODEF(cCreeper)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;

	bool IsBlowing(void) const;
	bool IsCharged(void) const;
	bool IsBurnedWithFlintAndSteel(void) const;

private:
	cBehaviorAttackerSuicideBomber m_BehaviorSuicideBomber;
} ;




