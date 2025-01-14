
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "SandSimulator.h"
#include "../BlockInfo.h"
#include "../World.h"
#include "../Defines.h"
#include "../Entities/FallingBlock.h"
#include "../Chunk.h"
#include "../IniFile.h"
#include "../EffectID.h"

#include "../Blocks/BlockSlab.h"
#include "../Blocks/BlockAnvil.h"
#include "../Blocks/BlockConcretePowder.h"
#include "../Protocol/Palettes/Upgrade.h"
#include "../Registries/BlockItemConverter.h"





cSandSimulator::cSandSimulator(cWorld & a_World, cIniFile & a_IniFile) :
	cSimulator(a_World),
	m_TotalBlocks(0)
{
	m_IsInstantFall = a_IniFile.GetValueSetB("Physics", "SandInstantFall", false);
}





void cSandSimulator::SimulateChunk(std::chrono::milliseconds a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk)
{
	cSandSimulatorChunkData & ChunkData = a_Chunk->GetSandSimulatorData();
	if (ChunkData.empty())
	{
		return;
	}

	int BaseX = a_Chunk->GetPosX() * cChunkDef::Width;
	int BaseZ = a_Chunk->GetPosZ() * cChunkDef::Width;
	for (cSandSimulatorChunkData::const_iterator itr = ChunkData.begin(), end = ChunkData.end(); itr != end; ++itr)
	{
		auto Neighbor = a_Chunk->GetBlock(itr->x, itr->y, itr->z);
		if (!IsAllowedBlock(Neighbor) || (itr->y <= 0))
		{
			continue;
		}

		auto BlockBelow = (itr->y > 0) ? a_Chunk->GetBlock(itr->x, itr->y - 1, itr->z) : Block::Air::Air();
		if (CanStartFallingThrough(BlockBelow))
		{
			if (m_IsInstantFall)
			{
				DoInstantFall(a_Chunk, {itr->x, itr->y, itr->z});
				continue;
			}
			Vector3i Pos;
			Pos.x = itr->x + BaseX;
			Pos.y = itr->y;
			Pos.z = itr->z + BaseZ;

			SANDLOG(
				"Creating a falling block at {0} of type {1}, block below: {2}",
				Pos, "MISSING FUNCTION BlockState -> String", "MISSING FUNCTION BlockState -> String"
			);
			// ItemTypeToString(BlockType), ItemTypeToString(BlockBelow)


			m_World.SpawnFallingBlock(Pos, Neighbor);
			a_Chunk->SetBlock({itr->x, itr->y, itr->z}, Block::Air::Air());
		}
	}
	m_TotalBlocks -= static_cast<int>(ChunkData.size());
	ChunkData.clear();
}





void cSandSimulator::AddBlock(cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block)
{
	if (!IsAllowedBlock(a_Block))
	{
		return;
	}

	// Check for duplicates:
	cSandSimulatorChunkData & ChunkData = a_Chunk.GetSandSimulatorData();
	for (cSandSimulatorChunkData::iterator itr = ChunkData.begin(); itr != ChunkData.end(); ++itr)
	{
		if ((itr->x == a_Position.x) && (itr->y == a_Position.y) && (itr->z == a_Position.z))
		{
			return;
		}
	}

	m_TotalBlocks += 1;
	ChunkData.emplace_back(a_Position.x, a_Position.y, a_Position.z);
}





bool cSandSimulator::CanStartFallingThrough(BlockState a_Block)
{
	// Please keep the list alpha-sorted
	switch (a_Block.Type())
	{
		case BlockType::Air:
		case BlockType::CaveAir:
		case BlockType::VoidAir:
		case BlockType::Fire:
		case BlockType::Lava:
		case BlockType::Snow:
		case BlockType::Water:
		{
			return true;
		}
		default: return false;
	}
}





bool cSandSimulator::CanContinueFallThrough(BlockState a_Block)
{
	// Please keep the list alpha-sorted
	switch (a_Block.Type())
	{
		case BlockType::Air:
		case BlockType::CaveAir:
		case BlockType::VoidAir:
		case BlockType::Beetroots:
		case BlockType::BrownMushroom:
		case BlockType::Carrots:
		case BlockType::Cobweb:
		case BlockType::DeadBush:
		case BlockType::DetectorRail:
		case BlockType::Fire:
		case BlockType::HeavyWeightedPressurePlate:
		case BlockType::Lava:
		case BlockType::Lever:
		case BlockType::LightWeightedPressurePlate:
		case BlockType::Rail:
		case BlockType::MelonStem:
		case BlockType::AttachedMelonStem:
		case BlockType::PoweredRail:
		case BlockType::PottedAcaciaSapling:
		case BlockType::PottedAllium:
		case BlockType::PottedAzureBluet:
		case BlockType::PottedBamboo:
		case BlockType::PottedBirchSapling:
		case BlockType::PottedBlueOrchid:
		case BlockType::PottedBrownMushroom:
		case BlockType::PottedCactus:
		case BlockType::PottedCornflower:
		case BlockType::PottedCrimsonFungus:
		case BlockType::PottedCrimsonRoots:
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
		case BlockType::PumpkinStem:
		case BlockType::AttachedPumpkinStem:
		case BlockType::Repeater:
		case BlockType::RedstoneTorch:
		case BlockType::RedstoneWallTorch:
		case BlockType::RedstoneWire:
		case BlockType::RedMushroom:
		case BlockType::Snow:
		case BlockType::StonePressurePlate:
		case BlockType::Torch:
		case BlockType::WallTorch:
		case BlockType::Tripwire:
		case BlockType::TripwireHook:
		case BlockType::Water:
		case BlockType::Wheat:

			// Banner
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
			// Big Flowers
		case BlockType::TallGrass:
		case BlockType::LargeFern:
		case BlockType::Lilac:
		case BlockType::Peony:
		case BlockType::RoseBush:
		case BlockType::Sunflower:
			// Buttons
		case BlockType::AcaciaButton:
		case BlockType::BirchButton:
		case BlockType::CrimsonButton:
		case BlockType::DarkOakButton:
		case BlockType::JungleButton:
		case BlockType::OakButton:
		case BlockType::PolishedBlackstoneButton:
		case BlockType::SpruceButton:
		case BlockType::StoneButton:
		case BlockType::WarpedButton:
			// Small Flowers
		case BlockType::Allium:
		case BlockType::AzureBluet:
		case BlockType::BlueOrchid:
		case BlockType::Cornflower:
		case BlockType::LilyOfTheValley:
		case BlockType::OrangeTulip:
		case BlockType::OxeyeDaisy:
		case BlockType::PinkTulip:
		case BlockType::Poppy:
		case BlockType::RedTulip:
		case BlockType::WhiteTulip:
			// Signs
		case BlockType::AcaciaSign:
		case BlockType::BirchSign:
		case BlockType::CrimsonSign:
		case BlockType::DarkOakSign:
		case BlockType::JungleSign:
		case BlockType::OakSign:
		case BlockType::SpruceSign:
		case BlockType::WarpedSign:
			// WallSigns
		case BlockType::AcaciaWallSign:
		case BlockType::BirchWallSign:
		case BlockType::CrimsonWallSign:
		case BlockType::DarkOakWallSign:
		case BlockType::OakWallSign:
		case BlockType::JungleWallSign:
		case BlockType::WarpedWallSign:
		case BlockType::SpruceWallSign:
			// WoodenPressurePlates
		case BlockType::AcaciaPressurePlate:
		case BlockType::BirchPressurePlate:
		case BlockType::CrimsonPressurePlate:
		case BlockType::DarkOakPressurePlate:
		case BlockType::JunglePressurePlate:
		case BlockType::OakPressurePlate:
		case BlockType::PolishedBlackstonePressurePlate:
		case BlockType::SprucePressurePlate:
		case BlockType::WarpedPressurePlate:
			// Trapdoor
		case BlockType::AcaciaTrapdoor:
		case BlockType::BirchTrapdoor:
		case BlockType::CrimsonTrapdoor:
		case BlockType::DarkOakTrapdoor:
		case BlockType::IronTrapdoor:
		case BlockType::JungleTrapdoor:
		case BlockType::OakTrapdoor:
		case BlockType::SpruceTrapdoor:
		case BlockType::WarpedTrapdoor:
			return true;
		default: return false;
	}
}





bool cSandSimulator::IsReplacedOnRematerialization(BlockState a_Block)
{
	// Please keep the list alpha-sorted
	switch (a_Block.Type())
	{
		case BlockType::Air:
		case BlockType::ChorusFlower:
		case BlockType::ChorusPlant:
		case BlockType::DeadBush:
		case BlockType::Fire:
		case BlockType::Lava:
		case BlockType::Snow:
		case BlockType::StructureVoid:
		case BlockType::TallGrass:
		case BlockType::Water:
			return true;
		default: return false;
	}
}





bool cSandSimulator::DoesBreakFallingThrough(BlockState a_Block)
{
	if (cBlockSlabHandler::IsAnySlabType(a_Block))
	{
		return !cBlockSlabHandler::IsSlabTop(a_Block);
	}
	return false;
}





void cSandSimulator::FinishFalling(
	cWorld * a_World, Vector3i a_Pos,
	BlockState a_FallingBlock
)
{
	ASSERT(a_Pos.y < cChunkDef::Height);
	auto CurrentBlock = a_World->GetBlock(a_Pos);
	if (IsBlockAnvil(a_FallingBlock) || IsReplacedOnRematerialization(CurrentBlock))
	{
		// Rematerialize the material here:
		a_World->SetBlock(a_Pos, a_FallingBlock);
		if (IsBlockAnvil(a_FallingBlock))
		{
			a_World->BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_ANVIL_LAND, a_Pos, 0);
		}
		return;
	}

	// Create a pickup instead:
	cItems Pickups;
	Pickups.Add(BlockItemConverter::FromBlock(a_FallingBlock.Type()));
	a_World->SpawnItemPickups(
		Pickups,
		Vector3d(a_Pos) + Vector3d(0.5, 0.5, 0.5)
	);
}





bool cSandSimulator::IsAllowedBlock(BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::Anvil:
		case BlockType::ChippedAnvil:
		case BlockType::DamagedAnvil:
		case BlockType::BlackConcretePowder:
		case BlockType::BlueConcretePowder:
		case BlockType::BrownConcretePowder:
		case BlockType::CyanConcretePowder:
		case BlockType::GrayConcretePowder:
		case BlockType::GreenConcretePowder:
		case BlockType::LightBlueConcretePowder:
		case BlockType::LightGrayConcretePowder:
		case BlockType::LimeConcretePowder:
		case BlockType::MagentaConcretePowder:
		case BlockType::OrangeConcretePowder:
		case BlockType::PinkConcretePowder:
		case BlockType::PurpleConcretePowder:
		case BlockType::RedConcretePowder:
		case BlockType::WhiteConcretePowder:
		case BlockType::YellowConcretePowder:
		case BlockType::DragonEgg:
		case BlockType::Gravel:
		case BlockType::Sand:
		case BlockType::RedSand:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





void cSandSimulator::DoInstantFall(cChunk * a_Chunk, Vector3i a_RelPos)
{
	// Remove the original block:
	auto FallingBlock = a_Chunk->GetBlock(a_RelPos);
	a_Chunk->SetBlock(a_RelPos, Block::Air::Air());

	// Search for a place to put it:
	for (int y = a_RelPos.y - 1; y >= 0; y--)
	{
		auto NewBlock = a_Chunk->GetBlock(a_RelPos.x, y, a_RelPos.z);
		int BlockY;
		if (DoesBreakFallingThrough(NewBlock))
		{
			BlockY = y;
		}
		else if (!CanContinueFallThrough(NewBlock))
		{
			BlockY = y + 1;
		}
		else if ((cBlockConcretePowderHandler::IsBlockConcretePowder(FallingBlock)) && NewBlock.Type() == BlockType::Water)
		{
			FallingBlock = cBlockConcretePowderHandler::GetConcreteFromConcretePowder(FallingBlock);
			BlockY = y;
		}
		else
		{
			// Can fall further down
			continue;
		}

		// Finish the fall at the found bottom:
		int BlockX = a_RelPos.x + a_Chunk->GetPosX() * cChunkDef::Width;
		int BlockZ = a_RelPos.z + a_Chunk->GetPosZ() * cChunkDef::Width;
		FinishFalling(&m_World, {BlockX, BlockY, BlockZ}, FallingBlock);
		return;
	}

	// The block just "fell off the world" without leaving a trace
}




