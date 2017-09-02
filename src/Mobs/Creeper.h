
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
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;

	bool IsBlowing(void) const {return m_BehaviorSuicideBomber.IsBlowing(); }
	bool IsCharged(void) const {return m_BehaviorSuicideBomber.IsCharged(); }
	bool IsBurnedWithFlintAndSteel(void) const {return m_BehaviorSuicideBomber.IsBurnedWithFlintAndSteel(); }

private:
	cBehaviorSuicideBomer m_BehaviorSuicideBomber;
} ;




