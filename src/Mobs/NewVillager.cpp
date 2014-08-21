
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "NewVillager.h"
#include "../World.h"
#include "../BlockArea.h"
#include "../Blocks/BlockHandler.h"
#include "../BlockInServerPluginInterface.h"





cNewVillager::cNewVillager(eVillagerType VillagerType) :
	super("Villager", mtVillager, "", "", 0.6, 1.8),
	m_Type(VillagerType)
{
}

