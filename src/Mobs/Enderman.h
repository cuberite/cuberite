
#pragma once

#include "PassiveAggressiveMonster.h"





class cEnderman:
	public cPassiveAggressiveMonster
{
	using Super = cPassiveAggressiveMonster;

public:

	cEnderman();

	CLASS_PROTODEF(cEnderman)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual void CheckEventSeePlayer(cChunk & a_Chunk) override;
	virtual void EventLosePlayer(void) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	bool IsScreaming(void) const {return m_bIsScreaming; }
	BlockState GetCarriedBlock(void) const { return m_CarriedBlock; }

private:

	bool m_bIsScreaming;
	BlockState m_CarriedBlock;

} ;
