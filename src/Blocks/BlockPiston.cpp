
#include "Globals.h"
#include "BlockPiston.h"
#include "../Item.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "BlockInServerPluginInterface.h"
#include "ChunkInterface.h"

#include <vector>
#include <array>





#define AddPistonDir(x, y, z, dir, amount) \
	switch (dir & 0x07) \
	{ \
		case 0: (y) -= (amount); break; \
		case 1: (y) += (amount); break; \
		case 2: (z) -= (amount); break; \
		case 3: (z) += (amount); break; \
		case 4: (x) -= (amount); break; \
		case 5: (x) += (amount); break; \
		default: \
		{ \
			LOGWARNING("%s: invalid direction %d, ignoring", __FUNCTION__, dir & 0x07); \
			break; \
		} \
	}

#define PISTON_MAX_PUSH_DISTANCE 12




cBlockPistonHandler::cBlockPistonHandler(BLOCKTYPE a_BlockType)
	: cBlockHandler(a_BlockType)
{
}





void cBlockPistonHandler::OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	NIBBLETYPE OldMeta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

	int newX = a_BlockX;
	int newY = a_BlockY;
	int newZ = a_BlockZ;
	AddPistonDir(newX, newY, newZ, OldMeta, 1);

	if (a_ChunkInterface.GetBlock(newX, newY, newZ) == E_BLOCK_PISTON_EXTENSION)
	{
		a_ChunkInterface.SetBlock(newX, newY, newZ, E_BLOCK_AIR, 0);
	}
}





void cBlockPistonHandler::ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta)
{
	// Returning Piston Item without Direction-Metavalue
	a_Pickups.push_back(cItem(m_BlockType, 1));
}





bool cBlockPistonHandler::GetPlacementBlockTypeMeta(
	cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
	int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
	int a_CursorX, int a_CursorY, int a_CursorZ,
	BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
)
{
	a_BlockType = m_BlockType;
	a_BlockMeta = RotationPitchToMetaData(a_Player->GetYaw(), a_Player->GetPitch());
	return true;
}





bool cBlockPistonHandler::CanPushBlock(
	int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World, bool a_RequirePushable,
	std::unordered_set<Vector3i, VectorHasher<int>> & a_BlocksPushed, NIBBLETYPE a_PistonMeta
)
{
	const static std::array<Vector3i, 6> pushingDirs = {{ Vector3i(-1, 0, 0), Vector3i(1, 0, 0), Vector3i(0, -1, 0), Vector3i(0, 1, 0),
		Vector3i(0, 0, -1), Vector3i(0, 0, 1) }};

	BLOCKTYPE currBlock;
	NIBBLETYPE currMeta;
	a_World->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, currBlock, currMeta);

	if (currBlock == E_BLOCK_AIR)
	{
		// Air can be pushed
		return true;
	}

	if(!a_RequirePushable && cBlockInfo::IsPistonBreakable(currBlock))
	{
		// Block should not be broken, when it's not in the pushing direction
		return true;
	}

	if (!CanPush(currBlock, currMeta))
	{
		return !a_RequirePushable;
	}

	if (a_BlocksPushed.size() >= PISTON_MAX_PUSH_DISTANCE)
	{
		return false;
	}

	if (!a_BlocksPushed.emplace(a_BlockX, a_BlockY, a_BlockZ).second || cBlockInfo::IsPistonBreakable(currBlock))
	{
		return true;  // Element exist already
	}

	if(currBlock == E_BLOCK_SLIME_BLOCK)
	{
		// Try to push the other directions
		for(const Vector3i & testDir : pushingDirs)
		{
			if(!CanPushBlock(a_BlockX + testDir.x, a_BlockY + testDir.y, a_BlockZ + testDir.z, a_World, false, a_BlocksPushed, a_PistonMeta))
			{
				return false;
			}
		}
	}

	AddPistonDir(a_BlockX, a_BlockY, a_BlockZ, a_PistonMeta, 1);
	return CanPushBlock(a_BlockX, a_BlockY, a_BlockZ, a_World, true, a_BlocksPushed, a_PistonMeta);
}





void cBlockPistonHandler::ExtendPiston(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World)
{
	BLOCKTYPE pistonBlock;
	NIBBLETYPE pistonMeta;
	a_World->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, pistonBlock, pistonMeta);

	if (IsExtended(pistonMeta))
	{
		// Already extended, bail out
		return;
	}

	int moveX = a_BlockX;
	int moveY = a_BlockY;
	int moveZ = a_BlockZ;

	AddPistonDir(moveX, moveY, moveZ, pistonMeta, 1);
	std::unordered_set<Vector3i, VectorHasher<int>> blocksPushed;
	if (!CanPushBlock(moveX, moveY, moveZ, a_World, true, blocksPushed, pistonMeta))
	{
		// Can't push anything, bail out
		return;
	}

	Vector3i pistonMoveVec;
	AddPistonDir(pistonMoveVec.x, pistonMoveVec.y, pistonMoveVec.z, pistonMeta, 1);
	std::vector<Vector3i> sortedBlocks(blocksPushed.begin(), blocksPushed.end());
	std::sort(sortedBlocks.begin(), sortedBlocks.end(), [pistonMoveVec](const Vector3i & a, const Vector3i & b)
	{
		return a.Dot(pistonMoveVec) > b.Dot(pistonMoveVec);
	});

	a_World->BroadcastBlockAction(a_BlockX, a_BlockY, a_BlockZ, 0, pistonMeta, pistonBlock);
	a_World->BroadcastSoundEffect("tile.piston.out", static_cast<double>(a_BlockX), static_cast<double>(a_BlockY), static_cast<double>(a_BlockZ), 0.5f, 0.7f);

	BLOCKTYPE moveBlock;
	NIBBLETYPE moveMeta;
	for (const Vector3i & moveBlockVec : sortedBlocks)
	{
		moveX = moveBlockVec.x;
		moveY = moveBlockVec.y;
		moveZ = moveBlockVec.z;
		a_World->GetBlockTypeMeta(moveX, moveY, moveZ, moveBlock, moveMeta);
		a_World->SetBlock(moveX, moveY, moveZ, E_BLOCK_AIR, 0);

		AddPistonDir(moveX, moveY, moveZ, pistonMeta, 1);

		if (cBlockInfo::IsPistonBreakable(moveBlock))
		{
			cBlockHandler * Handler = BlockHandler(moveBlock);
			if (Handler->DoesDropOnUnsuitable())
			{
				cChunkInterface ChunkInterface(a_World->GetChunkMap());
				cBlockInServerPluginInterface PluginInterface(*a_World);
				Handler->DropBlock(ChunkInterface, *a_World, PluginInterface, nullptr, moveX, moveY, moveZ);
			}
		} else
		{
			a_World->SetBlock(moveX, moveY, moveZ, moveBlock, moveMeta);
		}
	}

	a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, pistonBlock, pistonMeta | 0x8);
	AddPistonDir(a_BlockX, a_BlockY, a_BlockZ, pistonMeta, 1);
	a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_PISTON_EXTENSION, pistonMeta | (IsSticky(pistonBlock) ? 8 : 0));
}





void cBlockPistonHandler::RetractPiston(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World)
{
	BLOCKTYPE pistonBlock;
	NIBBLETYPE pistonMeta;
	a_World->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, pistonBlock, pistonMeta);

	if (!IsExtended(pistonMeta))
	{
		// Already retracted, bail out
		return;
	}

	// Check the extension:
	AddPistonDir(a_BlockX, a_BlockY, a_BlockZ, pistonMeta, 1);
	if (a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) != E_BLOCK_PISTON_EXTENSION)
	{
		LOGD("%s: Piston without an extension - still extending, or just in an invalid state?", __FUNCTION__);
		return;
	}

	// Remove Extension
	a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);

	AddPistonDir(a_BlockX, a_BlockY, a_BlockZ, pistonMeta, -1);
	a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, pistonBlock, pistonMeta & ~(8));
	a_World->BroadcastBlockAction(a_BlockX, a_BlockY, a_BlockZ, 1, pistonMeta & ~(8), pistonBlock);
	a_World->BroadcastSoundEffect("tile.piston.in", static_cast<double>(a_BlockX), static_cast<double>(a_BlockY), static_cast<double>(a_BlockZ), 0.5f, 0.7f);

	if(!IsSticky(pistonBlock))
	{
		// No need for block pulling, bail out
		return;
	}

	AddPistonDir(a_BlockX, a_BlockY, a_BlockZ, pistonMeta, 2);
	// Try to "push" the pulling block in the opposite direction
	switch(pistonMeta & 0x07)
	{
		case 0:
		case 1:
			pistonMeta = 1 - pistonMeta;
			break;
		case 2:
		case 3:
			pistonMeta = 5 - pistonMeta;
			break;

		case 4:
		case 5:
			pistonMeta = 9 - pistonMeta;
			break;

		default:
			{
				LOGWARNING("%s: invalid direction %d, ignoring", __FUNCTION__, pistonMeta & 0x07); \
				break;
			}
	}

	std::unordered_set<Vector3i, VectorHasher<int>> pushedBlocks;
	if (!CanPushBlock(a_BlockX, a_BlockY, a_BlockZ, a_World, false, pushedBlocks, pistonMeta))
	{
		// Not pushable, bail out
		return;
	}

	Vector3i pistonMoveVec;
	AddPistonDir(pistonMoveVec.x, pistonMoveVec.y, pistonMoveVec.z, pistonMeta, 1);
	std::vector<Vector3i> sortedBlocks(pushedBlocks.begin(), pushedBlocks.end());
	std::sort(sortedBlocks.begin(), sortedBlocks.end(), [pistonMoveVec](const Vector3i & a, const Vector3i & b)
	{
		return a.Dot(pistonMoveVec) > b.Dot(pistonMoveVec);
	});

	int moveX, moveY, moveZ;
	BLOCKTYPE moveBlock;
	NIBBLETYPE moveMeta;
	for (const Vector3i & moveBlockVec : sortedBlocks)
	{
		moveX = moveBlockVec.x;
		moveY = moveBlockVec.y;
		moveZ = moveBlockVec.z;
		a_World->GetBlockTypeMeta(moveX, moveY, moveZ, moveBlock, moveMeta);
		a_World->SetBlock(moveX, moveY, moveZ, E_BLOCK_AIR, 0);

		AddPistonDir(moveX, moveY, moveZ, pistonMeta, 1);

		if (cBlockInfo::IsPistonBreakable(moveBlock))
		{
			cBlockHandler * Handler = BlockHandler(moveBlock);
			if (Handler->DoesDropOnUnsuitable())
			{
				cChunkInterface ChunkInterface(a_World->GetChunkMap());
				cBlockInServerPluginInterface PluginInterface(*a_World);
				Handler->DropBlock(ChunkInterface, *a_World, PluginInterface, nullptr, moveX, moveY, moveZ);
			}
		} else
		{
			a_World->SetBlock(moveX, moveY, moveZ, moveBlock, moveMeta);
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// cBlockPistonHeadHandler:

cBlockPistonHeadHandler::cBlockPistonHeadHandler(void) :
	super(E_BLOCK_PISTON_EXTENSION)
{
}





void cBlockPistonHeadHandler::OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	NIBBLETYPE OldMeta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

	int newX = a_BlockX;
	int newY = a_BlockY;
	int newZ = a_BlockZ;
	AddPistonDir(newX, newY, newZ, OldMeta, -1);

	BLOCKTYPE Block = a_ChunkInterface.GetBlock(newX, newY, newZ);
	if ((Block == E_BLOCK_STICKY_PISTON) || (Block == E_BLOCK_PISTON))
	{
		a_ChunkInterface.DigBlock(a_WorldInterface, newX, newY, newZ);
		if (a_Player->IsGameModeCreative())
		{
			return;  // No pickups if creative
		}

		cItems Pickups;
		Pickups.push_back(cItem(Block, 1));
		a_WorldInterface.SpawnItemPickups(Pickups, a_BlockX + 0.5, a_BlockY + 0.5, a_BlockZ + 0.5);
	}
}
