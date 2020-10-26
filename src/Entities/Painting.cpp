
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Painting.h"
#include "Player.h"
#include "../ClientHandle.h"
#include "../Chunk.h"





cPainting::cPainting(const AString & a_Name, eBlockFace a_Direction, Vector3d a_Pos):
	Super(etPainting, a_Direction, a_Pos),
	m_Name(a_Name)
{
}





void cPainting::SpawnOn(cClientHandle & a_Client)
{
	Super::SpawnOn(a_Client);
	a_Client.SendPaintingSpawn(*this);
}





void cPainting::GetDrops(cItems & a_Items, cEntity * a_Killer)
{
	if ((a_Killer != nullptr) && a_Killer->IsPlayer() && !static_cast<cPlayer *>(a_Killer)->IsGameModeCreative())
	{
		a_Items.push_back(cItem(E_ITEM_PAINTING));
	}
}




