
// BlockInServerPluginInterface.h

// Defines the cBlockInServerPluginInterface class that implements the cBlockPluginInterface for blocks, using the plugin manager





#pragma once

#include "Blocks/BlockPluginInterface.h"
#include "World.h"
#include "Bindings/PluginManager.h"





class cBlockInServerPluginInterface :
	public cBlockPluginInterface
{
public:
	cBlockInServerPluginInterface(cWorld & a_World) : m_World(a_World) {}

	virtual bool CallHookBlockSpread(int a_BlockX, int a_BlockY, int a_BlockZ, eSpreadSource a_Source) override
	{
		return cPluginManager::Get()->CallHookBlockSpread(m_World, a_BlockX, a_BlockY, a_BlockZ, a_Source);
	}

	virtual bool CallHookBlockToPickups(cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cItems & a_Pickups) override
	{
		return cPluginManager::Get()->CallHookBlockToPickups(m_World, a_Digger, a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta, a_Pickups);
	}

	virtual bool CallHookPlayerBreakingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override
	{
		return cPluginManager::Get()->CallHookPlayerBreakingBlock(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_BlockType, a_BlockMeta);
	}

	virtual bool CallHookPlayerBrokenBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override
	{
		return cPluginManager::Get()->CallHookPlayerBrokenBlock(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_BlockType, a_BlockMeta);
	}

private:
	cWorld & m_World;
};




