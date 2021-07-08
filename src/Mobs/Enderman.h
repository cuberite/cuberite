
#pragma once

#include "PassiveAggressiveMonster.h"





bool AttemptTeleport(cWorld &a_World, Vector3i a_Centre, const int a_LinearLength, const int a_HeightRequired, const unsigned int a_NumTries, Vector3i &a_Destination);





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
	BLOCKTYPE GetCarriedBlock(void) const {return m_CarriedBlock; }
	NIBBLETYPE GetCarriedMeta(void) const {return m_CarriedMeta; }

private:

	bool m_bIsScreaming;
	BLOCKTYPE m_CarriedBlock;
	NIBBLETYPE m_CarriedMeta;

} ;
