#include "Globals.h"

#include "../Protocol_1_12.h"


/**
* Custom linking from "modern" SoundEvent enums to 1.12 sounds
*/
AString cProtocol_1_12::GetProtocolSoundEffectAsString(eSoundEvent a_SoundEvent) const
{
	switch (a_SoundEvent)
	{
		// Entity Items
		case eSoundEvent::EnderEyeDeath:               return "entity.endereye.death";
		default:                                       return Super::GetProtocolSoundEffectAsString(a_SoundEvent);
	}
	
}
