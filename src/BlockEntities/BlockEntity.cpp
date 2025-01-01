
// BlockEntity.cpp

// Implements the cBlockEntity class that is the common ancestor for all block entities

#include "Globals.h"
#include "BannerEntity.h"
#include "BeaconEntity.h"
#include "BedEntity.h"
#include "BlockEntity.h"
#include "EnchantingTableEntity.h"
#include "BrewingstandEntity.h"
#include "ChestEntity.h"
#include "CommandBlockEntity.h"
#include "DispenserEntity.h"
#include "DropperEntity.h"
#include "EnderChestEntity.h"
#include "EndPortalEntity.h"
#include "FlowerPotEntity.h"
#include "FurnaceEntity.h"
#include "HopperEntity.h"
#include "MobHeadEntity.h"
#include "MobSpawnerEntity.h"
#include "JukeboxEntity.h"
#include "NoteEntity.h"
#include "SignEntity.h"





cBlockEntity::cBlockEntity(BlockState a_Block, const Vector3i a_Pos, cWorld * const a_World) :
	m_Pos(a_Pos),
	m_RelX(a_Pos.x - cChunkDef::Width * FAST_FLOOR_DIV(a_Pos.x, cChunkDef::Width)),
	m_RelZ(a_Pos.z - cChunkDef::Width * FAST_FLOOR_DIV(a_Pos.z, cChunkDef::Width)),
	m_Block(a_Block),
	m_World(a_World)
{
}





OwnedBlockEntity cBlockEntity::Clone(const Vector3i a_Pos)
{
	auto res = CreateByBlockType(m_Block, a_Pos, nullptr);
	res->CopyFrom(*this);
	return res;
}





cItems cBlockEntity::ConvertToPickups() const
{
	return {};
}





void cBlockEntity::CopyFrom(const cBlockEntity & a_Src)
{
	// Nothing to copy, but check that we're copying the right entity:
	ASSERT(m_Block == a_Src.m_Block);
}





OwnedBlockEntity cBlockEntity::CreateByBlockType(BlockState a_Block, const Vector3i a_Pos, cWorld * const a_World)
{
	switch (a_Block.Type())
	{
		case BlockType::BlackBanner:
		case BlockType::BlueBanner:
		case BlockType::BrownBanner:
		case BlockType::CyanBanner:
		case BlockType::GrayBanner:
		case BlockType::GreenBanner:
		case BlockType::LightBlueBanner:
		case BlockType::LightGrayBanner:
		case BlockType::LimeBanner:
		case BlockType::MagentaBanner:
		case BlockType::OrangeBanner:
		case BlockType::PinkBanner:
		case BlockType::PurpleBanner:
		case BlockType::RedBanner:
		case BlockType::WhiteBanner:
		case BlockType::YellowBanner:

		case BlockType::BlackWallBanner:
		case BlockType::BlueWallBanner:
		case BlockType::BrownWallBanner:
		case BlockType::CyanWallBanner:
		case BlockType::GrayWallBanner:
		case BlockType::GreenWallBanner:
		case BlockType::LightBlueWallBanner:
		case BlockType::LightGrayWallBanner:
		case BlockType::LimeWallBanner:
		case BlockType::MagentaWallBanner:
		case BlockType::OrangeWallBanner:
		case BlockType::PinkWallBanner:
		case BlockType::PurpleWallBanner:
		case BlockType::RedWallBanner:
		case BlockType::WhiteWallBanner:
		case BlockType::YellowWallBanner: return std::make_unique<cBannerEntity>       (a_Block, a_Pos, a_World);

		case BlockType::Beacon:          return std::make_unique<cBeaconEntity>        (a_Block, a_Pos, a_World);
		case BlockType::BlackBed:
		case BlockType::BlueBed:
		case BlockType::BrownBed:
		case BlockType::CyanBed:
		case BlockType::GrayBed:
		case BlockType::GreenBed:
		case BlockType::LightBlueBed:
		case BlockType::LightGrayBed:
		case BlockType::LimeBed:
		case BlockType::MagentaBed:
		case BlockType::OrangeBed:
		case BlockType::PinkBed:
		case BlockType::PurpleBed:
		case BlockType::RedBed:
		case BlockType::WhiteBed:
		case BlockType::YellowBed:       return std::make_unique<cBedEntity>            (a_Block, a_Pos, a_World);

		case BlockType::BrewingStand:    return std::make_unique<cBrewingstandEntity>   (a_Block, a_Pos, a_World);
		case BlockType::Chest:           return std::make_unique<cChestEntity>          (a_Block, a_Pos, a_World);
		case BlockType::CommandBlock:
		case BlockType::ChainCommandBlock:
		case BlockType::RepeatingCommandBlock: return std::make_unique<cCommandBlockEntity>   (a_Block, a_Pos, a_World);
		case BlockType::Dispenser:       return std::make_unique<cDispenserEntity>      (a_Block, a_Pos, a_World);
		case BlockType::Dropper:         return std::make_unique<cDropperEntity>        (a_Block, a_Pos, a_World);
		case BlockType::EnchantingTable: return std::make_unique<cEnchantingTableEntity>(a_Block, a_Pos, a_World);
		case BlockType::EnderChest:      return std::make_unique<cEnderChestEntity>     (a_Block, a_Pos, a_World);
		case BlockType::EndPortal:       return std::make_unique<cEndPortalEntity>      (a_Block, a_Pos, a_World);
		case BlockType::FlowerPot:       return std::make_unique<cFlowerPotEntity>      (a_Block, a_Pos, a_World);
		case BlockType::Furnace:         return std::make_unique<cFurnaceEntity>        (a_Block, a_Pos, a_World);

		case BlockType::CreeperHead:
		case BlockType::CreeperWallHead:
		case BlockType::DragonHead:
		case BlockType::DragonWallHead:
		case BlockType::PlayerHead:
		case BlockType::PlayerWallHead:
		case BlockType::ZombieHead:
		case BlockType::ZombieWallHead:
		case BlockType::SkeletonSkull:
		case BlockType::SkeletonWallSkull:
		case BlockType::WitherSkeletonSkull:
		case BlockType::WitherSkeletonWallSkull:
		case BlockType::PiglinHead:
		case BlockType::PiglinWallHead:  return std::make_unique<cMobHeadEntity>(a_Block, a_Pos, a_World);

		case BlockType::Hopper:          return std::make_unique<cHopperEntity>    (a_Block, a_Pos, a_World);
		case BlockType::Jukebox:         return std::make_unique<cJukeboxEntity>   (a_Block, a_Pos, a_World);
		case BlockType::Spawner:         return std::make_unique<cMobSpawnerEntity>(a_Block, a_Pos, a_World);
		case BlockType::NoteBlock:       return std::make_unique<cNoteEntity>      (a_Block, a_Pos, a_World);

		case BlockType::PottedAcaciaSapling:
		case BlockType::PottedAzureBluet:
		case BlockType::PottedBamboo:
		case BlockType::PottedBirchSapling:
		case BlockType::PottedBlueOrchid:
		case BlockType::PottedBrownMushroom:
		case BlockType::PottedCactus:
		case BlockType::PottedCornflower:
		case BlockType::PottedCrimsonRoots:
		case BlockType::PottedCrimsonFungus:
		case BlockType::PottedDandelion:
		case BlockType::PottedDarkOakSapling:
		case BlockType::PottedDeadBush:
		case BlockType::PottedFern:
		case BlockType::PottedJungleSapling:
		case BlockType::PottedLilyOfTheValley:
		case BlockType::PottedOakSapling:
		case BlockType::PottedOrangeTulip:
		case BlockType::PottedOxeyeDaisy:
		case BlockType::PottedPinkTulip:
		case BlockType::PottedPoppy:
		case BlockType::PottedRedMushroom:
		case BlockType::PottedRedTulip:
		case BlockType::PottedSpruceSapling:
		case BlockType::PottedWarpedFungus:
		case BlockType::PottedWarpedRoots:
		case BlockType::PottedWhiteTulip:
		case BlockType::PottedWitherRose:
		case BlockType::PottedAllium:       return std::make_unique<cFlowerPotEntity>(a_Block, a_Pos, a_World);

		case BlockType::AcaciaSign:
		case BlockType::AcaciaWallSign:
		case BlockType::BirchSign:
		case BlockType::BirchWallSign:
		case BlockType::CrimsonSign:
		case BlockType::CrimsonWallSign:
		case BlockType::DarkOakSign:
		case BlockType::DarkOakWallSign:
		case BlockType::JungleSign:
		case BlockType::JungleWallSign:
		case BlockType::OakSign:
		case BlockType::OakWallSign:
		case BlockType::SpruceSign:
		case BlockType::SpruceWallSign:
		case BlockType::WarpedSign:
		case BlockType::WarpedWallSign: return std::make_unique<cSignEntity> (a_Block, a_Pos, a_World);

		case BlockType::TrappedChest:   return std::make_unique<cChestEntity>(a_Block, a_Pos, a_World);
		default:
		{
			FLOGD("{}: Requesting creation of an unknown block entity - block {}",
				__FUNCTION__, a_Block.Type()
			);
			ASSERT(!"Requesting creation of an unknown block entity");
			return nullptr;
		}
	}
}





void cBlockEntity::Destroy()
{
}





bool cBlockEntity::IsBlockEntityBlockType(const BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::BlackBanner:
		case BlockType::BlueBanner:
		case BlockType::BrownBanner:
		case BlockType::CyanBanner:
		case BlockType::GrayBanner:
		case BlockType::GreenBanner:
		case BlockType::LightBlueBanner:
		case BlockType::LightGrayBanner:
		case BlockType::LimeBanner:
		case BlockType::MagentaBanner:
		case BlockType::OrangeBanner:
		case BlockType::PinkBanner:
		case BlockType::PurpleBanner:
		case BlockType::RedBanner:
		case BlockType::WhiteBanner:
		case BlockType::YellowBanner:

		case BlockType::BlackWallBanner:
		case BlockType::BlueWallBanner:
		case BlockType::BrownWallBanner:
		case BlockType::CyanWallBanner:
		case BlockType::GrayWallBanner:
		case BlockType::GreenWallBanner:
		case BlockType::LightBlueWallBanner:
		case BlockType::LightGrayWallBanner:
		case BlockType::LimeWallBanner:
		case BlockType::MagentaWallBanner:
		case BlockType::OrangeWallBanner:
		case BlockType::PinkWallBanner:
		case BlockType::PurpleWallBanner:
		case BlockType::RedWallBanner:
		case BlockType::WhiteWallBanner:
		case BlockType::YellowWallBanner:

		case BlockType::Beacon:

		case BlockType::BlackBed:
		case BlockType::BlueBed:
		case BlockType::BrownBed:
		case BlockType::CyanBed:
		case BlockType::GrayBed:
		case BlockType::GreenBed:
		case BlockType::LightBlueBed:
		case BlockType::LightGrayBed:
		case BlockType::LimeBed:
		case BlockType::MagentaBed:
		case BlockType::OrangeBed:
		case BlockType::PinkBed:
		case BlockType::PurpleBed:
		case BlockType::RedBed:
		case BlockType::WhiteBed:
		case BlockType::YellowBed:

		case BlockType::BrewingStand:
		case BlockType::Chest:

		case BlockType::CommandBlock:
		case BlockType::ChainCommandBlock:
		case BlockType::RepeatingCommandBlock:

		case BlockType::Dispenser:
		case BlockType::Dropper:
		case BlockType::EnchantingTable:
		case BlockType::EnderChest:
		case BlockType::EndPortal:
		case BlockType::FlowerPot:
		case BlockType::Furnace:

		case BlockType::CreeperHead:
		case BlockType::CreeperWallHead:
		case BlockType::DragonHead:
		case BlockType::DragonWallHead:
		case BlockType::PlayerHead:
		case BlockType::PlayerWallHead:
		case BlockType::ZombieHead:
		case BlockType::ZombieWallHead:
		case BlockType::SkeletonSkull:
		case BlockType::SkeletonWallSkull:
		case BlockType::WitherSkeletonSkull:
		case BlockType::WitherSkeletonWallSkull:

		case BlockType::Hopper:
		case BlockType::Jukebox:
		case BlockType::NoteBlock:

		case BlockType::PottedAcaciaSapling:
		case BlockType::PottedAzureBluet:
		case BlockType::PottedBamboo:
		case BlockType::PottedBirchSapling:
		case BlockType::PottedBlueOrchid:
		case BlockType::PottedBrownMushroom:
		case BlockType::PottedCactus:
		case BlockType::PottedCornflower:
		case BlockType::PottedCrimsonRoots:
		case BlockType::PottedCrimsonFungus:
		case BlockType::PottedDandelion:
		case BlockType::PottedDarkOakSapling:
		case BlockType::PottedDeadBush:
		case BlockType::PottedFern:
		case BlockType::PottedJungleSapling:
		case BlockType::PottedLilyOfTheValley:
		case BlockType::PottedOakSapling:
		case BlockType::PottedOrangeTulip:
		case BlockType::PottedOxeyeDaisy:
		case BlockType::PottedPinkTulip:
		case BlockType::PottedPoppy:
		case BlockType::PottedRedMushroom:
		case BlockType::PottedRedTulip:
		case BlockType::PottedSpruceSapling:
		case BlockType::PottedWarpedFungus:
		case BlockType::PottedWarpedRoots:
		case BlockType::PottedWhiteTulip:
		case BlockType::PottedWitherRose:
		case BlockType::PottedAllium:

		case BlockType::AcaciaSign:
		case BlockType::AcaciaWallSign:
		case BlockType::BirchSign:
		case BlockType::BirchWallSign:
		case BlockType::CrimsonSign:
		case BlockType::CrimsonWallSign:
		case BlockType::DarkOakSign:
		case BlockType::DarkOakWallSign:
		case BlockType::JungleSign:
		case BlockType::JungleWallSign:
		case BlockType::OakSign:
		case BlockType::OakWallSign:
		case BlockType::SpruceSign:
		case BlockType::SpruceWallSign:
		case BlockType::WarpedSign:
		case BlockType::WarpedWallSign:

		case BlockType::Spawner:
		case BlockType::TrappedChest:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





void cBlockEntity::OnAddToWorld(cWorld & a_World, cChunk & a_Chunk)
{
	m_World = &a_World;
}





void cBlockEntity::OnRemoveFromWorld()
{
}





void cBlockEntity::SetPos(const Vector3i a_NewPos)
{
	ASSERT(m_World == nullptr);  // Cannot move block entities that represent world blocks (only use this for cBlockArea's BEs)
	m_Pos = a_NewPos;
}





void cBlockEntity::SetWorld(cWorld * const a_World)
{
	m_World = a_World;
}





bool cBlockEntity::Tick(const std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	return false;
}
