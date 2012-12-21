
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Silverfish.h"





cSilverfish::cSilverfish(void)
{
	m_MobType = 60;
	m_SoundHurt = "mob.silverfish.hit";
	m_SoundDeath = "mob.silverfish.kill";
	GetMonsterConfig("Silverfish");
}




