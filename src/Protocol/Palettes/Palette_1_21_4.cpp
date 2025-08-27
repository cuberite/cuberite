#include "Globals.h"
#include "Palette_1_21_4.h"
#include "Registries/BlockStates.h"
#include "BlockType.h"
#include "BlockMap.h"
#include "Root.h"
#include "fmt/format.h"

namespace Palette_1_21_4
{
	UInt32 From(const BlockState Block)
	{
		return cRoot::Get()->GetBlockMap()->GetProtocolBlockId(cProtocol::Version::v1_21_4, Block);
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
		return cRoot::Get()->GetRegistryMap()->BlockTypeToProtocol(a_Block, cProtocol::Version::v1_21_4);
	}
	UInt32 From(const CustomStatistic ID)
	{
		switch (ID)
		{
			case CustomStatistic::AnimalsBred:                  return 32;
			case CustomStatistic::AviateOneCm:                  return 18;
			case CustomStatistic::BellRing:                     return 68;
			case CustomStatistic::BoatOneCm:                    return 15;
			case CustomStatistic::CleanArmor:                   return 40;
			case CustomStatistic::CleanBanner:                  return 41;
			case CustomStatistic::CleanShulkerBox:              return 42;
			case CustomStatistic::ClimbOneCm:                   return 11;
			case CustomStatistic::CrouchOneCm:                  return 7;
			case CustomStatistic::DamageAbsorbed:               return 28;
			case CustomStatistic::DamageBlockedByShield:        return 27;
			case CustomStatistic::DamageDealt:                  return 23;
			case CustomStatistic::DamageDealtAbsorbed:          return 24;
			case CustomStatistic::DamageDealtResisted:          return 25;
			case CustomStatistic::DamageResisted:               return 29;
			case CustomStatistic::DamageTaken:                  return 26;
			case CustomStatistic::Deaths:                       return 30;
			case CustomStatistic::Drop:                         return 22;
			case CustomStatistic::EatCakeSlice:                 return 37;
			case CustomStatistic::EnchantItem:                  return 53;
			case CustomStatistic::FallOneCm:                    return 10;
			case CustomStatistic::FillCauldron:                 return 38;
			case CustomStatistic::FishCaught:                   return 34;
			case CustomStatistic::FlyOneCm:                     return 12;
			case CustomStatistic::HorseOneCm:                   return 17;
			case CustomStatistic::InspectDispenser:             return 47;
			case CustomStatistic::InspectDropper:               return 45;
			case CustomStatistic::InspectHopper:                return 46;
			case CustomStatistic::InteractWithAnvil:            return 71;
			case CustomStatistic::InteractWithBeacon:           return 44;
			case CustomStatistic::InteractWithBlastFurnace:     return 61;
			case CustomStatistic::InteractWithBrewingstand:     return 43;
			case CustomStatistic::InteractWithCampfire:         return 64;
			case CustomStatistic::InteractWithCartographyTable: return 65;
			case CustomStatistic::InteractWithCraftingTable:    return 56;
			case CustomStatistic::InteractWithFurnace:          return 55;
			case CustomStatistic::InteractWithGrindstone:       return 72;
			case CustomStatistic::InteractWithLectern:          return 63;
			case CustomStatistic::InteractWithLoom:             return 66;
			case CustomStatistic::InteractWithSmithingTable:    return 74;
			case CustomStatistic::InteractWithSmoker:           return 62;
			case CustomStatistic::InteractWithStonecutter:      return 67;
			case CustomStatistic::Jump:                         return 21;
			case CustomStatistic::LeaveGame:                    return 0;
			case CustomStatistic::MinecartOneCm:                return 14;
			case CustomStatistic::MobKills:                     return 31;
			case CustomStatistic::OpenBarrel:                   return 60;
			case CustomStatistic::OpenChest:                    return 57;
			case CustomStatistic::OpenEnderchest:               return 52;
			case CustomStatistic::OpenShulkerBox:               return 59;
			case CustomStatistic::PigOneCm:                     return 16;
			case CustomStatistic::PlayNoteblock:                return 48;
			case CustomStatistic::PlayRecord:                   return 54;
			case CustomStatistic::PlayTime:                     return 1;
			case CustomStatistic::PlayerKills:                  return 33;
			case CustomStatistic::PotFlower:                    return 50;
			case CustomStatistic::RaidTrigger:                  return 69;
			case CustomStatistic::RaidWin:                      return 70;
			case CustomStatistic::SleepInBed:                   return 58;
			case CustomStatistic::SneakTime:                    return 5;
			case CustomStatistic::SprintOneCm:                  return 8;
			case CustomStatistic::StriderOneCm:                 return 20;
			case CustomStatistic::SwimOneCm:                    return 19;
			case CustomStatistic::TalkedToVillager:             return 35;
			case CustomStatistic::TargetHit:                    return 73;
			case CustomStatistic::TimeSinceDeath:               return 3;
			case CustomStatistic::TimeSinceRest:                return 4;
			case CustomStatistic::TotalWorldTime:               return 2;
			case CustomStatistic::TradedWithVillager:           return 36;
			case CustomStatistic::TriggerTrappedChest:          return 51;
			case CustomStatistic::TuneNoteblock:                return 49;
			case CustomStatistic::UseCauldron:                  return 39;
			case CustomStatistic::WalkOnWaterOneCm:             return 9;
			case CustomStatistic::WalkOneCm:                    return 6;
			case CustomStatistic::WalkUnderWaterOneCm:          return 13;
			default: return 0;
		}
	}
	Item ToItem(const UInt32 ID)
	{
		return cRoot::Get()->GetRegistryMap()->ProtocolToItem(ID, cProtocol::Version::v1_21_4);
	}
	UInt32 From(const Item ID)
	{
		return cRoot::Get()->GetRegistryMap()->ItemToProtocol(ID, cProtocol::Version::v1_21_4);
	}
}  // namespace Palette_1_21_4
