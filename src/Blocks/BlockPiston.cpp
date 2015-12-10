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

////////////////////////////////////////////////////////////////////////////////
// cBlockPistonHeadHandler:

cBlockPistonHeadHandler::cBlockPistonHeadHandler(void)
	: super(E_BLOCK_PISTON_EXTENSION)
{
}




cBlockPistonHandler::cBlockPistonHandler(BLOCKTYPE a_BlockType)
	: cBlockHandler(a_BlockType)
{
}




void cBlockPistonHandler::OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	Vector3i blockPos(a_BlockX, a_BlockY, a_BlockZ);
	
	// Get the extension of the piston
	NIBBLETYPE OldMeta = a_ChunkInterface.GetBlockMeta(blockPos.x, blockPos.y, blockPos.z);
	blockPos += MetadataToOffset(OldMeta);

	if (a_ChunkInterface.GetBlock(blockPos) == E_BLOCK_PISTON_EXTENSION)
	{
		a_ChunkInterface.SetBlock(blockPos.x, blockPos.y, blockPos.z, E_BLOCK_AIR, 0);
	}
}





void cBlockPistonHandler::ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta)
{
	// Returning Piston Item without Direction-Metavalue
	a_Pickups.push_back(cItem(m_BlockType, 1));
}




bool cBlockPistonHandler::GetPlacementBlockTypeMeta(cChunkInterface  & a_ChunkInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta)
{
	a_BlockType = m_BlockType;
	a_BlockMeta = RotationPitchToMetaData(a_Player->GetYaw(), a_Player->GetPitch());
	
	return true;
}




Vector3i cBlockPistonHandler::MetadataToOffset(NIBBLETYPE a_PistonMeta)
{
	Vector3i result;

	switch (a_PistonMeta & 0x07)
	{
		case 0: result = Vector3i( 0, -1,  0); break;
		case 1: result = Vector3i( 0,  1,  0); break;
		case 2: result = Vector3i( 0,  0, -1); break;
		case 3: result = Vector3i( 0,  0,  1); break;
		case 4: result = Vector3i(-1,  0,  0); break;
		case 5: result = Vector3i( 1,  0,  0); break;
		default:
		{
			LOGWARNING("%s: invalid direction %d, ignoring", __FUNCTION__, a_PistonMeta & 0x07);
			ASSERT(!"Invalid direction");
			result = Vector3i();
		}
		
		break;
	}
	
	return result;
}




void cBlockPistonHandler::PushBlocks(const Vector3iSet & a_BlocksToPush, cWorld * a_World, const Vector3i & a_PushDir)
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
	
	for (auto & moveBlockPos: sortedBlocks)
	{
		a_World->GetBlockTypeMeta(moveBlockPos.x, moveBlockPos.y, moveBlockPos.z, moveBlock, moveMeta);
		a_World->SetBlock(moveBlockPos.x, moveBlockPos.y, moveBlockPos.z, E_BLOCK_AIR, 0);

		moveBlockPos += a_PushDir;
		if (cBlockInfo::IsPistonBreakable(moveBlock))
		{
			// Block is breakable, drop it
			cBlockHandler * Handler = BlockHandler(moveBlock);
			
			if (Handler->DoesDropOnUnsuitable())
			{
				cChunkInterface ChunkInterface(a_World->GetChunkMap());
				cBlockInServerPluginInterface PluginInterface(*a_World);
				
				Handler->DropBlock(
					ChunkInterface, *a_World,
					PluginInterface, nullptr,
					moveBlockPos.x, moveBlockPos.y, moveBlockPos.z
				);
			}
		}
		else
		{
			// Not breakable, just move it
			a_World->SetBlock(moveBlockPos.x, moveBlockPos.y, moveBlockPos.z, moveBlock, moveMeta);
		}
	}
}




bool cBlockPistonHandler::CanPushBlock(const Vector3i & a_BlockPos, cWorld * a_World, bool a_RequirePushable, Vector3iSet & a_BlocksPushed, const Vector3i & a_PushDir)
{
	bool result = false;
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
	
	a_World->GetBlockTypeMeta(
		a_BlockPos.x, a_BlockPos.y, a_BlockPos.z,
		currBlock, currMeta
	);
	
	if (currBlock == E_BLOCK_AIR)
	{
		// Air can be pushed
		result = true;
	}
	else if (!a_RequirePushable && cBlockInfo::IsPistonBreakable(currBlock))
	{
		// Block should not be broken, when it's not in the pushing direction
		result = true;
	}
	else if (!CanPush(currBlock, currMeta))
	{
		// When it's not required to push this block, simply exit.
		result = !a_RequirePushable;
	}
	else if (a_BlocksPushed.size() >= PISTON_MAX_PUSH_DISTANCE)
	{
		// Do not allow to push too much blocks
		result = false;
	}
	else if (!a_BlocksPushed.insert(a_BlockPos).second || cBlockInfo::IsPistonBreakable(currBlock))
	{
		// Element exist already
		result = true;
	}
	else
	{
		if (currBlock == E_BLOCK_SLIME_BLOCK)
		{
			bool finished = false;
			
			// Try to push the other directions
			for (const auto & testDir: pushingDirs)
			{
				if (finished)
				{
					break;
				}
				
				result = CanPushBlock(a_BlockPos + testDir, a_World, false, a_BlocksPushed, a_PushDir);
				
				if (result == false)
				{
					// If we can't push the block, we're done.
					finished = true;
				}
			}
			
			// All done here. DO PASS GO, do collect $100.
		}
		else
		{
			// Try to push the block in front of this block
			result = CanPushBlock(a_BlockPos + a_PushDir, a_World, true, a_BlocksPushed, a_PushDir);
		}
	}
	
	return result;
}




void cBlockPistonHandler::ExtendPiston(Vector3i a_BlockPos, cWorld * a_World)
{
	BLOCKTYPE pistonBlock;
	NIBBLETYPE pistonMeta;
	
	a_World->GetBlockTypeMeta(
		a_BlockPos.x, a_BlockPos.y, a_BlockPos.z,
		pistonBlock, pistonMeta
	);
	
	if (!IsExtended(pistonMeta))
	{
		Vector3i pushDir = MetadataToOffset(pistonMeta);
		Vector3iSet blocksPushed;
		
		if (CanPushBlock(a_BlockPos + pushDir, a_World, true, blocksPushed, pushDir))
		{
			a_World->BroadcastBlockAction(
				a_BlockPos.x, a_BlockPos.y, a_BlockPos.z,
				0, pistonMeta, pistonBlock
			);
			
			a_World->BroadcastSoundEffect(
				"tile.piston.out", 
				a_BlockPos.x, a_BlockPos.y, a_BlockPos.z,
				0.5f, 0.7f
			);
			
			PushBlocks(blocksPushed, a_World, pushDir);
			
			// Set the extension and the piston base correctly
			Vector3i extensionPos = a_BlockPos + pushDir;
			
			a_World->SetBlock(
				a_BlockPos.x, a_BlockPos.y, a_BlockPos.z,
				pistonBlock, pistonMeta | 0x8);
			
			a_World->SetBlock(
				extensionPos.x, extensionPos.y, extensionPos.z,
				E_BLOCK_PISTON_EXTENSION, pistonMeta | (IsSticky(pistonBlock) ? 8 : 0)
			);
		}
		else
		{
			// Can't push anything.
			BLOCKTYPE blockAtPos = a_World->GetBlock(a_BlockPos + pushDir);
			LOGD("%s: Unable to push object; unable to proceed. (blockAtPos = %d)", __FUNCTION__, blockAtPos);
		}
	}
}




void cBlockPistonHandler::RetractPiston(Vector3i a_BlockPos, cWorld * a_World)
{
	BLOCKTYPE pistonBlock, blockAtPos;
	NIBBLETYPE pistonMeta;
	
	a_World->GetBlockTypeMeta(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, pistonBlock, pistonMeta);
	Vector3i pushDir = MetadataToOffset(pistonMeta);
	
	if (IsExtended(pistonMeta))
	{
		Vector3i extensionPos = a_BlockPos + pushDir;
		blockAtPos = a_World->GetBlock(extensionPos);
		
		// Check the extension
		switch (blockAtPos)
		{
		// Remove Extension
		case E_BLOCK_PISTON_EXTENSION:
			a_World->SetBlock(extensionPos.x, extensionPos.y, extensionPos.z, E_BLOCK_AIR, 0);
			break;
		
		// This here is an air block. I think I will manually pull the block that's two blocks 
		// away towards me.
		case E_BLOCK_AIR:
			
			if (IsSticky(pistonBlock))
			{
				LOGD("%s: Sticky piston without extension- am I supposed to pull my cargo back", __FUNCTION__);
				LOGD("  -- lighth7015");
				
				a_World->SetBlock(
					extensionPos.x, extensionPos.y, extensionPos.z,
					E_BLOCK_PISTON_EXTENSION, pistonMeta | (IsSticky(pistonBlock)? 8: 0)
				);
			}
			
			break;
		
		default:
			LOGD("%s: Piston without an extension - still extending, or just in an invalid state?", __FUNCTION__);
			LOGD("%s: Expected: %d, Actual: %d", __FUNCTION__, E_BLOCK_PISTON_EXTENSION, blockAtPos);
			return;
		}
		
		a_World->SetBlock(
			a_BlockPos.x, a_BlockPos.y, a_BlockPos.z,
			pistonBlock, 
			pistonMeta & ~(8)
		);
		
		a_World->BroadcastBlockAction(
			a_BlockPos.x, a_BlockPos.y, a_BlockPos.z,
			1, pistonMeta & ~(8),
			pistonBlock
		);
		
		a_World->BroadcastSoundEffect(
			"tile.piston.in", 
			a_BlockPos.x, a_BlockPos.y, a_BlockPos.z,
			0.5f, 0.7f
		);
		
		if (IsSticky(pistonBlock))
		{
			// Get the block to pull
			a_BlockPos += pushDir * 2;
			
			// Try to "push" the pulling block in the opposite direction
			pushDir *= -1;
			Vector3iSet pushedBlocks;
			
			if (CanPushBlock(a_BlockPos, a_World, false, pushedBlocks, pushDir))
			{
				PushBlocks(pushedBlocks, a_World, pushDir);
			}
		}
	}
}




void cBlockPistonHeadHandler::OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	Vector3i blockPos(a_BlockX, a_BlockY, a_BlockZ);
	
	// Get the base of the piston
	NIBBLETYPE OldMeta = a_ChunkInterface.GetBlockMeta(blockPos.x, blockPos.y, blockPos.z);
	
	blockPos -= cBlockPistonHandler::MetadataToOffset(OldMeta);
	BLOCKTYPE Block = a_ChunkInterface.GetBlock(blockPos);
	
	if ((Block == E_BLOCK_STICKY_PISTON) || (Block == E_BLOCK_PISTON))
	{
		a_ChunkInterface.DigBlock(a_WorldInterface, blockPos.x, blockPos.y, blockPos.z);
		
		if (a_Player->IsGameModeCreative() == false)
		{
			cItems Pickups;
			Pickups.push_back(cItem(Block, 1));
			
			a_WorldInterface.SpawnItemPickups(
				Pickups, blockPos.x + 0.5,
				blockPos.y + 0.5, blockPos.z + 0.5
			);
		}
	}
}
