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





Vector3f cFluidSimulator::GetFlowingDirection(int a_X, int a_Y, int a_Z)
{
	Vector3f vDirection;

	if (!cChunkDef::IsValidHeight(a_Y))
	{
		return vDirection;
	}

	BLOCKTYPE BlockID = m_World.GetBlock(a_X, a_Y, a_Z);

	if (!IsAllowedBlock(BlockID))  // No Fluid -> No Flowing direction :D
	{
		return vDirection;
	}

	NIBBLETYPE CentralPoint = m_World.GetBlockMeta(a_X, a_Y, a_Z);

	if (CentralPoint > 7)
	{
		CentralPoint -= 8;
	}

	int LevelPoint[4];
	Vector3i Points[4];	// blocks around the checking pos
	Points[0] = { a_X + 1, a_Y, a_Z };
	Points[1] = { a_X, a_Y, a_Z + 1 };
	Points[2] = { a_X - 1, a_Y, a_Z };
	Points[3] = { a_X, a_Y, a_Z - 1 };

	for (int i = 0; i < ARRAYCOUNT(LevelPoint); i++)
	{
		BlockID = m_World.GetBlock(Points[i].x, Points[i].y, Points[i].z);

		if (IsAllowedBlock(BlockID))
		{
			LevelPoint[i] = m_World.GetBlockMeta(Points[i].x, Points[i].y, Points[i].z);

			if (LevelPoint[i] > 7)
			{
				LevelPoint[i] -= 8;
			}
			else if (LevelPoint[i] == 0)
			{
				BlockID = m_World.GetBlock(Points[i].x, Points[i].y - 1, Points[i].z);

				if (BlockID == m_FluidBlock)
				{
					LevelPoint[i] = CentralPoint + 1;
				}
				else if (BlockID == m_StationaryFluidBlock)
				{
					LevelPoint[i] = CentralPoint;
				}
			}
		}
		else if (BlockID == E_BLOCK_AIR)
		{
			BlockID = m_World.GetBlock(Points[i].x, Points[i].y - 1, Points[i].z);

			if (IsAllowedBlock(BlockID))
			{
				LevelPoint[i] = CentralPoint + 1;
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
	}

	// Calculate the flow direction

	vDirection.x = LevelPoint[0] - LevelPoint[2];
	vDirection.z = LevelPoint[1] - LevelPoint[3];

	float Length = static_cast<float>(sqrt(vDirection.x * vDirection.x + vDirection.z * vDirection.z));

	if (Length != 0.0f)
	{
		float Len = 1.0f / Length;

		vDirection.x *= Len;
		vDirection.z *= Len;
	}

	return vDirection;
}

