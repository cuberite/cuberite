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

#define D_DN lp[dn]
#define D_UP lp[up]
#define D_RT lp[rt]
#define D_LT lp[lt]
//#define DEBUG_PICKUP_WATERMOVE


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

	NIBBLETYPE LowestPoint = m_World.GetBlockMeta(a_X, a_Y, a_Z);  // Current Block Meta so only lower points will be counted

    if(LowestPoint == 8)
        LowestPoint = 0;

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

	int lp[4];
	int i = 0;
	int ct = LowestPoint;
	int dir = -1;
	int dir_ = -1;
	int up, dn, lt, rt;
	float lrp = 0;

	for (auto itr = Points.cbegin(), end = Points.cend(); itr != end; ++itr)
	{
		Vector3i * Pos = (*itr);
		auto PosBlockID = m_World.GetBlock(Pos->x, Pos->y, Pos->z);
		if (IsAllowedBlock(PosBlockID))
		{
			lp[i] = m_World.GetBlockMeta(Pos->x, Pos->y, Pos->z);
			if(lp[i] == 8)
                lp[i] = 0;

		}
		else if (PosBlockID == E_BLOCK_AIR)
		{
			if(LowestPoint == 7)
				lp[i] = 8;
			else
				lp[i] = -1;
		}
		else
		{
			lp[i] = -2;
		}
		if (ct < lp[i])
        {
            ct = lp[i];
			dir_ = i;
        }
		delete Pos;
		Pos = nullptr;
		i++;
	}

	if (dir_ == -1 && LowestPoint == 0)
		return vDirection;

    ct = LowestPoint;

	if ((lp[0] == lp[1] && lp[1] == lp[2] && lp[2] == lp[3]) || (lp[0] == lp[2] && lp[1] == lp[3]))
		return vDirection;

	if (lp[0] == lp[2] || (lp[0] < 0 && lp[2] < 0))
	{
		if (lp[1] > lp[3] && lp[1] > ct)
			dir = DIR_SOUTH;
		else if (lp[3] > lp[1] && lp[3] > ct)
			dir = DIR_NORTH;
	}
	else if (lp[1] == lp[3] || (lp[1] < 0 && lp[3] < 0))
	{
		if (lp[0] > lp[2] && lp[0] > ct)
			dir = DIR_EAST;
		else if (lp[2] > lp[0] && lp[2] > ct)
			dir = DIR_WEST;
	}

	if (dir == -1)
	{
        #ifdef DEBUG_PICKUP_WATERMOVE
		LOG("dir_ = %d", dir_);
		#endif

		switch (dir_)
		{
			case DIR_EAST:
			{
				up = 0;
				rt = 1;
				dn = 2;
				lt = 3;
				break;
			}
			case DIR_SOUTH:
			{
				up = 1;
				rt = 2;
				dn = 3;
				lt = 0;
				break;
			}
			case DIR_WEST:
			{
				up = 2;
				rt = 3;
				dn = 0;
				lt = 1;
				break;
			}
			case DIR_NORTH:
			{
				up = 3;
				rt = 0;
				dn = 1;
				lt = 2;
				break;
			}
		}

		if (D_UP == D_RT)
		{
			if (D_LT >= 0)
			{
				dir = rt;
				if (D_DN >= 0)
					lrp = -1.0;
				else
					lrp = -0.5;
			}
			else if (D_DN >= 0)
			{
				dir = up;
				if (D_LT >= 0)
					lrp = 1.0;
				else
					lrp = 0.5;
			}
			else
			{
				dir = up;
				lrp = 1.0;
			}

		}
		else if (D_UP == D_LT)
		{
			if (D_RT >= 0)
			{
				dir = lt;
				if (D_DN >= 0)
					lrp = 1.0;
				else
					lrp = 0.5;
			}
			else if (D_DN >= 0)
			{
				dir = up;
				if (D_RT >= 0)
					lrp = -1.0;
				else
					lrp = -0.5;
			}
			else
			{
				dir = up;
				lrp = -1.0;
			}
		}
		else if (D_LT < 0)
		{
			dir = up;
			if (D_LT == D_DN)
				lrp = 1.0;
			else
				lrp = 0.25;
		}
		else if (D_RT < 0)
		{
			dir = up;
			if (D_RT == D_DN)
				lrp = -1.0;
			else
				lrp = -0.25;
		}
		else if (D_DN < 0 && D_RT != 0 && D_LT != 0)
		{
			dir = up;
			if (D_RT > D_LT)
			{
				lrp = 1.0;
			}
			else if (D_LT > D_RT)
			{
				lrp = -1.0;
			}
		}
		else
			dir = up;
	}

    switch (dir)
    {
        case DIR_EAST:
        {
        #ifdef DEBUG_PICKUP_WATERMOVE
			LOG("***************\n ct = %d\n lp[0] = %d\n lp[1] = %d\n lp[2] = %d\n lp[3] = %d\n lrp = %1.1f\n DIR_EAST\n", ct, lp[0], lp[1], lp[2], lp[3], lrp);
         #endif
            vDirection.x = 1.0;
            vDirection.z = lrp;
            break;
        }
        case DIR_SOUTH:
        {
        #ifdef DEBUG_PICKUP_WATERMOVE
			LOG("***************\n ct = %d\n lp[0] = %d\n lp[1] = %d\n lp[2] = %d\n lp[3] = %d\n lrp = %1.1f\n DIR_SOUTH\n", ct, lp[0], lp[1], lp[2], lp[3], lrp);
         #endif
            vDirection.x = -lrp;
			vDirection.z = 1.0;
            break;
        }
        case DIR_WEST:
        {
        #ifdef DEBUG_PICKUP_WATERMOVE
			LOG("***************\n ct = %d\n lp[0] = %d\n lp[1] = %d\n lp[2] = %d\n lp[3] = %d\n lrp = %1.1f\n DIR_WEST\n", ct, lp[0], lp[1], lp[2], lp[3], lrp);
        #endif
            vDirection.x = -1.0;
            vDirection.z = -lrp;
            break;
        }
        case DIR_NORTH:
        {
        #ifdef DEBUG_PICKUP_WATERMOVE
			LOG("***************\n ct = %d\n lp[0] = %d\n lp[1] = %d\n lp[2] = %d\n lp[3] = %d\n lrp = %1.1f\n DIR_NORTH\n", ct, lp[0], lp[1], lp[2], lp[3], lrp);
         #endif
            vDirection.x = lrp;
			vDirection.z = -1.0;
            break;
        }
        #ifdef DEBUG_PICKUP_WATERMOVE
		default:
		{
			LOG("***************\n ct = %d\n lp[0] = %d\n lp[1] = %d\n lp[2] = %d\n lp[3] = %d\n", ct, lp[0], lp[1], lp[2], lp[3]);
		}
		#endif
    }

	//vDirection = { 0.0, 0.0, 0.0 }; //For Debug
	return vDirection;
}


