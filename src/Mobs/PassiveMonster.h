
#pragma once

#include "Monster.h"
#include "Behaviors/BehaviorBreeder.h"
#include "Behaviors/BehaviorItemFollower.h"
#include "Behaviors/BehaviorCoward.h"
#include "Behaviors/BehaviorWanderer.h"


typedef std::string AString;
class cPassiveMonster : public cMonster
{
	typedef cMonster super;

public:
	cPassiveMonster(const AString & a_ConfigName, eMonsterType a_MobType,
					const AString & a_SoundHurt, const AString & a_SoundDeath,
					double a_Width, double a_Height);
	virtual ~cPassiveMonster();
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;

	/** When hit by someone, run away */
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;

	virtual void Destroyed(void) override;
private:
	cBehaviorBreeder m_BehaviorBreeder;
	cBehaviorItemFollower m_BehaviorItemFollower;
	cBehaviorCoward m_BehaviorCoward;
	cBehaviorWanderer m_BehaviorWanderer;
};
