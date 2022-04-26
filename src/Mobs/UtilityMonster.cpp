
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "UtilityMonster.h"





cUtilityMonster::cUtilityMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, const AString & a_SoundAmbient, float a_Width, float a_Height) :
	Super(a_ConfigName, a_MobType, a_SoundHurt, a_SoundDeath, a_SoundAmbient, a_Width, a_Height)
{
	m_EMPersonality = PASSIVE;
}
