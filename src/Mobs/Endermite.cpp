
#include "Globals.h"

#include "Endermite.h"

#include "../World.h"
#include "../Chunk.h"
#include "../Blocks/BlockHandler.h"
#include "../Blocks/BlockInfested.h"





cEndermite::cEndermite() :
	Super("Endermite", etEndermite, "entity.endermite.hurt", "entity.endermite.death", "entity.endermite.ambient", 0.4f, 0.3f),
	m_Timer(0),
	m_Lifetime(2 * 1000 * 60)  // 2 minutes (2 * 1000 (mili to sec) * 60 (sec to min) * 2 because tick = 0.5 sec)
{
}





void cEndermite::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);

	// Not destroying the endermite if a name is set
	if (m_CustomName.empty())
	{
		m_Timer += a_Dt;
		// Destroy the endermite after 2 minutes
		if (m_Timer > m_Lifetime)
		{
			Destroy();
		}

	}
}
