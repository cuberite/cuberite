
#include "Globals.h"
#include "BlockPiston.h"
#include "../Item.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "BlockInServerPluginInterface.h"





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

#define PISTON_TICK_DELAY 1
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
		a_ChunkInterface.SetBlock(a_WorldInterface, newX, newY, newZ, E_BLOCK_AIR, 0);
	}
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





int cBlockPistonHandler::FirstPassthroughBlock(int a_PistonX, int a_PistonY, int a_PistonZ, NIBBLETYPE pistonmeta, cWorld * a_World)
{
	// Examine each of the 12 blocks ahead of the piston:
	for (int ret = 0; ret < PISTON_MAX_PUSH_DISTANCE; ret++)
	{
		BLOCKTYPE currBlock;
		NIBBLETYPE currMeta;
		AddPistonDir(a_PistonX, a_PistonY, a_PistonZ, pistonmeta, 1);
		a_World->GetBlockTypeMeta(a_PistonX, a_PistonY, a_PistonZ, currBlock, currMeta);
		if (cBlockInfo::IsPistonBreakable(currBlock))
		{
			// This block breaks when pushed, extend up to here
			return ret;
		}
		if (!CanPush(currBlock, currMeta))
		{
			// This block cannot be pushed at all, the piston can't extend
			return -1;
		}
	}
	// There is no space for the blocks to move, piston can't extend
	return -1;
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

	int dist = FirstPassthroughBlock(a_BlockX, a_BlockY, a_BlockZ, pistonMeta, a_World);
	if (dist < 0)
	{
		// FirstPassthroughBlock says piston can't push anything, bail out
		return;
	}

	a_World->BroadcastBlockAction(a_BlockX, a_BlockY, a_BlockZ, 0, pistonMeta, pistonBlock);
	a_World->BroadcastSoundEffect("tile.piston.out", (double)a_BlockX, (double)a_BlockY, (double)a_BlockZ, 0.5f, 0.7f);

	// Drop the breakable block in the line, if appropriate:
	AddPistonDir(a_BlockX, a_BlockY, a_BlockZ, pistonMeta, dist + 1);  // "a_Block" now at the breakable / empty block
	BLOCKTYPE currBlock;
	NIBBLETYPE currMeta;
	a_World->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, currBlock, currMeta);
	if (currBlock != E_BLOCK_AIR)
	{
		cBlockHandler * Handler = BlockHandler(currBlock);
		if (Handler->DoesDropOnUnsuitable())
		{
			cChunkInterface ChunkInterface(a_World->GetChunkMap());
			cBlockInServerPluginInterface PluginInterface(*a_World);
			Handler->DropBlock(ChunkInterface, *a_World, PluginInterface, NULL, a_BlockX, a_BlockY, a_BlockZ);
		}
	}

	// Push blocks, from the furthest to the nearest:
	int oldx = a_BlockX, oldy = a_BlockY, oldz = a_BlockZ;
	NIBBLETYPE currBlockMeta;
	std::vector<Vector3i> ScheduledBlocks;
	ScheduledBlocks.reserve(PISTON_MAX_PUSH_DISTANCE);

	for (int i = dist + 1; i > 1; i--)
	{
		AddPistonDir(a_BlockX, a_BlockY, a_BlockZ, pistonMeta, -1);
		a_World->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, currBlock, currBlockMeta);
		a_World->SetBlock(oldx, oldy, oldz, currBlock, currBlockMeta, false);
		ScheduledBlocks.push_back(Vector3i(oldx, oldy, oldz));
		oldx = a_BlockX;
		oldy = a_BlockY;
		oldz = a_BlockZ;
	}

	int extx = a_BlockX;
	int exty = a_BlockY;
	int extz = a_BlockZ;
	ScheduledBlocks.push_back(Vector3i(extx, exty, extz));
	AddPistonDir(a_BlockX, a_BlockY, a_BlockZ, pistonMeta, -1);
	// "a_Block" now at piston body, "ext" at future extension

	a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, pistonBlock, pistonMeta | 0x8);
	a_World->SetBlock(extx, exty, extz, E_BLOCK_PISTON_EXTENSION, pistonMeta | (IsSticky(pistonBlock) ? 8 : 0), false);
	a_World->ScheduleTask(PISTON_TICK_DELAY, new cWorld::cTaskSendBlockToAllPlayers(ScheduledBlocks));
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

	AddPistonDir(a_BlockX, a_BlockY, a_BlockZ, pistonMeta, -1);
	a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, pistonBlock, pistonMeta & ~(8));
	a_World->BroadcastBlockAction(a_BlockX, a_BlockY, a_BlockZ, 1, pistonMeta & ~(8), pistonBlock);
	a_World->BroadcastSoundEffect("tile.piston.in", (double)a_BlockX, (double)a_BlockY, (double)a_BlockZ, 0.5f, 0.7f);
	AddPistonDir(a_BlockX, a_BlockY, a_BlockZ, pistonMeta, 1);

	// Retract the extension, pull block if appropriate
	if (IsSticky(pistonBlock))
	{
		int tempx = a_BlockX, tempy = a_BlockY, tempz = a_BlockZ;
		AddPistonDir(tempx, tempy, tempz, pistonMeta, 1);
		BLOCKTYPE tempBlock;
		NIBBLETYPE tempMeta;
		a_World->GetBlockTypeMeta(tempx, tempy, tempz, tempBlock, tempMeta);
		if (CanPull(tempBlock, tempMeta))
		{
			// Pull the block
			a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, tempBlock, tempMeta, false);
			a_World->SetBlock(tempx, tempy, tempz, E_BLOCK_AIR, 0, false);

			std::vector<Vector3i> ScheduledBlocks;
			ScheduledBlocks.push_back(Vector3i(a_BlockX, a_BlockY, a_BlockZ));
			ScheduledBlocks.push_back(Vector3i(tempx, tempy, tempz));
			a_World->ScheduleTask(PISTON_TICK_DELAY + 1, new cWorld::cTaskSendBlockToAllPlayers(ScheduledBlocks));
			return;
		}
	}
	
	// Retract without pulling
	a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0, false);

	std::vector<Vector3i> ScheduledBlocks;
	ScheduledBlocks.push_back(Vector3i(a_BlockX, a_BlockY, a_BlockZ));
	a_World->ScheduleTask(PISTON_TICK_DELAY + 1, new cWorld::cTaskSendBlockToAllPlayers(ScheduledBlocks));
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





