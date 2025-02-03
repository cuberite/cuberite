#include "Globals.h"

#include "../Protocol_1_8.h"


/**
 * Custom linking from "modern" SoundEvent enums to 1.8 sounds
 * Because no subtitles exist, close sounding sounds are used (just in case)
 */
AString cProtocol_1_8_0::GetProtocolSoundEffectAsString(SoundEvent a_SoundEvent) const
{
	switch (a_SoundEvent)
	{
		// Weather
		case SoundEvent::LightningboltThunder:     return "ambient.weather.thunder";
		// Explosions
		case SoundEvent::EntityGenericExplode:     return "random.explode";

		// Mobs
		case SoundEvent::EntityHorseAngry:         return "mob.horse.angry";
		case SoundEvent::PaintingPlace:            // Entity Item Borrow
		case SoundEvent::EntityHorseSaddle:        return "mob.horse.leather";
		case SoundEvent::EntityHorseArmor:         return "mob.horse.armor";
		case SoundEvent::EntityGhastWarn:          return "mob.ghast.charge";
		case SoundEvent::EntityBlazeShoot:         // Like Ghast Fireball
		case SoundEvent::EntityGhastShoot:         return "mob.ghast.fireball";
		case SoundEvent::TntPrimed:
		case SoundEvent::EntityCreeperPrimed:      return "creeper.primed";
		case SoundEvent::EntityZombievillagerCure: return "mob.zombie.remedy";
		case SoundEvent::LeashknotBreak:           // Entity Item
		case SoundEvent::EntitySheepShear:         return "mob.sheep.shear";

		// Blocks
		case SoundEvent::BlockChestOpen:           return "random.chestopen";
		case SoundEvent::BlockChestClose:          return "random.chestclosed";
		case SoundEvent::BlockDispenserFail:
		case SoundEvent::BlockDispenserDispense:
		case SoundEvent::BlockWoodbtnClickon:
		case SoundEvent::BlockLeverClick:          return "random.click";
		case SoundEvent::BlockPistonExtend:        return "tile.piston.in";
		case SoundEvent::BlockPistonContract:      return "tile.piston.out";
		case SoundEvent::BlockBrewingstandBrew:    return "mob.zombie.unfect";

		// Items
		case SoundEvent::PaintingBreak:            // Entity Item
		case SoundEvent::EnderEyeCustomSurvive:    // Entity Item (custom)
		case SoundEvent::ItemPickup:               return "random.pop";
		case SoundEvent::ItemHoeTill:              return "dig.grass";
		case SoundEvent::ItemFirechargeUse:        return "item.fireCharge.use";
		case SoundEvent::ItemChorusfruitTeleport:  return "mob.endermen.portal";

		// Entity Items
		case SoundEvent::EnderEyeDeath:            return "dig.glass";
		case SoundEvent::ArrowShoot:               // ThrowSounds
		case SoundEvent::EnderEyeLaunch:           // ThrowSounds
		case SoundEvent::SplashPotionThrow:        return "random.bow";
		case SoundEvent::LeashknotPlace:           return "step.grass";
		case SoundEvent::ArrowHit:                 return "random.bowhit";
		case SoundEvent::ExporbPickup:             return "random.orb";
		case SoundEvent::FishingbobberSplash:      return "random.splash";
	}
	return AString();
}
