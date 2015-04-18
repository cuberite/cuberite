
#pragma once

#include "Blocks/BlockPluginInterface.h"
#include "World.h"
#include "Root.h"
#include "Bindings/PluginManager.h"





class cBlockInServerPluginInterface :
	public cBlockPluginInterface
{
public:
	cBlockInServerPluginInterface(cWorld & a_World) : m_World(a_World) {}

	virtual bool CallHookBlockSpread(int a_BlockX, int a_BlockY, int a_BlockZ, eSpreadSource a_Source)
	{
		return cPluginManager::Get()->CallHook(cPluginManager::HOOK_BLOCK_SPREAD, &m_World, a_BlockX, a_BlockY, a_BlockZ, a_Source);
	}

	virtual bool CallHookBlockToPickups(cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cItems & a_Pickups) override
	{
		return cPluginManager::Get()->CallHook(cPluginManager::HOOK_BLOCK_TO_PICKUPS, &m_World, a_Digger, a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta, a_Pickups);
	}

private:
	cWorld & m_World;
};




