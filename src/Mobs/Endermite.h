
#pragma once

#include "AggressiveMonster.h"



class cEndermite : public cAggressiveMonster
{
	using Super = cAggressiveMonster;

	// Endermite should despawn in two minutes
	std::chrono::milliseconds m_Timer;
	std::chrono::milliseconds m_Lifetime;

  public:
	cEndermite();

	CLASS_PROTODEF(cEndermite)

	void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
};
