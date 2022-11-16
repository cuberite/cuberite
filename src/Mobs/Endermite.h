
#pragma once

#include "AggressiveMonster.h"
#include <chrono>





class cEndermite:
	public cAggressiveMonster
{
	using Super = cAggressiveMonster;

	// Endermite should despawn in two minutes
	std::chrono::milliseconds m_Lifetime;
	std::chrono::milliseconds m_Timer;

public:

	cEndermite();

	CLASS_PROTODEF(cEndermite)

	void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);
} ;
