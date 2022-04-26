
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "UtilityMonster.h"
#include "LineBlockTracer.h"
#include "../World.h"





cUtilityMonster::cUtilityMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, const AString & a_SoundAmbient, float a_Width, float a_Height) :
	Super(a_ConfigName, a_MobType, a_SoundHurt, a_SoundDeath, a_SoundAmbient, a_Width, a_Height)
{
	m_EMPersonality = PASSIVE;
}





void cUtilityMonster::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	// Set or clear m_Target:
	if (m_EMState == CHASING)
	{
		CheckEventSeeMob(a_Chunk);
	}
	else
	{
		CheckEventLostMob(a_Dt);
	}
}




