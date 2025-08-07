#include "Globals.h"

#include "../Protocol_1_13.h"


/**
* Custom linking from "modern" SoundEvent enums to 1.13 sounds
*/
AString cProtocol_1_13::GetProtocolSoundEffectAsString(eSoundEvent a_SoundEvent) const
{
	switch (a_SoundEvent)
	{
		// Weather
		case eSoundEvent::LightningboltThunder:        return "entity.lightning_bolt.thunder";

		// Blocks
		case eSoundEvent::BlockWoodbtnClickon:         return "block.wooden_button.click_on";

		// Entity Items
		case eSoundEvent::EnderEyeDeath:               return "entity.ender_eye.death";
		case eSoundEvent::EnderEyeLaunch:              return "entity.ender_eye.launch";
		case eSoundEvent::FishingbobberSplash:         return "entity.fishing_bobber.splash";
		case eSoundEvent::LeashknotBreak:              return "entity.leash_knot.break";
		case eSoundEvent::LeashknotPlace:              return "entity.leash_knot.place";
		default:                                       return Super::GetProtocolSoundEffectAsString(a_SoundEvent);
	}
	
}
