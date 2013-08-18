#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Piston.h"
#include "ChunkDef.h"
#include "Pickup.h"
#include "Item.h"
#include "Root.h"
#include "ClientHandle.h"
#include "World.h"
#include "BlockID.h"
#include "Server.h"
#include "Blocks/BlockHandler.h"





extern bool g_BlockPistonBreakable[];





cPiston::cPiston(cWorld * a_World)
	: m_World(a_World)
{

}





int cPiston::FirstPassthroughBlock(int pistonX, int pistonY, int pistonZ, NIBBLETYPE pistonmeta)
{
	// Examine each of the 12 blocks ahead of the piston:
	for (int ret = 0; ret < 12; ret++)
	{
		BLOCKTYPE currBlock;
		NIBBLETYPE currMeta;
		AddDir(pistonX, pistonY, pistonZ, pistonmeta, 1);
		m_World->GetBlockTypeMeta(pistonX, pistonY, pistonZ, currBlock, currMeta);
		if (CanBreakPush(currBlock, currMeta))
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





void cPiston::ExtendPiston(int pistx, int pisty, int pistz)
{
	BLOCKTYPE pistonBlock;
	NIBBLETYPE pistonMeta;
	m_World->GetBlockTypeMeta(pistx, pisty, pistz, pistonBlock, pistonMeta);
	
	if (IsExtended(pistonMeta))
	{
		// Already extended, bail out
		return;
	}
	
	int dist = FirstPassthroughBlock(pistx, pisty, pistz, pistonMeta);
	if (dist < 0)
	{
		// FirstPassthroughBlock says piston can't push anything, bail out
		return;
	}

	// Drop the breakable block in the line, if appropriate:
	AddDir(pistx, pisty, pistz, pistonMeta, dist + 1);  // "pist" now at the breakable / empty block
	BLOCKTYPE currBlock;
	NIBBLETYPE currMeta;
	m_World->GetBlockTypeMeta(pistx, pisty, pistz, currBlock, currMeta);
	if (currBlock != E_BLOCK_AIR)
	{
		cBlockHandler * Handler = BlockHandler(currBlock);
		if (Handler->DoesDropOnUnsuitable())
		{
			Handler->DropBlock(m_World, NULL, pistx, pisty, pistz);
		}
	}
	
	// Push blocks, from the furthest to the nearest:
	int oldx = pistx, oldy = pisty, oldz = pistz;
	NIBBLETYPE currBlockMeta;
	for (int i = dist + 1; i > 1; i--)
	{
		AddDir(pistx, pisty, pistz, pistonMeta, -1);
		m_World->GetBlockTypeMeta(pistx, pisty, pistz, currBlock, currBlockMeta);
		m_World->SetServerBlock( oldx, oldy, oldz, currBlock, currBlockMeta);
		oldx = pistx;
		oldy = pisty;
		oldz = pistz;
	}

	int extx = pistx;
	int exty = pisty;
	int extz = pistz;
	AddDir(pistx, pisty, pistz, pistonMeta, -1);
	// "pist" now at piston body, "ext" at future extension
	
	m_World->BroadcastBlockAction(pistx, pisty, pistz, 0, pistonMeta, pistonBlock);
	m_World->BroadcastSoundEffect("tile.piston.out", pistx * 8, pisty * 8, pistz * 8, 0.5f, 0.7f);
	m_World->FastSetBlock( pistx, pisty, pistz, pistonBlock, pistonMeta | 0x8 );
	m_World->SetServerBlock(extx, exty, extz, E_BLOCK_PISTON_EXTENSION, pistonMeta | (IsSticky(pistonBlock) ? 8 : 0));
}





void cPiston::RetractPiston( int pistx, int pisty, int pistz )
{
	BLOCKTYPE pistonBlock;
	NIBBLETYPE pistonMeta;
	m_World->GetBlockTypeMeta(pistx, pisty, pistz, pistonBlock, pistonMeta);
	if (!IsExtended(pistonMeta))
	{
		// Already retracted, bail out
		return;
	}
	
	m_World->BroadcastBlockAction(pistx, pisty, pistz, 1, pistonMeta & ~(8), pistonBlock);
	m_World->BroadcastSoundEffect("tile.piston.in", pistx * 8, pisty * 8, pistz * 8, 0.5f, 0.7f);
	m_World->SetServerBlock(pistx, pisty, pistz, pistonBlock, pistonMeta & ~(8));


	// Check the extension:
	AddDir(pistx, pisty, pistz, pistonMeta, 1);
	if (m_World->GetBlock(pistx, pisty, pistz) != E_BLOCK_PISTON_EXTENSION)
	{
		LOGD("%s: Piston without an extension?", __FUNCTION__);
		return;
	}
	
	// Retract the extension, pull block if appropriate
	if (IsSticky(pistonBlock))
	{
		int tempx = pistx, tempy = pisty, tempz = pistz;
		AddDir( tempx, tempy, tempz, pistonMeta, 1);
		BLOCKTYPE tempBlock;
		NIBBLETYPE tempMeta;
		m_World->GetBlockTypeMeta(tempx, tempy, tempz, tempBlock, tempMeta);
		if (CanPull(tempBlock, tempMeta))
		{
			// Pull the block
			m_World->SetServerBlock(pistx, pisty, pistz, tempBlock, tempMeta);
			m_World->SetServerBlock(tempx, tempy, tempz, E_BLOCK_AIR, 0);
		}
		else
		{
			// Retract without pulling
			m_World->SetServerBlock(pistx, pisty, pistz, E_BLOCK_AIR, 0);
		}
	}
	else
	{
		m_World->SetServerBlock(pistx, pisty, pistz, E_BLOCK_AIR, 0);
	}
}





bool cPiston::IsExtended(NIBBLETYPE a_PistonMeta)
{
	return ((a_PistonMeta & 0x8) != 0x0);
}





bool cPiston::IsSticky(BLOCKTYPE a_BlockType)
{
	return (a_BlockType == E_BLOCK_STICKY_PISTON);
}





bool cPiston::IsStickyExtension(NIBBLETYPE a_ExtMeta)
{
	return ((a_ExtMeta & 0x08) != 0);
}





bool cPiston::CanPush(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	switch (a_BlockType)
	{
		case E_BLOCK_ANVIL:
		case E_BLOCK_BED:
		case E_BLOCK_BEDROCK:
		case E_BLOCK_BREWING_STAND:
		case E_BLOCK_CHEST:
		case E_BLOCK_COMMAND_BLOCK:
		case E_BLOCK_DISPENSER:
		case E_BLOCK_DROPPER:
		case E_BLOCK_ENCHANTMENT_TABLE:
		case E_BLOCK_END_PORTAL:
		case E_BLOCK_END_PORTAL_FRAME:
		case E_BLOCK_FURNACE:
		case E_BLOCK_LIT_FURNACE:
		case E_BLOCK_HOPPER:
		case E_BLOCK_JUKEBOX:
		case E_BLOCK_MOB_SPAWNER:
		case E_BLOCK_NETHER_PORTAL:
		case E_BLOCK_NOTE_BLOCK:
		case E_BLOCK_OBSIDIAN:
		case E_BLOCK_PISTON_EXTENSION:
		{
			return false;
		}
		case E_BLOCK_PISTON:
		{
			// A piston can only be pushed if retracted:
			return !IsExtended(a_BlockMeta);
		}
	}
	return true;
}





bool cPiston::CanBreakPush(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	return g_BlockPistonBreakable[a_BlockType];
}





bool cPiston::CanPull(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	switch (a_BlockType)
	{
		case E_BLOCK_LAVA:
		case E_BLOCK_STATIONARY_LAVA:
		case E_BLOCK_STATIONARY_WATER:
		case E_BLOCK_WATER:
		{
			return false;
		}
	}
	return CanPush(a_BlockType, a_BlockMeta) || CanBreakPush(a_BlockType, a_BlockMeta);
}





void cPiston::AddDir(int & a_BlockX, int & a_BlockY, int & a_BlockZ, NIBBLETYPE a_PistonMeta, int a_Amount)
{
	switch (a_PistonMeta & 0x07)
	{
		case 0: a_BlockY -= a_Amount; break;
		case 1: a_BlockY += a_Amount; break;
		case 2: a_BlockZ -= a_Amount; break;
		case 3: a_BlockZ += a_Amount; break;
		case 4: a_BlockX -= a_Amount; break;
		case 5: a_BlockX += a_Amount; break;
		default:
		{
			LOGWARNING("%s: invalid direction %d, ignoring", __FUNCTION__, a_PistonMeta & 0x07);
			break;
		}
	}
}




