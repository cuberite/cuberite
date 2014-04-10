
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Painting.h"
#include "ClientHandle.h"
#include "Player.h"
#include "../Chunk.h"





cPainting::cPainting(const AString & a_Name, int a_Direction, double a_X, double a_Y, double a_Z)
	: cEntity(etPainting, a_X, a_Y, a_Z, 1, 1),
	m_Name(a_Name),
	m_Direction(a_Direction)
{
}






void cPainting::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendPaintingSpawn(*this);
}





void cPainting::Tick(float a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
}





void cPainting::GetDrops(cItems & a_Items, cEntity * a_Killer)
{
	if ((a_Killer != NULL) && a_Killer->IsPlayer() && !((cPlayer *)a_Killer)->IsGameModeCreative())
	{
		a_Items.push_back(cItem(E_ITEM_PAINTING));
	}
}




