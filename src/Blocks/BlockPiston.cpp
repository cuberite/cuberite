
#include "Globals.h"
#include "BlockPiston.h"
#include "../Item.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "BlockInServerPluginInterface.h"
#include "ChunkInterface.h"





#define PISTON_MAX_PUSH_DISTANCE 12




cBlockPistonHandler::cBlockPistonHandler(BLOCKTYPE a_BlockType)
	: cBlockHandler(a_BlockType)
{
}





void cBlockPistonHandler::OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	Vector3i blockPos(a_BlockX, a_BlockY, a_BlockZ);

	NIBBLETYPE OldMeta = a_ChunkInterface.GetBlockMeta(blockPos);
	// If the piston is extended, destroy the extension as well
	if (IsExtended(OldMeta))
	{
		// Get the position of the extension
		blockPos += MetadataToOffset(OldMeta);

		if (a_ChunkInterface.GetBlock(blockPos) == E_BLOCK_PISTON_EXTENSION)
		{
			a_ChunkInterface.SetBlock(blockPos.x, blockPos.y, blockPos.z, E_BLOCK_AIR, 0);
		}
	}
}





void cBlockPistonHandler::ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta)
{
	// Returning Piston Item without Direction-Metavalue
	a_Pickups.push_back(cItem(m_BlockType, 1));
}





bool cBlockPistonHandler::GetPlacementBlockTypeMeta(
	cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
	int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
	int a_CursorX, int a_CursorY, int a_CursorZ,
	BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
)
{
	a_BlockType = m_BlockType;
	a_BlockMeta = RotationPitchToMetaData(a_Player.GetYaw(), a_Player.GetPitch());
	return true;
}





Vector3i cBlockPistonHandler::MetadataToOffset(NIBBLETYPE a_PistonMeta)
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
			ASSERT(!"Invalid direction");
			return Vector3i();
		}
	}
}





void cBlockPistonHandler::PushBlocks(
	const Vector3iSet & a_BlocksToPush,
	cWorld & a_World, const Vector3i & a_PushDir
)
{
	// Sort blocks to move the blocks first, which are farest away from the piston
	// This prevents the overwriting of existing blocks
	std::vector<Vector3i> sortedBlocks(a_BlocksToPush.begin(), a_BlocksToPush.end());
	std::sort(sortedBlocks.begin(), sortedBlocks.end(), [a_PushDir](const Vector3i & a, const Vector3i & b)
	{
		return a.Dot(a_PushDir) > b.Dot(a_PushDir);
	});

	// Move every block
	BLOCKTYPE moveBlock;
	NIBBLETYPE moveMeta;
	for (auto & moveBlockPos : sortedBlocks)
	{
		a_World.GetBlockTypeMeta(moveBlockPos.x, moveBlockPos.y, moveBlockPos.z, moveBlock, moveMeta);

		if (cBlockInfo::IsPistonBreakable(moveBlock))
		{
			// Block is breakable, drop it
			cBlockHandler * Handler = BlockHandler(moveBlock);
			if (Handler->DoesDropOnUnsuitable())
			{
				cChunkInterface ChunkInterface(a_World.GetChunkMap());
				cBlockInServerPluginInterface PluginInterface(a_World);
				Handler->DropBlock(ChunkInterface, a_World, PluginInterface, nullptr,
					moveBlockPos.x, moveBlockPos.y, moveBlockPos.z
				);
			}
			a_World.SetBlock(moveBlockPos.x, moveBlockPos.y, moveBlockPos.z, E_BLOCK_AIR, 0);
		}
		else
		{
			// Not breakable, just move it
			a_World.SetBlock(moveBlockPos.x, moveBlockPos.y, moveBlockPos.z, E_BLOCK_AIR, 0);
			moveBlockPos += a_PushDir;
			a_World.SetBlock(moveBlockPos.x, moveBlockPos.y, moveBlockPos.z, moveBlock, moveMeta);
		}
	}
}





bool cBlockPistonHandler::CanPushBlock(
	const Vector3i & a_BlockPos, cWorld & a_World, bool a_RequirePushable,
	Vector3iSet & a_BlocksPushed, const Vector3i & a_PushDir
)
{
	const static std::array<Vector3i, 6> pushingDirs =
	{
		{
			Vector3i(-1,  0,  0), Vector3i(1, 0, 0),
			Vector3i( 0, -1,  0), Vector3i(0, 1, 0),
			Vector3i( 0,  0, -1), Vector3i(0, 0, 1)
		}
	};

	BLOCKTYPE currBlock;
	NIBBLETYPE currMeta;
	a_World.GetBlockTypeMeta(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, currBlock, currMeta);

	if (currBlock == E_BLOCK_AIR)
	{
		// Air can be pushed
		return true;
	}

	if (!a_RequirePushable && cBlockInfo::IsPistonBreakable(currBlock))
	{
		// Block should not be broken, when it's not in the pushing direction
		return true;
	}

	if (!CanPush(currBlock, currMeta))
	{
		// When it's not required to push this block, don't fail
		return !a_RequirePushable;
	}

	if (a_BlocksPushed.size() >= PISTON_MAX_PUSH_DISTANCE)
	{
		// Do not allow to push too much blocks
		return false;
	}

	if (!a_BlocksPushed.insert(a_BlockPos).second || cBlockInfo::IsPistonBreakable(currBlock))
	{
		return true;  // Element exist already
	}

	if (currBlock == E_BLOCK_SLIME_BLOCK)
	{
		// Try to push the other directions
		for (const auto & testDir : pushingDirs)
		{
			if (!CanPushBlock(a_BlockPos + testDir, a_World, false, a_BlocksPushed, a_PushDir))
			{
				// When it's not possible for a direction, then fail
				return false;
			}
		}
	}

	// Try to push the block in front of this block
	return CanPushBlock(a_BlockPos + a_PushDir, a_World, true, a_BlocksPushed, a_PushDir);
}





void cBlockPistonHandler::ExtendPiston(Vector3i a_BlockPos, cWorld & a_World)
{
	BLOCKTYPE pistonBlock;
	NIBBLETYPE pistonMeta;
	a_World.GetBlockTypeMeta(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, pistonBlock, pistonMeta);

	if (IsExtended(pistonMeta))
	{
		// Already extended, bail out
		return;
	}

	Vector3i pushDir = MetadataToOffset(pistonMeta);

	Vector3iSet blocksPushed;
	if (!CanPushBlock(a_BlockPos + pushDir, a_World, true, blocksPushed, pushDir))
	{
		// Can't push anything, bail out
		return;
	}

	a_World.BroadcastBlockAction(a_BlockPos, 0, pistonMeta, pistonBlock);
	a_World.BroadcastSoundEffect("block.piston.extend", a_BlockPos, 0.5f, 0.7f);

	PushBlocks(blocksPushed, a_World, pushDir);

	// Set the extension and the piston base correctly
	Vector3i extensionPos = a_BlockPos + pushDir;
	a_World.SetBlock(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, pistonBlock, pistonMeta | 0x8);
	a_World.SetBlock(
		extensionPos.x, extensionPos.y, extensionPos.z,
		E_BLOCK_PISTON_EXTENSION, pistonMeta | (IsSticky(pistonBlock) ? 8 : 0)
	);
}





void cBlockPistonHandler::RetractPiston(Vector3i a_BlockPos, cWorld & a_World)
{
	BLOCKTYPE pistonBlock;
	NIBBLETYPE pistonMeta;
	a_World.GetBlockTypeMeta(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, pistonBlock, pistonMeta);

	if (!IsExtended(pistonMeta))
	{
		// Already retracted, bail out
		return;
	}

	Vector3i pushDir = MetadataToOffset(pistonMeta);

	// Check the extension:
	Vector3i extensionPos = a_BlockPos + pushDir;
	if (a_World.GetBlock(extensionPos) != E_BLOCK_PISTON_EXTENSION)
	{
		LOGD("%s: Piston without an extension - still extending, or just in an invalid state?", __FUNCTION__);
		return;
	}

	// Remove Extension
	a_World.SetBlock(extensionPos.x, extensionPos.y, extensionPos.z, E_BLOCK_AIR, 0);

	a_World.SetBlock(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, pistonBlock, pistonMeta & ~(8));
	a_World.BroadcastBlockAction(a_BlockPos, 1, pistonMeta & ~(8), pistonBlock);
	a_World.BroadcastSoundEffect("block.piston.contract", a_BlockPos, 0.5f, 0.7f);

	if (!IsSticky(pistonBlock))
	{
		// No need for block pulling, bail out
		return;
	}

	// Get the block to pull
	a_BlockPos += pushDir * 2;
	// Try to "push" the pulling block in the opposite direction
	pushDir *= -1;

	Vector3iSet pushedBlocks;
	if (!CanPushBlock(a_BlockPos, a_World, false, pushedBlocks, pushDir))
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





void cBlockPistonHeadHandler::OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	Vector3i blockPos(a_BlockX, a_BlockY, a_BlockZ);

	// Get the base of the piston
	NIBBLETYPE OldMeta = a_ChunkInterface.GetBlockMeta(blockPos);
	blockPos -= cBlockPistonHandler::MetadataToOffset(OldMeta);

	BLOCKTYPE Block = a_ChunkInterface.GetBlock(blockPos);
	if ((Block == E_BLOCK_STICKY_PISTON) || (Block == E_BLOCK_PISTON))
	{
		a_ChunkInterface.DigBlock(a_WorldInterface, blockPos.x, blockPos.y, blockPos.z);
		if (a_Player.IsGameModeCreative())
		{
			return;  // No pickups if creative
		}

		cItems Pickups;
		Pickups.push_back(cItem(Block, 1));
		a_WorldInterface.SpawnItemPickups(Pickups, blockPos.x + 0.5, blockPos.y + 0.5, blockPos.z + 0.5);
	}
}





