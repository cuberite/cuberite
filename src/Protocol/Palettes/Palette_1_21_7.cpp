#include "Globals.h"
#include "Palette_1_21_7.h"
#include "Registries/BlockStates.h"
#include "BlockType.h"
#include "BlockMap.h"
#include "Root.h"
#include "fmt/format.h"

namespace Palette_1_21_7
{
	UInt32 From(const BlockState Block)
	{
		return cRoot::Get()->GetBlockMap()->GetProtocolBlockId(cProtocol::Version::v1_21_7, Block);
	}
	UInt32 From(FluidType a_Fluid)
	{
		switch (a_Fluid)
		{
			case FluidType::Empty:        return 0;
			case FluidType::FlowingLava:  return 3;
			case FluidType::FlowingWater: return 1;
			case FluidType::Lava:         return 4;
			case FluidType::Water:        return 2;
		}
		LOGWARN(fmt::format(FMT_STRING("Unknown fluid type {}"), static_cast<UInt32>(a_Fluid)));
		return 0;
	}

	UInt32 From(const BlockType a_Block)
	{
		return cRoot::Get()->GetRegistryMap()->BlockTypeToProtocol(a_Block, cProtocol::Version::v1_21_7);
	}
	UInt32 From(const CustomStatistic ID)
	{
		switch (ID)
		{
			case CustomStatistic::AnimalsBred:                  return 33;
			case CustomStatistic::AviateOneCm:                  return 19;
			case CustomStatistic::BellRing:                     return 69;
			case CustomStatistic::BoatOneCm:                    return 15;
			case CustomStatistic::CleanArmor:                   return 41;
			case CustomStatistic::CleanBanner:                  return 42;
			case CustomStatistic::CleanShulkerBox:              return 43;
			case CustomStatistic::ClimbOneCm:                   return 11;
			case CustomStatistic::CrouchOneCm:                  return 7;
			case CustomStatistic::DamageAbsorbed:               return 29;
			case CustomStatistic::DamageBlockedByShield:        return 28;
			case CustomStatistic::DamageDealt:                  return 24;
			case CustomStatistic::DamageDealtAbsorbed:          return 25;
			case CustomStatistic::DamageDealtResisted:          return 26;
			case CustomStatistic::DamageResisted:               return 30;
			case CustomStatistic::DamageTaken:                  return 27;
			case CustomStatistic::Deaths:                       return 31;
			case CustomStatistic::Drop:                         return 23;
			case CustomStatistic::EatCakeSlice:                 return 38;
			case CustomStatistic::EnchantItem:                  return 54;
			case CustomStatistic::FallOneCm:                    return 10;
			case CustomStatistic::FillCauldron:                 return 39;
			case CustomStatistic::FishCaught:                   return 35;
			case CustomStatistic::FlyOneCm:                     return 12;
			case CustomStatistic::HappyGhastOneCm:              return 17;
			case CustomStatistic::HorseOneCm:                   return 18;
			case CustomStatistic::InspectDispenser:             return 48;
			case CustomStatistic::InspectDropper:               return 46;
			case CustomStatistic::InspectHopper:                return 47;
			case CustomStatistic::InteractWithAnvil:            return 72;
			case CustomStatistic::InteractWithBeacon:           return 45;
			case CustomStatistic::InteractWithBlastFurnace:     return 62;
			case CustomStatistic::InteractWithBrewingstand:     return 44;
			case CustomStatistic::InteractWithCampfire:         return 65;
			case CustomStatistic::InteractWithCartographyTable: return 66;
			case CustomStatistic::InteractWithCraftingTable:    return 57;
			case CustomStatistic::InteractWithFurnace:          return 56;
			case CustomStatistic::InteractWithGrindstone:       return 73;
			case CustomStatistic::InteractWithLectern:          return 64;
			case CustomStatistic::InteractWithLoom:             return 67;
			case CustomStatistic::InteractWithSmithingTable:    return 75;
			case CustomStatistic::InteractWithSmoker:           return 63;
			case CustomStatistic::InteractWithStonecutter:      return 68;
			case CustomStatistic::Jump:                         return 22;
			case CustomStatistic::LeaveGame:                    return 0;
			case CustomStatistic::MinecartOneCm:                return 14;
			case CustomStatistic::MobKills:                     return 32;
			case CustomStatistic::OpenBarrel:                   return 61;
			case CustomStatistic::OpenChest:                    return 58;
			case CustomStatistic::OpenEnderchest:               return 53;
			case CustomStatistic::OpenShulkerBox:               return 60;
			case CustomStatistic::PigOneCm:                     return 16;
			case CustomStatistic::PlayNoteblock:                return 49;
			case CustomStatistic::PlayRecord:                   return 55;
			case CustomStatistic::PlayTime:                     return 1;
			case CustomStatistic::PlayerKills:                  return 34;
			case CustomStatistic::PotFlower:                    return 51;
			case CustomStatistic::RaidTrigger:                  return 70;
			case CustomStatistic::RaidWin:                      return 71;
			case CustomStatistic::SleepInBed:                   return 59;
			case CustomStatistic::SneakTime:                    return 5;
			case CustomStatistic::SprintOneCm:                  return 8;
			case CustomStatistic::StriderOneCm:                 return 21;
			case CustomStatistic::SwimOneCm:                    return 20;
			case CustomStatistic::TalkedToVillager:             return 36;
			case CustomStatistic::TargetHit:                    return 74;
			case CustomStatistic::TimeSinceDeath:               return 3;
			case CustomStatistic::TimeSinceRest:                return 4;
			case CustomStatistic::TotalWorldTime:               return 2;
			case CustomStatistic::TradedWithVillager:           return 37;
			case CustomStatistic::TriggerTrappedChest:          return 52;
			case CustomStatistic::TuneNoteblock:                return 50;
			case CustomStatistic::UseCauldron:                  return 40;
			case CustomStatistic::WalkOnWaterOneCm:             return 9;
			case CustomStatistic::WalkOneCm:                    return 6;
			case CustomStatistic::WalkUnderWaterOneCm:          return 13;
			default: return 0;
		}
	}
	Item ToItem(const UInt32 ID)
	{
		return cRoot::Get()->GetRegistryMap()->ProtocolToItem(ID, cProtocol::Version::v1_21_7);
	}
	UInt32 From(const Item ID)
	{
		return cRoot::Get()->GetRegistryMap()->ItemToProtocol(ID, cProtocol::Version::v1_21_7);
	}
}  // namespace Palette_1_21_7
