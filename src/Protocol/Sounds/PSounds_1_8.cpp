#include "Globals.h"

#include "../Protocol_1_8.h"


AString cProtocol_1_8_0::GetProtocolSoundEffectAsString(SoundEvent a_SoundEvent) const
{
	switch (a_SoundEvent)
	{
		// Ender Eye
		case SoundEvent::EnderEyeCustomSurvive:  return "random.pop";  // not part of the 1.21.4 sound list
		case SoundEvent::EnderEyeDeath:          return "dig.glass";
		case SoundEvent::EnderEyeLaunch:         return "random.bow";

		// Explosions
		case SoundEvent::EntityGenericExplode:   return "random.explode";
	}
	return AString();
}
