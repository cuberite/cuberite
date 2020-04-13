
#pragma once

#include "AggressiveMonster.h"





class cCreeper:
	public cAggressiveMonster
{
	using Super = cAggressiveMonster;

public:

	cCreeper();

	CLASS_PROTODEF(cCreeper)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual bool Attack(std::chrono::milliseconds a_Dt) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;

	bool IsBlowing(void) const {return m_bIsBlowing; }
	bool IsCharged(void) const {return m_bIsCharged; }
	bool IsBurnedWithFlintAndSteel(void) const {return m_BurnedWithFlintAndSteel; }

private:

	bool m_bIsBlowing, m_bIsCharged, m_BurnedWithFlintAndSteel;
	int m_ExplodingTimer;

} ;




