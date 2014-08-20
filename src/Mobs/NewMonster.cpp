#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "NewMonster.h"
#include "../ClientHandle.h"

cNewMonster::cNewMonster(const AString & a_ConfigName, eType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height)
	: super(etMonster, a_Width, a_Height)
	, m_MobType(a_MobType)
	, m_SoundHurt(a_SoundHurt)
	, m_SoundDeath(a_SoundDeath)
{
}

void cNewMonster::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendSpawnMob(*this);
}

