#include "Globals.h"

#include "../Protocol_1_8.h"


/**
* Custom linking from "modern" SoundEvent enums to 1.8 sounds
* Because no subtitles exist, close sounding sounds are used (just in case)
*/
AString cProtocol_1_8_0::GetProtocolSoundEffectAsString(eSoundEvent a_SoundEvent) const
{
	switch (a_SoundEvent)
	{
		case eSoundEvent::NullValue:                   return "";
		// Weather
		case eSoundEvent::LightningboltThunder:        return "ambient.weather.thunder";
		// Explosions
		case eSoundEvent::EntityGenericExplode:        return "random.explode";

		// Mobs
		case eSoundEvent::EntityHorseAmbient:          return "mob.horse.idle";
		case eSoundEvent::EntityHorseHurt:             return "mob.horse.hit";
		case eSoundEvent::EntityHorseDeath:            return "mob.horse.death";
		case eSoundEvent::EntityHorseAngry:            return "mob.horse.angry";
		case eSoundEvent::PaintingPlace:               // Entity Item Borrow
		case eSoundEvent::EntityHorseSaddle:           return "mob.horse.leather";
		case eSoundEvent::EntityHorseArmor:            return "mob.horse.armor";
		case eSoundEvent::EntityGhastAmbient:          return "mob.ghast.moan";
		case eSoundEvent::EntityGhastHurt:             return "mob.ghast.scream";
		case eSoundEvent::EntityGhastDeath:            return "mob.ghast.death";
		case eSoundEvent::EntityGhastWarn:             return "mob.ghast.charge";
		case eSoundEvent::EntityBlazeShoot:            // Like Ghast Fireball
		case eSoundEvent::EntityGhastShoot:            return "mob.ghast.fireball";
		case eSoundEvent::EntityBlazeAmbient:          return "mob.blaze.breathe";
		case eSoundEvent::EntityBlazeHurt:             return "mob.blaze.hit";
		case eSoundEvent::EntityBlazeDeath:            return "mob.blaze.death";
		case eSoundEvent::EntityCreeperHurt:           return "mob.creeper.say";
		case eSoundEvent::EntityCreeperDeath:          return "mob.creeper.death";
		case eSoundEvent::TntPrimed:
		case eSoundEvent::EntityCreeperPrimed:         return "creeper.primed";
		case eSoundEvent::EntityZombievillagerAmbient: return "mob.zombie.say";
		case eSoundEvent::EntityZombievillagerHurt:    return "mob.zombie.hurt";
		case eSoundEvent::EntityZombievillagerDeath:   return "mob.zombie.death";
		case eSoundEvent::EntityZombievillagerCure:    return "mob.zombie.remedy";
		case eSoundEvent::LeashknotBreak:              // Entity Item
		case eSoundEvent::EntitySheepShear:            return "mob.sheep.shear";
		case eSoundEvent::EntitySheepHurt:             // Uses Ambient
		case eSoundEvent::EntitySheepDeath:            // Uses Ambient
		case eSoundEvent::EntitySheepAmbient:          return "mob.sheep.say";

		// Blocks
		case eSoundEvent::BlockChestOpen:              return "random.chestopen";
		case eSoundEvent::BlockChestClose:             return "random.chestclosed";
		case eSoundEvent::BlockDispenserFail:          // Random.Click
		case eSoundEvent::BlockDispenserDispense:      // Random.Click
		case eSoundEvent::BlockLeverClick:             return "random.click";
		case eSoundEvent::BlockWoodbtnClickon:         return "random.wood_click";
		case eSoundEvent::BlockPistonExtend:           return "tile.piston.out";
		case eSoundEvent::BlockPistonContract:         return "tile.piston.in";
		case eSoundEvent::BlockBrewingstandBrew:       return "mob.zombie.unfect";

		// Items
		case eSoundEvent::PaintingBreak:               // Entity Item
		case eSoundEvent::EnderEyeCustomSurvive:       // Entity Item (custom)
		case eSoundEvent::ItemPickup:                  return "random.pop";
		case eSoundEvent::ItemHoeTill:                 return "dig.grass";
		case eSoundEvent::ItemFirechargeUse:           return "item.fireCharge.use";
		case eSoundEvent::ItemFlintsteelUse:           return "fire.ignite";

		case eSoundEvent::ItemChorusfruitTeleport:     return "mob.endermen.portal";

		// Entity Items
		case eSoundEvent::EnderEyeDeath:               return "dig.glass";
		case eSoundEvent::ArrowShoot:                  // ThrowSounds
		case eSoundEvent::EnderEyeLaunch:              // ThrowSounds
		case eSoundEvent::SplashPotionThrow:           return "random.bow";
		case eSoundEvent::LeashknotPlace:              return "step.grass";
		case eSoundEvent::ArrowHit:                    return "random.bowhit";
		case eSoundEvent::ExporbPickup:                return "random.orb";
		case eSoundEvent::FishingbobberSplash:         return "random.splash";
	}
	return AString();
}
