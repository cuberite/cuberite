
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Ocelot.h"





cOcelot::cOcelot()
{
	m_MobType = 98;
	m_SoundHurt = "mob.cat.hitt";
	m_SoundDeath = "mob.cat.hitt";
	GetMonsterConfig("Ocelot");
}




