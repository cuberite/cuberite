
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "PassiveAggressiveMonster.h"

#include "../Entities/Player.h"





cPassiveAggressiveMonster::cPassiveAggressiveMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, const AString & a_SoundAmbient, double a_Width, double a_Height) :
	Super(a_ConfigName, a_MobType, a_SoundHurt, a_SoundDeath, a_SoundAmbient, a_Width, a_Height)
{
	m_EMPersonality = PASSIVE;
}





bool cPassiveAggressiveMonster::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (!Super::DoTakeDamage(a_TDI))
	{
		return false;
	}

	if ((GetTarget() != nullptr) && (GetTarget()->IsPlayer()))
	{
		if (static_cast<cPlayer *>(GetTarget())->CanMobsTarget())
		{
			m_EMState = CHASING;
		}
	}
	return true;
}





void cPassiveAggressiveMonster::EventSeePlayer(cPlayer *, cChunk & a_Chunk)
{
	// don't do anything, neutral mobs don't react to just seeing the player
}


