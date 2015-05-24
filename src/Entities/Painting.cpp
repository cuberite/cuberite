
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Painting.h"
#include "ClientHandle.h"
#include "Player.h"
#include "../Chunk.h"





cPainting::cPainting(const AString & a_Name, eBlockFace a_Direction, double a_X, double a_Y, double a_Z)
	: cHangingEntity(etPainting, a_Direction, a_X, a_Y, a_Z),
	m_Name(a_Name)
{
}






void cPainting::SpawnOn(cClientHandle & a_Client)
{
	super::SpawnOn(a_Client);
	a_Client.SendPaintingSpawn(*this);
}





void cPainting::GetDrops(cItems & a_Items, cEntity * a_Killer)
{
	if ((a_Killer != nullptr) && a_Killer->IsPlayer() && !static_cast<cPlayer *>(a_Killer)->IsGameModeCreative())
	{
		a_Items.push_back(cItem(E_ITEM_PAINTING));
	}
}




