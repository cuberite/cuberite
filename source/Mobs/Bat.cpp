
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Bat.h"





cBat::cBat(void)
{
	m_MobType = 65;
	m_SoundHurt = "mob.bat.hurt";
	m_SoundDeath = "mob.bat.death";
	GetMonsterConfig("Bat");
}




