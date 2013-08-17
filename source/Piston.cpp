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




//Replace AddDir (...) with the switch cases that, sets coords in direction of piston face
#define AddDir( x, y, z, dir, amount ) \
	switch (dir) \
	{ \
		case 0: (y)-=(amount); break; \
		case 1: (y)+=(amount); break; \
		case 2: (z)-=(amount); break; \
		case 3: (z)+=(amount); break; \
		case 4: (x)-=(amount); break; \
		case 5: (x)+=(amount); break; \
	}





cPiston::cPiston(cWorld * a_World)
	: m_World(a_World)
{

}





unsigned short cPiston::FirstPassthroughBlock(int pistonX, int pistonY, int pistonZ, char pistonmeta)
{
	unsigned short ret;
	pistonmeta &= 7;
	if (pistonmeta >= 6)
	{
		// Just in case, it shouldn't happen but if it would, it'd cause inf loop
		LOGD("cPiston::FirstPassthroughBlock - piston has invalid meta data!\n");
		return 9001;
	}
	BLOCKTYPE currBlock;
	NIBBLETYPE currMeta;
	for (ret = 0; ret < 24; ret++)  //Push up to 24 blocks
	{
		AddDir( pistonX, pistonY, pistonZ, pistonmeta, 1) //Set the coords one further from the piston direction
		m_World->GetBlockTypeMeta(pistonX, pistonY, pistonZ, currBlock, currMeta);
		if ((currBlock == E_BLOCK_BEDROCK) || (currBlock == E_BLOCK_OBSIDIAN) || (currBlock == E_BLOCK_PISTON_EXTENSION) || ( (currMeta & 0x8) != 0x0 ))
		{
			return 9001;
		}
		if (g_BlockPistonBreakable[currBlock]) //If it's a breakable block (air, torch, etc.) then the line of blocks can be pushed
		{
			return ret;
		}
	}
	return 9001; //There is no space for the blocks to move within 24 spaces, piston can't push
}





void cPiston::ExtendPiston( int pistx, int pisty, int pistz )
{
	BLOCKTYPE pistonBlock;
	NIBBLETYPE pistonMeta;
	m_World->GetBlockTypeMeta(pistx, pisty, pistz, pistonBlock, pistonMeta);
	
	char isSticky = (char)(pistonBlock == E_BLOCK_STICKY_PISTON) * 8;
	if ( (pistonMeta & 0x8) != 0x0 )
	{
		// Piston already extended, bail out
		return;
	}
	
	unsigned short dist = FirstPassthroughBlock(pistx, pisty, pistz, pistonMeta);
	if (dist > 9000) return; // FirstPassthroughBlock says piston can't push anything, bail out

	AddDir(pistx, pisty, pistz, pistonMeta & 7, dist + 1) //Get the coords of the air / breakable block in the line, dist+1 because of coords
	BLOCKTYPE currBlock;
	NIBBLETYPE currMeta;
	m_World->GetBlockTypeMeta(pistx, pisty, pistz, currBlock, currMeta);
	if (currBlock != E_BLOCK_AIR)
	{
		cBlockHandler * Handler = BlockHandler(currBlock);
		if (Handler->DoesDropOnUnsuitable())
		{
			Handler->DropBlock(m_World, NULL, pistx, pisty, pistz); //If block is breakable, drop it
		}
	}
	int oldx = pistx, oldy = pisty, oldz = pistz; //Make a second set of coord vars along with the originals
	NIBBLETYPE currBlockMeta;
	if (dist != 0) //Check for single block being pushed - for loop doesn't catch it in time and breaks stuffz
	{
		for (int i = dist + 1; i > 1; i--) //Decrement from the dropped breakable block to two further than the piston (one further will be future extension)
		{
			AddDir(pistx, pisty, pistz, pistonMeta & 7, -1) //Move one set of coords one back from breakable dropped block
			m_World->GetBlockTypeMeta(pistx, pisty, pistz, currBlock, currBlockMeta); //Get the block
			m_World->SetBlock( oldx, oldy, oldz, currBlock, currBlockMeta); //Set the block at the location of the original coords
			oldx = pistx; //Shift the selectors down a block and repeat
			oldy = pisty;
			oldz = pistz;
		}
	}

	int extx = pistx;
	int exty = pisty;
	int extz = pistz;

	AddDir(pistx, pisty, pistz, pistonMeta & 7, -1) //Move back one block to the piston base
	m_World->BroadcastBlockAction(pistx, pisty, pistz, 0, pistonMeta, E_BLOCK_PISTON); //Set the base
	m_World->BroadcastSoundEffect("tile.piston.out", pistx * 8, pisty * 8, pistz * 8, 0.5f, 0.7f);
	m_World->FastSetBlock( pistx, pisty, pistz, pistonBlock, pistonMeta | 0x8 );
	m_World->SetBlock(extx, exty, extz, E_BLOCK_PISTON_EXTENSION, isSticky + pistonMeta); //Set the arm
}





void cPiston::RetractPiston( int pistx, int pisty, int pistz )
{
	BLOCKTYPE pistonBlock;
	NIBBLETYPE pistonMeta;
	m_World->GetBlockTypeMeta(pistx, pisty, pistz, pistonBlock, pistonMeta);
	if (pistonMeta <= 6)
	{
		// Already retracted, bail out
		return;
	}
	m_World->BroadcastBlockAction(pistx, pisty, pistz, 1, pistonMeta & ~(8), E_BLOCK_PISTON);
	m_World->BroadcastSoundEffect("tile.piston.in", pistx * 8, pisty * 8, pistz * 8, 0.5f, 0.7f);
	m_World->FastSetBlock(pistx, pisty, pistz, pistonBlock, pistonMeta & ~(8)); //Set the base

	AddDir(pistx, pisty, pistz, pistonMeta & 7, 1) //Move forwards to the extension coord
	if (m_World->GetBlock(pistx, pisty, pistz) != E_BLOCK_PISTON_EXTENSION)
	{
		LOGD("%s: Piston without an extension?", __FUNCTION__);
		return;
	}
	
	if (pistonBlock == E_BLOCK_STICKY_PISTON)
	{
		int tempx = pistx, tempy = pisty, tempz = pistz;
		AddDir( tempx, tempy, tempz, pistonMeta & 7, 1 ) //Move forward to the block being pulled
		BLOCKTYPE tempblock;
		NIBBLETYPE tempmeta;
		m_World->GetBlockTypeMeta(tempx, tempy, tempz, tempblock, tempmeta); //Check for pullable-ness
		if (
			(tempblock == E_BLOCK_OBSIDIAN) || 
			(tempblock == E_BLOCK_BEDROCK) || 
			(tempblock == E_BLOCK_PISTON_EXTENSION) ||
			(g_BlockPistonBreakable[tempblock]) ||
			((tempmeta & 0x8) != 0x0 )
		)
		{
			// These cannot be moved by the sticky piston, bail out
			m_World->SetBlock(pistx, pisty, pistz, E_BLOCK_AIR, 0);
			return;
		}

		m_World->SetBlock(pistx, pisty, pistz, tempblock, tempmeta);
		m_World->SetBlock(tempx, tempy, tempz, E_BLOCK_AIR, 0);
	}
	else
	{

		m_World->SetBlock(pistx, pisty, pistz, E_BLOCK_AIR, 0);
	}
}




