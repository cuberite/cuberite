#include "Globals.h"

#include "../Protocol_1_9.h"


/**
* Custom linking from "modern" SoundEvent enums to 1.9 sounds
*/
AString cProtocol_1_9_0::GetProtocolSoundEffectAsString(eSoundEvent a_SoundEvent) const
{
	switch (a_SoundEvent)
	{
		case eSoundEvent::NullValue:                   return "";
		// Weather
		case eSoundEvent::LightningboltThunder:        return "entity.lightning.thunder";
		// Explosions
		case eSoundEvent::EntityGenericExplode:        return "entity.generic.explode";

		// Mobs
		case eSoundEvent::EntityHorseAmbient:          return "entity.horse.ambient";
		case eSoundEvent::EntityHorseHurt:             return "entity.horse.hurt";
		case eSoundEvent::EntityHorseDeath:            return "entity.horse.death";
		case eSoundEvent::EntityHorseAngry:            return "entity.horse.angry";
		case eSoundEvent::EntityHorseSaddle:           return "entity.horse.saddle";
		case eSoundEvent::EntityHorseArmor:            return "entity.horse.armor";
		case eSoundEvent::EntityBlazeAmbient:          return "entity.blaze.ambient";
		case eSoundEvent::EntityBlazeHurt:             return "entity.blaze.hurt";
		case eSoundEvent::EntityBlazeDeath:            return "entity.blaze.death";
		case eSoundEvent::EntityBlazeShoot:            return "entity.blaze.shoot";
		case eSoundEvent::EntityGhastAmbient:          return "entity.ghast.ambient";
		case eSoundEvent::EntityGhastHurt:             return "entity.ghast.hurt";
		case eSoundEvent::EntityGhastDeath:            return "entity.ghast.death";
		case eSoundEvent::EntityGhastShoot:            return "entity.ghast.shoot";
		case eSoundEvent::EntityGhastWarn:             return "entity.ghast.warn";
		case eSoundEvent::EntityCreeperHurt:           return "entity.creeper.hurt";
		case eSoundEvent::EntityCreeperDeath:          return "entity.creeper.death";
		case eSoundEvent::EntityCreeperPrimed:         return "entity.creeper.primed";
		case eSoundEvent::TntPrimed:                   return "entity.tnt.primed";
		case eSoundEvent::EntityZombievillagerAmbient: return "entity.zombie_villager.ambient";
		case eSoundEvent::EntityZombievillagerHurt:    return "entity.zombie_villager.hurt";
		case eSoundEvent::EntityZombievillagerDeath:   return "entity.zombie_villager.death";
		case eSoundEvent::EntityZombievillagerCure:    return "entity.zombie_villager.cure";
		case eSoundEvent::EntitySheepShear:            return "entity.sheep.shear";
		case eSoundEvent::EntitySheepAmbient:          return "entity.sheep.ambient";
		case eSoundEvent::EntitySheepHurt:             return "entity.sheep.hurt";
		case eSoundEvent::EntitySheepDeath:            return "entity.sheep.death";

		// Blocks
		case eSoundEvent::BlockChestOpen:              return "block.chest.open";
		case eSoundEvent::BlockChestClose:             return "block.chest.close";
		case eSoundEvent::BlockDispenserFail:          return "block.dispenser.fail";
		case eSoundEvent::BlockDispenserDispense:      return "block.dispenser.dispense";
		case eSoundEvent::BlockWoodbtnClickon:         return "block.wood_button.click_on";
		case eSoundEvent::BlockLeverClick:             return "block.lever.click";
		case eSoundEvent::BlockPistonExtend:           return "block.piston.extend";
		case eSoundEvent::BlockPistonContract:         return "block.piston.contract";
		case eSoundEvent::BlockBrewingstandBrew:       return "block.brewing_stand.brew";

		// Items
		case eSoundEvent::EnderEyeCustomSurvive:       // Entity Item (custom)
		case eSoundEvent::ItemPickup:                  return "entity.item.pickup";
		
		case eSoundEvent::ItemFirechargeUse:           return "item.firecharge.use";

		// Silenced Items (client covers these sounds)
		case eSoundEvent::ItemHoeTill:                 return "";
		case eSoundEvent::ItemFlintsteelUse:           return "";

		case eSoundEvent::ItemChorusfruitTeleport:     return "item.chorus_fruit.teleport";

		// Entity Items
		case eSoundEvent::EnderEyeDeath:               return "entity.splash_potion.break";
		case eSoundEvent::ArrowShoot:                  return "entity.arrow.shoot";
		case eSoundEvent::EnderEyeLaunch:              return "entity.endereye.launch";
		case eSoundEvent::SplashPotionThrow:           return "entity.splash_potion.throw";
		case eSoundEvent::ArrowHit:                    return "entity.arrow.hit";
		case eSoundEvent::ExporbPickup:                return "entity.experience_orb.pickup";
		case eSoundEvent::FishingbobberSplash:         return "entity.bobber.splash";
		case eSoundEvent::LeashknotBreak:              return "entity.leashknot.break";
		case eSoundEvent::LeashknotPlace:              return "entity.leashknot.place";
		case eSoundEvent::PaintingPlace:               return "entity.painting.place";
		case eSoundEvent::PaintingBreak:               return "entity.painting.break";
	}
	return AString();
}
