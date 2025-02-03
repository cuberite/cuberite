#include "Globals.h"

#include "../Protocol_1_8.h"


AString cProtocol_1_8_0::GetProtocolSoundEffectAsString(SoundEvent a_SoundEvent) const
{
	switch (a_SoundEvent)
	{
		// Explosions
		case SoundEvent::EntityGenericExplode:   return "random.explode";

		// Mobs
		case SoundEvent::EntityHorseAngry:       return "mob.horse.angry";
		case SoundEvent::PaintingPlace:          // Entity Item Borrow
		case SoundEvent::EntityHorseSaddle:      return "mob.horse.leather";
		case SoundEvent::EntityHorseArmor:       return "mob.horse.armor";
		case SoundEvent::EntityGhastWarn:        return "mob.ghast.charge";
		case SoundEvent::EntityBlazeShoot:       // Like Ghast Fireball
		case SoundEvent::EntityGhastShoot:       return "mob.ghast.fireball";
		case SoundEvent::EntityCreeperPrimed     return "creeper.primed";

		// Blocks
		case SoundEvent::BlockChestOpen:         return "random.chestopen";
		case SoundEvent::BlockChestClose:        return "random.chestclosed";

		// Items
		case SoundEvent::ItemHoeTill:            return "dig.grass";
		case SoundEvent::ItemFirechargeUse:      return "item.fireCharge.use";

		// Entity Items
		case SoundEvent::PaintingBreak:          // Borrow Random Pop
		case SoundEvent::EnderEyeCustomSurvive:  return "random.pop";  // not part of the 1.21.4 sound list
		case SoundEvent::EnderEyeDeath:          return "dig.glass";
		case SoundEvent::EnderEyeLaunch:         // ThrowSounds
		case SoundEvent::SplashPotionThrow:      return "random.bow";
	}
	return AString();
}
