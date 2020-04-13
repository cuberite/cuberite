#include "Globals.h"

#include "FluidSimulator.h"
#include "../World.h"





cFluidSimulator::cFluidSimulator(cWorld & a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid) :
	Super(a_World),
	m_FluidBlock(a_Fluid),
	m_StationaryFluidBlock(a_StationaryFluid)
{
}





bool cFluidSimulator::IsAllowedBlock(BLOCKTYPE a_BlockType)
{
	return ((a_BlockType == m_FluidBlock) || (a_BlockType == m_StationaryFluidBlock));
}





bool cFluidSimulator::CanWashAway(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_ACTIVATOR_RAIL:
		case E_BLOCK_ACTIVE_COMPARATOR:
		case E_BLOCK_BEETROOTS:
		case E_BLOCK_BIG_FLOWER:
		case E_BLOCK_BROWN_MUSHROOM:
		case E_BLOCK_CACTUS:
		case E_BLOCK_CARROTS:
		case E_BLOCK_COBWEB:
		case E_BLOCK_CROPS:
		case E_BLOCK_DEAD_BUSH:
		case E_BLOCK_DETECTOR_RAIL:
		case E_BLOCK_INACTIVE_COMPARATOR:
		case E_BLOCK_LILY_PAD:
		case E_BLOCK_POTATOES:
		case E_BLOCK_POWERED_RAIL:
		case E_BLOCK_RAIL:
		case E_BLOCK_REDSTONE_REPEATER_OFF:
		case E_BLOCK_REDSTONE_REPEATER_ON:
		case E_BLOCK_REDSTONE_TORCH_OFF:
		case E_BLOCK_REDSTONE_TORCH_ON:
		case E_BLOCK_REDSTONE_WIRE:
		case E_BLOCK_RED_MUSHROOM:
		case E_BLOCK_RED_ROSE:
		case E_BLOCK_SNOW:
		case E_BLOCK_SUGARCANE:
		case E_BLOCK_TALL_GRASS:
		case E_BLOCK_TORCH:
		case E_BLOCK_TRIPWIRE:
		case E_BLOCK_TRIPWIRE_HOOK:
		case E_BLOCK_YELLOW_FLOWER:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool cFluidSimulator::IsSolidBlock(BLOCKTYPE a_BlockType)
{
	return !IsPassableForFluid(a_BlockType);
}





bool cFluidSimulator::IsPassableForFluid(BLOCKTYPE a_BlockType)
{
	return (
		(a_BlockType == E_BLOCK_AIR) ||
		(a_BlockType == E_BLOCK_FIRE) ||
		IsAllowedBlock(a_BlockType) ||
		CanWashAway(a_BlockType)
	);
}





bool cFluidSimulator::IsHigherMeta(NIBBLETYPE a_Meta1, NIBBLETYPE a_Meta2)
{
	if (a_Meta1 == 0)
	{
		// Source block is higher than anything, even itself.
		return true;
	}
	if ((a_Meta1 & 0x08) != 0)
	{
		// Falling fluid is higher than anything, including self
		return true;
	}

	if (a_Meta2 == 0)
	{
		// Second block is a source and first block isn't
		return false;
	}
	if ((a_Meta2 & 0x08) != 0)
	{
		// Second block is falling and the first one is neither a source nor falling
		return false;
	}

	// All special cases have been handled, now it's just a raw comparison:
	return (a_Meta1 < a_Meta2);
}





Vector3f cFluidSimulator::GetFlowingDirection(int a_X, int a_Y, int a_Z)
{
	if (!cChunkDef::IsValidHeight(a_Y))
	{
		return {};
	}

	if (!IsAllowedBlock(m_World.GetBlock(a_X, a_Y, a_Z)))  // No Fluid -> No Flowing direction :D
	{
		return {};
	}

	const auto HeightFromMeta = [](NIBBLETYPE a_BlockMeta) -> NIBBLETYPE
		{
			// Falling water blocks are always full height (0)
			return ((a_BlockMeta & 0x08) != 0) ? 0 : a_BlockMeta;
		};

	auto BlockMeta = m_World.GetBlockMeta(a_X, a_Y, a_Z);
	NIBBLETYPE CentralPoint = HeightFromMeta(BlockMeta);
	NIBBLETYPE LevelPoint[4];

	// blocks around the checking pos
	Vector3i Points[]
	{
		{ a_X + 1, a_Y, a_Z },
		{ a_X, a_Y, a_Z + 1 },
		{ a_X - 1, a_Y, a_Z },
		{ a_X, a_Y, a_Z - 1 }
	};

	for (size_t i = 0; i < ARRAYCOUNT(LevelPoint); i++)
	{
		if (IsAllowedBlock(m_World.GetBlock(Points[i])))
		{
			LevelPoint[i] = HeightFromMeta(m_World.GetBlockMeta(Points[i]));
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

	if ((BlockMeta & 0x08) != 0)  // Test falling bit
	{
		Direction.y = -1.0f;
	}

	return Direction;
}

