
#include "Globals.h"
#include "BlockPiston.h"
#include "../Item.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "BlockInServerPluginInterface.h"
#include "ChunkInterface.h"

#include <vector>
#include <array>





#define PISTON_MAX_PUSH_DISTANCE 12




cBlockPistonHandler::cBlockPistonHandler(BLOCKTYPE a_BlockType)
	: cBlockHandler(a_BlockType)
{
}





void cBlockPistonHandler::OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	NIBBLETYPE OldMeta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

	const Vector3i pushDir = GetDirectionVec(OldMeta);
	int newX = a_BlockX + pushDir.x;
	int newY = a_BlockY + pushDir.y;
	int newZ = a_BlockZ + pushDir.z;

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





Vector3i cBlockPistonHandler::GetDirectionVec(int a_PistonMeta)
{
	switch (a_PistonMeta & 0x07)
	{
		case 0: return Vector3i( 0, -1,  0);
		case 1: return Vector3i( 0,  1,  0);
		case 2: return Vector3i( 0,  0, -1);
		case 3: return Vector3i( 0,  0,  1);
		case 4: return Vector3i(-1,  0,  0);
		case 5: return Vector3i( 1,  0,  0);
		default:
		{
			LOGWARNING("%s: invalid direction %d, ignoring", __FUNCTION__, a_PistonMeta & 0x07);
			return Vector3i();
		}
	}
}





void cBlockPistonHandler::PushBlocks(const std::unordered_set<Vector3i, VectorHasher<int>> & a_BlocksToPush,
	cWorld * a_World, const Vector3i & a_PushDir
)
{
	std::vector<Vector3i> sortedBlocks(a_BlocksToPush.begin(), a_BlocksToPush.end());
	std::sort(sortedBlocks.begin(), sortedBlocks.end(), [a_PushDir](const Vector3i & a, const Vector3i & b)
	{
		return a.Dot(a_PushDir) > b.Dot(a_PushDir);
	});

	BLOCKTYPE moveBlock;
	NIBBLETYPE moveMeta;
	for (Vector3i & moveBlockPos : sortedBlocks)
	{	
		a_World->GetBlockTypeMeta(moveBlockPos.x, moveBlockPos.y, moveBlockPos.z, moveBlock, moveMeta);
		a_World->SetBlock(moveBlockPos.x, moveBlockPos.y, moveBlockPos.z, E_BLOCK_AIR, 0);

		moveBlockPos += a_PushDir;
		if (cBlockInfo::IsPistonBreakable(moveBlock))
		{
			cBlockHandler * Handler = BlockHandler(moveBlock);
			if (Handler->DoesDropOnUnsuitable())
			{
				cChunkInterface ChunkInterface(a_World->GetChunkMap());
				cBlockInServerPluginInterface PluginInterface(*a_World);
				Handler->DropBlock(ChunkInterface, *a_World, PluginInterface, nullptr,
					moveBlockPos.x, moveBlockPos.y, moveBlockPos.z
				);
			}
		} else
		{
			a_World->SetBlock(moveBlockPos.x, moveBlockPos.y, moveBlockPos.z, moveBlock, moveMeta);
		}
	}
}





bool cBlockPistonHandler::CanPushBlock(
	int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World, bool a_RequirePushable,
	std::unordered_set<Vector3i, VectorHasher<int>> & a_BlocksPushed, const Vector3i & a_PushDir
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
			if(!CanPushBlock(a_BlockX + testDir.x, a_BlockY + testDir.y, a_BlockZ + testDir.z, a_World, false, a_BlocksPushed, a_PushDir))
			{
				return false;
			}
		}
	}

	return CanPushBlock(a_BlockX + a_PushDir.x, a_BlockY + a_PushDir.y, a_BlockZ + a_PushDir.z, a_World, true, a_BlocksPushed, a_PushDir);
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

	Vector3i pushDir = GetDirectionVec(pistonMeta);

	std::unordered_set<Vector3i, VectorHasher<int>> blocksPushed;
	if (!CanPushBlock(a_BlockX + pushDir.x, a_BlockY + pushDir.y, a_BlockZ + pushDir.z,
		a_World, true, blocksPushed, pushDir)
	)
	{
		// Can't push anything, bail out
		return;
	}

	a_World->BroadcastBlockAction(a_BlockX, a_BlockY, a_BlockZ, 0, pistonMeta, pistonBlock);
	a_World->BroadcastSoundEffect("tile.piston.out", static_cast<double>(a_BlockX), static_cast<double>(a_BlockY), static_cast<double>(a_BlockZ), 0.5f, 0.7f);

	PushBlocks(blocksPushed, a_World, pushDir);

	a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, pistonBlock, pistonMeta | 0x8);
	a_World->SetBlock(a_BlockX + pushDir.x, a_BlockY + pushDir.y, a_BlockZ + pushDir.z,
		E_BLOCK_PISTON_EXTENSION, pistonMeta | (IsSticky(pistonBlock) ? 8 : 0)
	);
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

	Vector3i pushDir = GetDirectionVec(pistonMeta);

	// Check the extension:
	if (a_World->GetBlock(a_BlockX + pushDir.x, a_BlockY + pushDir.y, a_BlockZ + pushDir.z) != E_BLOCK_PISTON_EXTENSION)
	{
		LOGD("%s: Piston without an extension - still extending, or just in an invalid state?", __FUNCTION__);
		return;
	}

	// Remove Extension
	a_World->SetBlock(a_BlockX + pushDir.x, a_BlockY + pushDir.y, a_BlockZ + pushDir.z, E_BLOCK_AIR, 0);

	a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, pistonBlock, pistonMeta & ~(8));
	a_World->BroadcastBlockAction(a_BlockX, a_BlockY, a_BlockZ, 1, pistonMeta & ~(8), pistonBlock);
	a_World->BroadcastSoundEffect("tile.piston.in", static_cast<double>(a_BlockX), static_cast<double>(a_BlockY), static_cast<double>(a_BlockZ), 0.5f, 0.7f);

	if(!IsSticky(pistonBlock))
	{
		// No need for block pulling, bail out
		return;
	}

	a_BlockX += 2 * pushDir.x;
	a_BlockY += 2 * pushDir.y;
	a_BlockZ += 2 * pushDir.z;
	// Try to "push" the pulling block in the opposite direction
	pushDir *= -1;

	std::unordered_set<Vector3i, VectorHasher<int>> pushedBlocks;
	if (!CanPushBlock(a_BlockX, a_BlockY, a_BlockZ, a_World, false, pushedBlocks, pushDir))
	{
		// Not pushable, bail out
		return;
	}

	PushBlocks(pushedBlocks, a_World, pushDir);
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

	Vector3i pushDir = cBlockPistonHandler::GetDirectionVec(OldMeta);
	int newX = a_BlockX - pushDir.x;
	int newY = a_BlockY - pushDir.y;
	int newZ = a_BlockZ - pushDir.z;

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
