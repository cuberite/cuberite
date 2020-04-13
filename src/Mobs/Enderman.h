
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
	virtual void CheckEventLostPlayer(void) override;
	virtual void EventLosePlayer(void) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	bool IsScreaming(void) const {return m_bIsScreaming; }
	BLOCKTYPE GetCarriedBlock(void) const {return m_CarriedBlock; }
	NIBBLETYPE GetCarriedMeta(void) const {return m_CarriedMeta; }

	/** Returns if the current sky light level is sufficient for the enderman to become aggravated */
	bool CheckLight(void);

private:

	bool m_bIsScreaming;
	BLOCKTYPE m_CarriedBlock;
	NIBBLETYPE m_CarriedMeta;

} ;
