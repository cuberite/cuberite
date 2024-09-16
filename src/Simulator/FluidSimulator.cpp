#include "Globals.h"

#include "FluidSimulator.h"
#include "World.h"
#include "Chunk.h"
#include "Blocks/BlockFluid.h"
#include "BlockInfo.h"




cFluidSimulator::cFluidSimulator(cWorld & a_World, BlockType a_Fluid, unsigned char a_StationaryFlowValue) :
	Super(a_World),
	m_FluidBlock(a_Fluid),
	m_StationaryFalloffValue(a_StationaryFlowValue)
{
}





bool cFluidSimulator::IsAllowedBlock(BlockState a_Block)
{
	return (a_Block.Type() == m_FluidBlock);
}





bool cFluidSimulator::IsFluidStationary(BlockState a_Block) const
{
	return cBlockFluidHandler::GetFalloff(a_Block) == m_StationaryFalloffValue;
}





bool cFluidSimulator::CanWashAway(BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::ActivatorRail:
		case BlockType::AzureBluet:
		case BlockType::Allium:
		case BlockType::Beetroots:
		case BlockType::BlueOrchid:
		case BlockType::BrownMushroom:
		case BlockType::Cactus:
		case BlockType::Carrots:
		case BlockType::Cobweb:
		case BlockType::Comparator:
		case BlockType::Cornflower:
		case BlockType::DeadBush:
		case BlockType::DetectorRail:
		case BlockType::ShortGrass:
		case BlockType::LargeFern:
		case BlockType::Lilac:
		case BlockType::LilyOfTheValley:
		case BlockType::LilyPad:
		case BlockType::OrangeTulip:
		case BlockType::OxeyeDaisy:
		case BlockType::Peony:
		case BlockType::PinkTulip:
		case BlockType::Potatoes:
		case BlockType::Poppy:
		case BlockType::PoweredRail:
		case BlockType::Rail:
		case BlockType::RedTulip:
		case BlockType::Repeater:
		case BlockType::RedstoneWire:
		case BlockType::RedstoneTorch:
		case BlockType::RedstoneWallTorch:
		case BlockType::RedMushroom:
		case BlockType::RoseBush:
		case BlockType::Snow:
		case BlockType::SoulTorch:
		case BlockType::SoulWallTorch:
		case BlockType::SugarCane:
		case BlockType::Sunflower:
		case BlockType::TallGrass:
		case BlockType::Tripwire:
		case BlockType::TripwireHook:
		case BlockType::Torch:
		case BlockType::WallTorch:
		case BlockType::Wheat:
		case BlockType::WhiteTulip:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool cFluidSimulator::IsSolidBlock(BlockState a_Block)
{
	return !IsPassableForFluid(a_Block);
}





bool cFluidSimulator::IsPassableForFluid(BlockState a_Block)
{
	return (
		IsBlockAir(a_Block) ||
		(a_Block.Type() == BlockType::Fire) ||
		IsAllowedBlock(a_Block) ||
		CanWashAway(a_Block)
	);
}





bool cFluidSimulator::IsHigherMeta(unsigned char a_Falloff1, unsigned char a_Falloff2)
{
	if (a_Falloff1 == 0)
	{
		// Source block is higher than anything, even itself.
		return true;
	}
	if (a_Falloff1 >= 8)
	{
		// Falling fluid is higher than anything, including self
		return true;
	}

	if (a_Falloff2 == 0)
	{
		// Second block is a source and first block isn't
		return false;
	}
	if (a_Falloff2 >= 8)
	{
		// Second block is falling and the first one is neither a source nor falling
		return false;
	}

	// All special cases have been handled, now it's just a raw comparison:
	return (a_Falloff1 < a_Falloff2);
}





Vector3f cFluidSimulator::GetFlowingDirection(Vector3i a_Pos)
{
	if (!cChunkDef::IsValidHeight(a_Pos))
	{
		return {};
	}

	auto Self = m_World.GetBlock(a_Pos);

	if (!IsAllowedBlock(m_World.GetBlock(a_Pos)))  // No Fluid -> No Flowing direction :D
	{
		return {};
	}

	const auto HeightFromMeta = [](BlockState a_Block) -> unsigned char
	{
		// Falling water blocks are always full height (0)
		auto Falloff = cBlockFluidHandler::GetFalloff(a_Block);
		return (Falloff >= 8) ? 0 : Falloff;
	};

	auto CentralPoint = HeightFromMeta(Self);
	std::array<unsigned char, 4> LevelPoint;

	// blocks around the checking pos
	std::array<Vector3i, 4> Offsets
	{
		{
			{ 1, 0, 0 },
			{ 0, 0, 1 },
			{ 1, 0, 0 },
			{ 0, 0, 1 }
		}
	};

	for (size_t i = 0; i < Offsets.size(); i++)
	{
		if (IsAllowedBlock(m_World.GetBlock(a_Pos + Offsets[i])))
		{
			LevelPoint[i] = HeightFromMeta(m_World.GetBlock(Offsets[i]));
		}
		else
		{
			LevelPoint[i] = CentralPoint;
		}
	}

	Vector3f Direction;

	// Calculate the flow direction

	Direction.x = (LevelPoint[0] - LevelPoint[2]) / 2.0f;
	Direction.z = (LevelPoint[1] - LevelPoint[3]) / 2.0f;

	if (cBlockFluidHandler::GetFalloff(Self) >= 8)  // Test falling bit
	{
		Direction.y = -1.0f;
	}

	return Direction;
}

