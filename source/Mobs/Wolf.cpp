
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Wolf.h"





cWolf::cWolf(void)
{
	m_MobType = 95;
	m_SoundHurt = "mob.wolf.hurt";
	m_SoundDeath = "mob.wolf.death";
	GetMonsterConfig("Wolf");
}




