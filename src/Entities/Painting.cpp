
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Painting.h"
#include "Player.h"
#include "../ClientHandle.h"
#include "../Chunk.h"





cPainting::cPainting(const AString & a_Name, eBlockFace a_Direction, Vector3d a_Pos, UInt32 a_id):
	Super(etPainting, a_Direction, a_Pos),
	m_Name(a_Name),
	m_id(a_id)
{
}





void cPainting::SpawnOn(cClientHandle & a_Client)
{
	Super::SpawnOn(a_Client);
	a_Client.SendPaintingSpawn(*this);

	m_World->BroadcastSoundEffect("entity.painting.place", GetPosition(), 1, 1);
}





void cPainting::GetDrops(cItems & a_Items, cEntity * a_Killer)
{
	a_Items.emplace_back(Item::Painting);
}





void cPainting::KilledBy(TakeDamageInfo & a_TDI)
{
	Super::KilledBy(a_TDI);

	m_World->BroadcastSoundEffect("entity.painting.break", GetPosition(), 1, 1);
}
