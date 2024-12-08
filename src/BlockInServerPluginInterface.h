
// BlockInServerPluginInterface.h

// Defines the cBlockInServerPluginInterface class that implements the cBlockPluginInterface for blocks, using the plugin manager





#pragma once

#include "Blocks/BlockPluginInterface.h"
#include "Bindings/PluginManager.h"


class cWorld;


class cBlockInServerPluginInterface :
	public cBlockPluginInterface
{
public:
	cBlockInServerPluginInterface(cWorld & a_World) : m_World(a_World) {}

	virtual bool CallHookBlockSpread(Vector3i a_BlockPos, eSpreadSource a_Source) override
	{
		return cPluginManager::Get()->CallHookBlockSpread(m_World, a_BlockPos, a_Source);
	}

	virtual bool CallHookPlayerBreakingBlock(cPlayer & a_Player, Vector3i a_BlockPos, eBlockFace a_BlockFace, BlockState a_Block) override
	{
		return cPluginManager::Get()->CallHookPlayerBreakingBlock(a_Player, a_BlockPos, a_BlockFace, a_Block);
	}

	virtual bool CallHookPlayerBrokenBlock(cPlayer & a_Player, Vector3i a_BlockPos, eBlockFace a_BlockFace, BlockState a_Block) override
	{
		return cPluginManager::Get()->CallHookPlayerBrokenBlock(a_Player, a_BlockPos, a_BlockFace, a_Block);
	}

private:
	cWorld & m_World;
};




