#include "Globals.h"

#include "FluidSimulator.h"
#include "../World.h"





cFluidSimulator::cFluidSimulator(cWorld & a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid) :
	super(a_World),
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
		case E_BLOCK_COBWEB:
		case E_BLOCK_CROPS:
		case E_BLOCK_DEAD_BUSH:
		case E_BLOCK_DETECTOR_RAIL:
		case E_BLOCK_INACTIVE_COMPARATOR:
		case E_BLOCK_LILY_PAD:
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
		case E_BLOCK_TRIPWIRE_HOOK:
		case E_BLOCK_TRIPWIRE:
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





Vector3f cFluidSimulator::GetFlowingDirectionVec(int a_X, int a_Y, int a_Z, bool a_Over)
{
	Vector3f vDirection = { 0.0, 0.0, 0.0 };

	if (!cChunkDef::IsValidHeight(a_Y))
	{
		return vDirection;
	}
	BLOCKTYPE BlockID = m_World.GetBlock(a_X, a_Y, a_Z);
	if (!IsAllowedBlock(BlockID))  // No Fluid -> No Flowing direction :D
	{
		return vDirection;
	}

	NIBBLETYPE CentralPoint = m_World.GetBlockMeta(a_X, a_Y, a_Z);  // Current Block Meta so only lower points will be counted

	if (CentralPoint == 8)
	{
		CentralPoint = 0;
	}

	if (IsAllowedBlock(m_World.GetBlock(a_X, a_Y + 1, a_Z)) && a_Over)  // check for upper block to flow because this also affects the flowing direction
	{
		return GetFlowingDirectionVec(a_X, a_Y + 1, a_Z, false);
	}

	std::vector< Vector3i * > Points;

	Points.reserve(4);  // Already allocate 4 places :D

	// add blocks around the checking pos

	Points.push_back(new Vector3i(a_X + 1, a_Y, a_Z));
	Points.push_back(new Vector3i(a_X, a_Y, a_Z + 1));
	Points.push_back(new Vector3i(a_X - 1, a_Y, a_Z));
	Points.push_back(new Vector3i(a_X, a_Y, a_Z - 1));

	int LevelPoint[4];
	int i = 0;

	for (auto itr = Points.cbegin(), end = Points.cend(); itr != end; ++itr)
	{
		Vector3i * Pos = (*itr);
		auto PosBlockID = m_World.GetBlock(Pos->x, Pos->y, Pos->z);
		if (IsAllowedBlock(PosBlockID))
		{
			LevelPoint[i] = m_World.GetBlockMeta(Pos->x, Pos->y, Pos->z);
			if (LevelPoint[i] == 8)
			{
				LevelPoint[i] = CentralPoint;
			}
		}
		else if (PosBlockID == E_BLOCK_AIR)
		{
			if (CentralPoint == 7)
			{
				LevelPoint[i] = 8;
			}
			else
			{
				LevelPoint[i] = CentralPoint;
			}
		}
		else
		{
			LevelPoint[i] = CentralPoint;
		}
		delete Pos;
		Pos = nullptr;
		i++;
	}

	// Calculate the flow direction

	vDirection.x = 1.0f * (LevelPoint[0] - CentralPoint) + -1.0f * (LevelPoint[2] - CentralPoint);
	vDirection.z = 1.0f * (LevelPoint[1] - CentralPoint) + -1.0f * (LevelPoint[3] - CentralPoint);

	vDirection.Normalize();

	return vDirection;
}


